#include "generatorwindow.h"
#include "ui_generatorwindow.h"

GeneratorWindow::GeneratorWindow(QWidget *parent)
    : QMainWindow(parent), totalTestTasks(0), lastSizeCount(0), pageNumber(0), mode(false),
      random(QRandomGenerator::global()), ui(new Ui::GeneratorWindow)
{
    uploadSettings();
    uploadUI();
    createTheoryImages();

    connect(ui->toolBar->actions().at(Check), &QAction::triggered, this, &GeneratorWindow::checkAnswers);
    connect(ui->toolBar->actions().at(Clear), &QAction::triggered, this, &GeneratorWindow::clearTasks);
    connect(ui->toolBar->actions().at(Print), &QAction::triggered, this, &GeneratorWindow::printTasks);
    connect(ui->toolBar->actions().at(Manual), &QAction::triggered, this, &GeneratorWindow::openManual);
    connect(ui->toolBar->actions().at(Font), &QAction::triggered, this, &GeneratorWindow::changeFontSize);
    connect(ui->toolBar->actions().at(Exit), &QAction::triggered, [&](){qApp->closeAllWindows();});
    connect(ui->prevPage, &QPushButton::clicked, this, &GeneratorWindow::prevTheoryPage);
    connect(ui->nextPage, &QPushButton::clicked, this, &GeneratorWindow::nextTheoryPage);

    showMaximized();
}

GeneratorWindow::~GeneratorWindow()
{
    delete engine;
    delete ui;
}

void GeneratorWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    event->accept();
}

void GeneratorWindow::uploadUI()
{
    ui->setupUi(this);
    ui->genButton->setEnabled(true);
    ui->genButton->setCursor(Qt::PointingHandCursor);
    ui->pushButton->setCursor(Qt::PointingHandCursor);
    ui->tabWidget->tabBar()->setCursor(Qt::PointingHandCursor);
    ui->taskView->setCursor(Qt::BlankCursor);
    ui->taskView->setContextMenuPolicy(Qt::CustomContextMenu);
    engine = new TeXEngine(ui->taskView);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    ui->toolBar->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->toolBar->addAction(new QAction(QPixmap("://general/img/checkAnswers.png"), "Показать ответы", ui->toolBar));
    ui->toolBar->addAction(new QAction(QPixmap("://general/img/clearTasks.png"), "Очистить задачи", ui->toolBar));
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(new QAction(QPixmap("://general/img/printer.png"), "Подготовить печатный вариант...", ui->toolBar));
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(new QAction(QPixmap("://general/img/manual.png"), "Справочник", ui->toolBar));
    ui->toolBar->addAction(new QAction(QPixmap("://general/img/font-size.png"), "Настройки шрифта", ui->toolBar));
    ui->toolBar->addAction(new QAction(QPixmap("://general/img/quit.png"), "Выход...", ui->toolBar));
    ui->toolBar->actions().at(0)->setDisabled(true);
    ui->toolBar->actions().at(1)->setDisabled(true);
    ui->toolBar->actions().at(3)->setDisabled(true);

    for (int i = 0; i < ui->tasksList->count(); ++i)
    {
        ui->comboBox->addItem(ui->tasksList->item(i)->text());
    }

    setCursor(Qt::ArrowCursor);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(App::AppName);
    setWindowIcon(QIcon(":/general/img/appIcon.png"));
}

void GeneratorWindow::saveSettings()
{
    QSettings settings(App::OrgName, App::AppName);

    settings.beginGroup("General");
    settings.setValue("windowSize", geometry());
    settings.endGroup();

    settings.beginGroup("Style");
    settings.setValue("taskFontSize", taskFontSize);
    settings.setValue("mathFontSize", mathFontSize);
    settings.endGroup();

    settings.beginGroup("Theory");
    settings.setValue("numberOfPage", pageNumber);
    settings.endGroup();
}

void GeneratorWindow::uploadSettings()
{
    QSettings settings(App::OrgName, App::AppName);

    settings.beginGroup("General");
    setGeometry(settings.value("windowSize").toRect());
    settings.endGroup();

    settings.beginGroup("Style");
    taskFontSize = settings.value("taskFontSize", "\\normalsize").toString();
    mathFontSize = settings.value("mathFontSize", "\\normalsize").toString();
    settings.endGroup();

    settings.beginGroup("Theory");
    pageNumber = settings.value("numberOfPage").toInt();
    settings.endGroup();
}

void GeneratorWindow::createTheoryImages()
{
    QFile pdfSource(RSC::theoryPath);
    if (!pdfSource.open(QFile::ReadOnly)) return;
    Poppler::Document *document = Poppler::Document::loadFromData(pdfSource.readAll());
    pdfSource.close();
    if (!document || document->isLocked()) {
      qDebug() << "ERROR WITH POPPLER";
      delete document;
      return;
    }
    document->setRenderHint(Poppler::Document::TextAntialiasing);
    Poppler::Page* pdfPage; QImage image;
    for (int i = 0; i < document->numPages(); ++i)
    {
        pdfPage = document->page(i);
        image = pdfPage->renderToImage(135.0, 135.0);
        images.push_back(image);
        delete pdfPage;
    }

    if (pageNumber == 0) ui->prevPage->setDisabled(true);
    else if (pageNumber == images.size() - 1) ui->nextPage->setDisabled(true);

    scene->addPixmap(QPixmap::fromImage(images[pageNumber]));
    scene->setSceneRect(images[pageNumber].rect());
    delete document;
}

void GeneratorWindow::prevTheoryPage()
{
    if (!ui->nextPage->isEnabled()) ui->nextPage->setEnabled(true);

    if (pageNumber > 0) {
        scene->addPixmap(QPixmap::fromImage(images[--pageNumber]));
        if (pageNumber == 0)
            ui->prevPage->setDisabled(true);
    }
}

void GeneratorWindow::nextTheoryPage()
{
    if (!ui->prevPage->isEnabled()) ui->prevPage->setEnabled(true);

    if (pageNumber < images.size() - 1) {
        scene->addPixmap(QPixmap::fromImage(images[++pageNumber]));
        if (pageNumber == images.size() - 1)
            ui->nextPage->setDisabled(true);
    }
}

void GeneratorWindow::isReadyRender(){
    QApplication::setOverrideCursor(Qt::WaitCursor);
    for (int i = lastSizeCount; i < descriptions.size(); ++i)
    {
        taskForTeX.append("{TASK_FONT\\textbf{" + descriptions.at(i) + ":}}\\\\");
        for (int j = 0; j < tasks.at(i).size(); ++j)
        {
            taskForTeX.append("{MATH_FONT" + QString::number(j + 1) + ")~" + tasks.at(i).at(j) + "}\\\\");
        }
    }
    lastSizeCount = descriptions.size();
    QString TeXBuffer = taskForTeX;
    TeXBuffer.replace("TASK_FONT", taskFontSize).replace("MATH_FONT", mathFontSize);
    engine->TeX2SVG(MathJax::startConf + TeXBuffer + MathJax::endConf, true);
    QApplication::restoreOverrideCursor();
}

void GeneratorWindow::checkAnswers(){
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QString TeXBuffer = taskForTeX;
    TeXBuffer.replace("TASK_FONT", taskFontSize).replace("MATH_FONT", mathFontSize);
    for (int i = 0; i < answers.size(); i++)
        TeXBuffer.replace(TeXBuffer.indexOf('?'), QString("?").size(), answers[i]);

    engine->TeX2SVG(MathJax::startConf + TeXBuffer + MathJax::endConf, true);
    ui->toolBar->actions().at(Check)->setDisabled(true);
    QApplication::restoreOverrideCursor();
}

void GeneratorWindow::clearTasks()
{
    descriptions.clear();
    tasks.clear();
    answers.clear();
    taskForTeX.clear();
    lastSizeCount = 0;
    engine->TeX2SVG(MathJax::startConf + taskFontSize + "{\\textbf{В ожидании генерации задач...}}" + MathJax::endConf, true);
    ui->toolBar->actions().at(Check)->setDisabled(true);
    ui->toolBar->actions().at(Clear)->setDisabled(true);
    ui->toolBar->actions().at(Print)->setDisabled(true);
}

void GeneratorWindow::printTasks()
{
    DialogLatexPrinter *latexDialog = new DialogLatexPrinter(descriptions, tasks, answers, this);
    latexDialog->exec();
}

void GeneratorWindow::openManual()
{
    DialogManual *manDialog = new DialogManual(taskFontSize, mathFontSize, this);
    manDialog->show();
}

void GeneratorWindow::changeFontSize()
{
    DialogFontSize *fontDialog = new DialogFontSize(taskFontSize, mathFontSize, this);

    connect(fontDialog, &DialogFontSize::changeTaskFontSize, [&](QString newFont){
        taskFontSize = newFont;
    });
    connect(fontDialog, &DialogFontSize::changeMathFontSize, [&](QString newFont){
        mathFontSize = newFont;
    });

    fontDialog->exec();
}

void GeneratorWindow::on_tabWidget_currentChanged(int index)
{
    switch (index) {
    case 2:
        mode = 1;
        ui->toolBar->actions().at(Check)->setVisible(false);
        ui->toolBar->actions().at(Clear)->setVisible(false);
        ui->toolBar->actions().at(Sep_1)->setVisible(false);
        ui->toolBar->actions().at(Print)->setVisible(false);
        ui->toolBar->actions().at(Sep_2)->setVisible(false);
        break;
    default:
        mode = 0;
        if (index == 0) {
            ui->toolBar->actions().at(Check)->setVisible(true);
            ui->toolBar->actions().at(Clear)->setVisible(true);
            ui->toolBar->actions().at(Sep_1)->setVisible(true);
            ui->toolBar->actions().at(Print)->setVisible(true);
            ui->toolBar->actions().at(Sep_2)->setVisible(true);
        }
        else {
            ui->toolBar->actions().at(Check)->setVisible(false);
            ui->toolBar->actions().at(Clear)->setVisible(false);
            ui->toolBar->actions().at(Sep_1)->setVisible(false);
            ui->toolBar->actions().at(Print)->setVisible(false);
            ui->toolBar->actions().at(Sep_2)->setVisible(false);
        }
        break;
    }
}

void GeneratorWindow::on_pushButton_clicked()
{
    if (tasksForTest.isEmpty())
        statusBar()->showMessage("Перед запуском теста, необходимо сгенерировать задания!", 1500);
    else {
        DialogTestTimer *window = new DialogTestTimer(this);
        connect(window, &DialogTestTimer::timeRemaining, this, &GeneratorWindow::startTest);
    }
}

void GeneratorWindow::startTest(QTime time)
{
    //Рандомная сортировка
    for (size_t i = 0; i < static_cast<size_t>(tasksForTest.size()); ++i) {
        int j = random->bounded(0, tasksForTest.size());
        auto tmp = tasksForTest[i];
        tasksForTest[i] = tasksForTest[j];
        tasksForTest[j] = tmp;
    }
    TestMode *testWindow = new TestMode(tasksForTest, time, this);
    testWindow->setWindowTitle("Тест");
    testWindow->showMaximized();
    totalTestTasks = 0;
    ui->lcdNumber->display(totalTestTasks);
}

void GeneratorWindow::on_genButton_clicked()
{
    if (ui->tasksList->currentItem() == nullptr) {
        statusBar()->showMessage("Перед генерацией выберите типаж задания!");
        return;
    }
    runTaskManager(ui->tasksList->currentItem()->text(), true);
}

void GeneratorWindow::on_comboBox_currentTextChanged(const QString &task)
{
    runTaskManager(task, false);
}

void GeneratorWindow::runTaskManager(const QString &task, bool closeMode)
{
    if (!ui->mainLayout->isEmpty() && !closeMode)
        delete ui->mainLayout->takeAt(0)->widget();

    DialogBase *window;
    if (task == "Группа Перестановок")
        window = new DialogBase(AllTasks::TranspositionGroup, closeMode, this);
    else if (task == "Функция Эйлера")
        window = new DialogBase(AllTasks::EulerFunction, closeMode, this);
    else if (task == "Функция Мёбиуса")
        window = new DialogBase(AllTasks::MebiusFunction, closeMode, this);
    else if (task == "Символ Лежандра")
        window = new DialogBase(AllTasks::SymbolLegandre, closeMode, this);
    else if (task == "Символ Якоби")
        window = new DialogBase(AllTasks::SymbolJacobi, closeMode, this);
    else if (task == "Алгебраические Структуры")
        window = new DialogBase(AllTasks::Set, closeMode, this);
    else if (task == "Группы и их свойства")
        window = new DialogBase(AllTasks::GroupProperties, closeMode, this);
    else if (task == "Матрицы")
        window = new DialogBase(AllTasks::Matrix, closeMode, this);
    else if (task == "Группы и их свойства")
        window = new DialogBase(AllTasks::GroupProperties, closeMode, this);
    else if (task == "Кольцо Вычетов")
        window = new DialogBase(AllTasks::RingResidue, closeMode, this);
    else if (task == "Кольцо Многочленов")
        window = new DialogBase(AllTasks::RingOfMembers, closeMode, this);

    connect(window, &DialogBase::sendingMetaInfo, this, &GeneratorWindow::receivedMetaInfo);
    connect(window, &DialogBase::sendingData, this, &GeneratorWindow::receivedData);

    if (!closeMode)
        ui->mainLayout->addWidget(window);
}

void GeneratorWindow::receivedMetaInfo(int countOfTasks)
{
    if (mode) {
        totalTestTasks += countOfTasks;
        statusBar()->showMessage("Задания сгенерированы!", 1500);
        ui->lcdNumber->display(totalTestTasks);
    } else {
        ui->toolBar->actions().at(Check)->setEnabled(true);
        ui->toolBar->actions().at(Clear)->setEnabled(true);
        ui->toolBar->actions().at(Print)->setEnabled(true);
    }
}

void GeneratorWindow::receivedData(std::vector<int> data, AllTasks task, int subTask, int optional)
{
    //If tasksCount is null
    if (data[0] == 0) return;

    switch (task) {
    case AllTasks::EulerFunction:
        runEulerFunction(data[0], data[1], data[2], static_cast<EulerFunctionOptions>(subTask));
        break;
    case AllTasks::MebiusFunction:
        runMebiusFunction(data[0], data[1], data[2], static_cast<MebiusFunctionOptions>(subTask));
        break;
    case AllTasks::SymbolLegandre:
        runSymbolLegandre(data[0], std::make_pair(data[1], data[2]),
                std::make_pair(data[3], data[4]), static_cast<SymbolLegandreOptions>(subTask));
        break;
    case AllTasks::SymbolJacobi:
        runSymbolJacobi(data[0], std::make_pair(data[1], data[2]),
                std::make_pair(data[3], data[4]), static_cast<SymbolJacobiOptions>(subTask));
        break;
    case AllTasks::TranspositionGroup:
        runTranspositionGroup(data[0], data[1], data[2], static_cast<TranspositionGroupOptions>(subTask), static_cast<ViewMode>(optional));
        break;
    case AllTasks::Set:
        runSet(data[0], static_cast<SetOptions>(subTask));
        break;
    case AllTasks::GroupProperties:
        runGroupProperties(data[0], data[1], data[2], static_cast<GroupPropertiesOptions>(subTask));
        break;
    case AllTasks::Matrix:
        runMatrix(data[0], std::make_pair(data[1], data[2]),
                std::make_pair(data[3], data[4]), static_cast<MatrixOptions>(subTask));
        break;
    case AllTasks::RingResidue:
        runRingResidue(data[0], data[1], data[2], static_cast<RingResidueOptions>(subTask));
    case AllTasks::RingOfMembers:
        runRingOfMembers(data[0], std::make_pair(data[1], data[2]), std::make_pair(data[3], data[4]),
                static_cast<RingOfMembersOptions>(subTask), static_cast<Set>(optional));
    }
}

void GeneratorWindow::runEulerFunction(int countOfTasks, int minNum, int maxNum, EulerFunctionOptions option)
{
    EulerFunction task;
    QVector<QString> tasks; QString buffer;
    switch (option) {
    case EulerFunctionOptions::Default:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            task.setTask(random->bounded(minNum, maxNum));
            if (!mode) {
                buffer = "\\varphi(" + QString::number(task.getTask()) + ")=~?";
                tasks.push_back(std::move(buffer));
                answers.push_back(QString::number(task.solve()));
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Вычислите~функцию~Эйлера:\\\\\\varphi(" + QString::number(task.getTask()) + ")=~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
            }
        } break;
    case EulerFunctionOptions::Primes:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minNum, maxNum));
            if (isPrime(task.getTask())) {
                if (!mode) {
                    buffer = "\\varphi(" + QString::number(task.getTask()) + ")=~?";
                    tasks.push_back(std::move(buffer));
                    answers.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~функцию~Эйлера:\\\\\\varphi(" + QString::number(task.getTask()) + ")=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i;
            }
        } break;
    case EulerFunctionOptions::PrimesDegrees:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minNum, maxNum));
            auto data = decompositionToSimple(task.getTask());
            if (data.size() == 1 && data[0].second > 1) {
                if (!mode) {
                    buffer = "\\varphi(" + QString::number(task.getTask()) + ")=~?";
                    tasks.push_back(std::move(buffer));
                    answers.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~функцию~Эйлера:\\\\\\varphi(" + QString::number(task.getTask()) + ")=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i;
            }
        } break;
    case EulerFunctionOptions::MultiplyPrimes:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minNum, maxNum));
            auto data = decompositionToSimple(task.getTask());
            if (data.size() == 1) continue;
            bool accessFlag = true;
            for (size_t i = 0; i < data.size(); ++i) {
                if (data[i].second != 1) {
                    accessFlag = false;
                    break;
                }
            } if (accessFlag) {
                if (!mode) {
                    buffer = "\\varphi(" + QString::number(task.getTask()) + ")=~?";
                    tasks.push_back(std::move(buffer));
                    answers.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~функцию~Эйлера:\\\\\\varphi(" + QString::number(task.getTask()) + ")=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i;
            }
        } break;
    case EulerFunctionOptions::MultiplyPrimesDegrees:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minNum, maxNum));
            auto data = decompositionToSimple(task.getTask());
            if (data.size() == 1) continue;
            bool accessFlag = true;
            for (size_t i = 0; i < data.size(); ++i){
                if (data[i].second == 1) {
                    accessFlag = false;
                    break;
                }
            } if (accessFlag) {
                if (!mode) {
                    buffer = "\\varphi(" + QString::number(task.getTask()) + ")=~?";
                    tasks.push_back(std::move(buffer));
                    answers.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~функцию~Эйлера:\\\\\\varphi(" + QString::number(task.getTask()) + ")=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i;
            }
        } break;
    }

    if (!mode) {
        descriptions.push_back(std::move(QString("Вычислите функцию Эйлера")));
        this->tasks.push_back(std::move(tasks));
        isReadyRender();
    }
}

void GeneratorWindow::runMebiusFunction(int countOfTasks, int minNum, int maxNum, MebiusFunctionOptions option)
{
    MebiusFunction task;
    QVector<QString> tasks; QString buffer;
    switch (option) {
    case MebiusFunctionOptions::Default:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            task.setTask(random->bounded(minNum, maxNum));
            if (!mode) {
                buffer = "\\mu(" + QString::number(task.getTask()) + ")=~?";
                tasks.push_back(std::move(buffer));
                answers.push_back(QString::number(task.solve()));
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Вычислите~функцию~Мёбиуса:\\\\\\mu(" + QString::number(task.getTask()) + ")=~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
            }
        } break;
    case MebiusFunctionOptions::Primes:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minNum, maxNum));
            if (!isPrime(task.getTask())) {
                if (!mode) {
                    buffer = "\\mu(" + QString::number(task.getTask()) + ")=~?";
                    tasks.push_back(std::move(buffer));
                    answers.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~функцию~Мёбиуса:\\\\\\mu(" + QString::number(task.getTask()) + ")=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i;
            }
        } break;
    case MebiusFunctionOptions::EvenPrimes:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minNum, maxNum));
            if (isPrime(task.getTask())) continue;
            auto data = decompositionToSimple(task.getTask());
            if (data.size() % 2 != 0) continue;
            bool accessFlag = true;
            for (size_t i = 0; i < data.size(); ++i){
                if (data[i].second > 1) {
                    accessFlag = false;
                    break;
                }
            }
            if (accessFlag) {
                if (!mode) {
                    buffer = "\\mu(" + QString::number(task.getTask()) + ")=~?";
                    tasks.push_back(std::move(buffer));
                    answers.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~функцию~Мёбиуса:\\\\\\mu(" + QString::number(task.getTask()) + ")=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i;
            }
        } break;
    case MebiusFunctionOptions::NotEvenPrimes:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minNum, maxNum));
            if (isPrime(task.getTask())) continue;
            auto data = decompositionToSimple(task.getTask());
            if (data.size() % 2 == 0) continue;
            bool accessFlag = true;
            for (size_t i = 0; i < data.size(); ++i){
                if (data[i].second > 1) {
                    accessFlag = false;
                    break;
                }
            }
            if (accessFlag) {
                if (!mode) {
                    buffer = "\\mu(" + QString::number(task.getTask()) + ")=~?";
                    tasks.push_back(std::move(buffer));
                    answers.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~функцию~Мёбиуса:\\\\\\mu(" + QString::number(task.getTask()) + ")=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i;
            }
        } break;
    case MebiusFunctionOptions::PrimeDegree:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minNum, maxNum));
            auto data = decompositionToSimple(task.getTask());
            bool accessFlag = false;
            for (size_t i = 0; i < data.size(); ++i) {
                if (data[i].second > 1) {
                    accessFlag = true;
                    break;
                }
            }
            if (accessFlag) {
                if (!mode) {
                    buffer = "\\mu(" + QString::number(task.getTask()) + ")=~?";
                    tasks.push_back(std::move(buffer));
                    answers.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~функцию~Мёбиуса:\\\\\\mu(" + QString::number(task.getTask()) + ")=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i;
            }
        } break;
    }

    if (!mode) {
        descriptions.push_back(std::move(QString("Вычислите функцию Мёбиуса")));
        this->tasks.push_back(std::move(tasks));
        isReadyRender();
    }
}

void GeneratorWindow::runSymbolLegandre(int countOfTasks, std::pair<int, int> a, std::pair<int, int> p, SymbolLegandreOptions option)
{
    SymbolLegandre task;
    QVector<QString> tasks; QString buffer;
    switch (option) {
    case SymbolLegandreOptions::Default:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second)) {
                if (!mode) {
                    buffer = "\\left(\\frac{" + QString::number(task.getTask().first) + "}{" +
                            QString::number(task.getTask().second) + "}\\right)=~?";
                    tasks.push_back(std::move(buffer));
                    answers.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~символ~Лежандра:\\\\\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i;
            }
        } break;
    case SymbolLegandreOptions::Primes:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second) && !isPrime(task.getTask().first)) {
                if (!mode) {
                    buffer = "\\left(\\frac{" + QString::number(task.getTask().first) + "}{" +
                            QString::number(task.getTask().second) + "}\\right)=~?";
                    tasks.push_back(std::move(buffer));
                    answers.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~символ~Лежандра:\\\\\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i;
            }
        } break;
    case SymbolLegandreOptions::aEqual_1:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second)) {
                if (!mode) {
                    buffer = "\\left(\\frac{" + QString::number(task.getTask().first) + "}{" +
                            QString::number(task.getTask().second) + "}\\right)=~?";
                    tasks.push_back(std::move(buffer));
                    answers.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~символ~Лежандра:\\\\\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i;
            }
        } break;
    case SymbolLegandreOptions::aEqual2:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second)) {
                if (!mode) {
                    buffer = "\\left(\\frac{" + QString::number(task.getTask().first) + "}{" +
                            QString::number(task.getTask().second) + "}\\right)=~?";
                    tasks.push_back(std::move(buffer));
                    answers.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~символ~Лежандра:\\\\\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i;
            }
        } break;
    case SymbolLegandreOptions::NotEvenPrimes:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second) && !isPrime(task.getTask().first) && task.getTask().first % 2 != 0) {
                if (!mode) {
                    buffer = "\\left(\\frac{" + QString::number(task.getTask().first) + "}{" +
                            QString::number(task.getTask().second) + "}\\right)=~?";
                    tasks.push_back(std::move(buffer));
                    answers.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~символ~Лежандра:\\\\\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i;
            }
        } break;
    }
    if (!mode) {
        descriptions.push_back(std::move(QString("Вычислите символ Лежандра")));
        this->tasks.push_back(std::move(tasks));
        isReadyRender();
    }
}

void GeneratorWindow::runSymbolJacobi(int countOfTasks, std::pair<int, int> a, std::pair<int, int> p, SymbolJacobiOptions option)
{
    SymbolJacobi task;
    QVector<QString> tasks; QString buffer;
    switch (option) {
    case SymbolJacobiOptions::Default:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (!isPrime(task.getTask().second) && task.getTask().second % 2 != 0) {
                if (!mode) {
                    buffer = "\\left(\\frac{" + QString::number(task.getTask().first) + "}{" +
                            QString::number(task.getTask().second) + "}\\right)=~?";
                    tasks.push_back(std::move(buffer));
                    answers.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~символ~Якоби:\\\\\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i;
            }
        } break;
    case SymbolJacobiOptions::Primes:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (!isPrime(task.getTask().second) && !isPrime(task.getTask().first) && task.getTask().second % 2 != 0) {
                if (!mode) {
                    buffer = "\\left(\\frac{" + QString::number(task.getTask().first) + "}{" +
                            QString::number(task.getTask().second) + "}\\right)=~?";
                    tasks.push_back(std::move(buffer));
                    answers.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~символ~Якоби:\\\\\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i;
            }
        } break;
    case SymbolJacobiOptions::aEqual_1:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (!isPrime(task.getTask().second) && task.getTask().second % 2 != 0) {
                if (!mode) {
                    buffer = "\\left(\\frac{" + QString::number(task.getTask().first) + "}{" +
                            QString::number(task.getTask().second) + "}\\right)=~?";
                    tasks.push_back(std::move(buffer));
                    answers.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~символ~Якоби:\\\\\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i;
            }
        } break;
    case SymbolJacobiOptions::aEqual2:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (!isPrime(task.getTask().second) && task.getTask().second % 2 != 0) {
                if (!mode) {
                    buffer = "\\left(\\frac{" + QString::number(task.getTask().first) + "}{" +
                            QString::number(task.getTask().second) + "}\\right)=~?";
                    tasks.push_back(std::move(buffer));
                    answers.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~символ~Якоби:\\\\\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i;
            }
        } break;
    case SymbolJacobiOptions::NotEvenPrimes:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (!isPrime(task.getTask().second) && !isPrime(task.getTask().first) && task.getTask().first % 2 != 0 && task.getTask().second % 2 != 0) {
                if (!mode) {
                    buffer = "\\left(\\frac{" + QString::number(task.getTask().first) + "}{" +
                            QString::number(task.getTask().second) + "}\\right)=~?";
                    tasks.push_back(std::move(buffer));
                    answers.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~символ~Якоби:\\\\\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i;
            }
        } break;
    }
    if (!mode) {
        descriptions.push_back(std::move(QString("Вычислите символ Якоби")));
        this->tasks.push_back(std::move(tasks));
        isReadyRender();
    }
}

void GeneratorWindow::runTranspositionGroup(int countOfTasks, int minN, int maxN, TranspositionGroupOptions option, ViewMode mode)
{
    TranspositionGroup task, task2, result;
    QVector<QString> tasks; QString buffer;
    switch (option) {
    case TranspositionGroupOptions::Write:
        if (!this->mode) {
            if (mode == ViewMode::Standart)
                descriptions.push_back(std::move(QString("Запишите подстановку в виде произведения независимых циклов")));
            else
                descriptions.push_back(std::move(QString("Запишите подстановку в табличном виде")));
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode);;
            if (!this->mode) {
                buffer = "S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(mode) + "=~?";
                tasks.push_back(std::move(buffer));
                answers.push_back(task.writeToMode(static_cast<ViewMode>((static_cast<int>(mode) + 1) % 2)));
            } else {
                if (task.getViewMode() == ViewMode::Standart) {
                    QString taskText = "\\begin{align}\\color{sienna}{Запишите~подстановку~в~виде~произведения~независимых~циклов.\\\\Неподвижные~элементы~указывать~не~требуется.\\\\S_{" +
                            QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, task.writeToMode(ViewMode::Cycle), SupCommands::Transposition, task.getTask()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Запишите~подстановку~в~табличном~виде.\\\\В~ответе~укажите~нижнюю~строку~получившейся~подстановки.\\\\S_{" +
                            QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, task.writeToMode(ViewMode::Standart, true), SupCommands::Transposition, task.getTask()));
                }
            } ++i;
        } break;

    case TranspositionGroupOptions::Multiply:
        if (!this->mode) {
            if (mode == ViewMode::Standart)
                descriptions.push_back(std::move(QString("Найдите произведение подстановок, записанных в табличном виде")));
            else
                descriptions.push_back(std::move(QString("Найдите произведение подстановок, записанных в циклическом виде")));
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            int n = static_cast<int>(random->bounded(minN, maxN));
            task.setTask(n, mode);
            task2.setTask(n, mode);
            result = task * task2;
            if (!this->mode) {
                buffer = "S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(mode) +
                        "\\cdot" + task2.writeToMode(mode) + "=~?";
                tasks.push_back(std::move(buffer));
                answers.push_back(task.writeToMode(mode));
            } else {
                if (task.getViewMode() == ViewMode::Standart) {
                    QString taskText = "\\begin{align}\\color{sienna}{Найдите~произведение~подстановок,~записанных~в~табличном~виде.\\\\В~ответе~укажите~нижнюю~строку~получившейся~подстановки.\\\\S_{" +
                            QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "·" + task2.writeToMode(ViewMode::Standart) + "=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, result.writeToMode(ViewMode::Standart, true), SupCommands::Transposition, result.getTask()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Найдите~произведение~подстановок,~записанных~в~циклическом~виде.\\\\Неподвижные~элементы~указывать~не~требуется.\\\\S_{" +
                            QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "·" + task2.writeToMode(ViewMode::Cycle) + "=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, result.writeToMode(ViewMode::Cycle), SupCommands::Transposition, result.getTask()));
                }
            } ++i;
        } break;

    case TranspositionGroupOptions::Inverse:
        if (!this->mode) {
            if (mode == ViewMode::Standart)
                descriptions.push_back(std::move(QString("Найдите подстановку, обратную данной, записанной в табличном виде")));
            else
                descriptions.push_back(std::move(QString("Найдите подстановку, обратную~данной, записанной в циклическом виде")));
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode);
            result = ~task;
            if (!this->mode) {
                buffer = "S_{" + QString::number(task.getTask()) + "}:{" + task.writeToMode(mode) + "}^{-1}=~?";
                tasks.push_back(std::move(buffer));
                answers.push_back(task.writeToMode(mode));
            } else {
                if (task.getViewMode() == ViewMode::Standart) {
                    QString taskText = "\\begin{align}\\color{sienna}{Найдите~подстановку,~обратную~данной,~записанной~в~табличном~виде.\\\\В~ответе~укажите~нижнюю~строку~получившейся~подстановки.\\\\S_{" +
                            QString::number(task.getTask()) + "}:{" + task.writeToMode(ViewMode::Standart) + "}^{-1}=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, result.writeToMode(ViewMode::Standart, true), SupCommands::Transposition, result.getTask()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Найдите~подстановку,~обратную~данной,~записанной~в~циклическом~виде.\\\\Неподвижные~элементы~указывать~не~требуется.\\\\S_{" +
                            QString::number(task.getTask()) + "}:{" + task.writeToMode(ViewMode::Cycle) + "}^{-1}=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, result.writeToMode(ViewMode::Cycle), SupCommands::Transposition, result.getTask()));
                }
            } ++i;
        } break;

    case TranspositionGroupOptions::Cycle:
        if (!this->mode) {
            if (mode == ViewMode::Standart)
                descriptions.push_back(std::move(QString("Найдите цикловой тип подстановки, записанной в табличном виде")));
            else
                descriptions.push_back(std::move(QString("Найдите цикловой тип подстановки, записанной в циклическом виде")));
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (!this->mode) {
                buffer = "S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(mode) + "=~?";
                tasks.push_back(std::move(buffer));
                answers.push_back(task.cycleType());
            } else {
                if (task.getViewMode() == ViewMode::Standart) {
                    QString taskText = "\\begin{align}\\color{sienna}{Найдите~цикловой~тип~подстановки,~записанной~в~табличном~виде.\\\\Ответ~дайте~в~формате~[1^i,2^j,...,n^k],~где~i,j,k\\neq0.\\\\S_{" +
                            QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, task.cycleType(), SupCommands::CycleType, 0));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Найдите~цикловой~тип~подстановки,~записанной~в~циклическом~виде.\\\\Ответ~дайте~в~формате~[1^i,2^j,...,n^k],~где~i,j,k\\neq0.\\\\S_{" +
                            QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, task.cycleType(), SupCommands::CycleType, 0));
                }
            } ++i;
        } break;

    case TranspositionGroupOptions::Count:
        if (!this->mode) {
            if (mode == ViewMode::Standart)
                descriptions.push_back(std::move(QString("Вычислите количество беспорядков подстановки, записанной в табличном виде")));
            else
                descriptions.push_back(std::move(QString("Вычислите количество беспорядков подстановки, записанной в циклическом виде")));
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (!this->mode) {
                buffer = "S_{" + QString::number(task.getTask()) + "}:\\Delta" + task.writeToMode(mode) + "=~?";
                tasks.push_back(std::move(buffer));
                answers.push_back(QString::number(task.getHaos()));
            } else {
                if (task.getViewMode() == ViewMode::Standart) {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~количество~беспорядков~подстановки,~записанной~в~табличном~виде.\\\\S_{" +
                            QString::number(task.getTask()) + "}:\\Delta" + task.writeToMode(ViewMode::Standart) + "=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.getHaos()), SupCommands::Number, 0));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~количество~беспорядков~подстановки,~записанной~в~циклическом~виде.\\\\S_{" +
                            QString::number(task.getTask()) + "}:\\Delta" + task.writeToMode(ViewMode::Cycle) + "=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.getHaos()), SupCommands::Number, 0));
                }
            } ++i;
        } break;

    case TranspositionGroupOptions::Mod2:
        if (!this->mode) {
            if (mode == ViewMode::Standart && this->mode)
                descriptions.push_back(std::move(QString("Определите четность подстановки, записанной в табличном виде")));
            else
                descriptions.push_back(std::move(QString("Определите четность подстановки, записанной в циклическом виде")));
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (!this->mode) {
                buffer = "S_{" + QString::number(task.getTask()) + "}:\\delta" + task.writeToMode(mode) + "\\Rightarrow~?";
                tasks.push_back(std::move(buffer));
                answers.push_back(task.getEven());
            } else {
                if (task.getViewMode() == ViewMode::Standart) {
                    QString taskText = "\\begin{align}\\color{sienna}{Определите~четность~подстановки,~записанной~в~табличном~виде.\\\\В~ответе~укажите~Ч,~если~подстановка~четная,~и~Н,~в~противном~случае.\\\\S_{" +
                            QString::number(task.getTask()) + "}:\\delta" + task.writeToMode(ViewMode::Standart) + "\\Rightarrow~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, task.getEven(true), SupCommands::Even, 0));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Определите~четность~подстановки,~записанной~в~циклическом~виде.\\\\В~ответе~укажите~Ч,~если~подстановка~четная,~и~Н,~в~противном~случае.\\\\S_{" +
                            QString::number(task.getTask()) + "}:\\delta" + task.writeToMode(ViewMode::Cycle) + "\\Rightarrow~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, task.getEven(true), SupCommands::Even, 0));
                }
            } ++i;
        } break;

    case TranspositionGroupOptions::Order:
        if (!this->mode) {
            if (mode == ViewMode::Standart)
                descriptions.push_back(std::move(QString("Определите порядок подстановки, записанной в табличном виде")));
            else
                descriptions.push_back(std::move(QString("Определите порядок подстановки, записанной в циклическом виде")));
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (!this->mode) {
                buffer = "S_{" + QString::number(task.getTask()) + "}:~ord" + task.writeToMode(mode) + "=~?";
                tasks.push_back(std::move(buffer));
                answers.push_back(QString::number(task.getOrder()));
            } else {
                if (task.getViewMode() == ViewMode::Standart) {
                    QString taskText = "\\begin{align}\\color{sienna}{Определите~порядок~подстановки,~записанной~в~табличном~виде.\\\\S_{" +
                            QString::number(task.getTask()) + "}:~ord" + task.writeToMode(ViewMode::Standart) + "=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.getOrder()), SupCommands::Number, 0));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Определите~порядок~подстановки,~записанной~в~циклическом~виде.\\\\S_{" +
                            QString::number(task.getTask()) + "}:~ord" + task.writeToMode(ViewMode::Cycle) + "=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.getOrder()), SupCommands::Number, 0));
                }
            } ++i;
        } break;

    case TranspositionGroupOptions::Decomposition:
        if (!this->mode)
            descriptions.push_back(std::move(QString("Запишите подстановку в виде произведения транспозиций")));

        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode, true);
            if (!this->mode) {
                buffer = "S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(mode) + "=~?";
                tasks.push_back(std::move(buffer));
                answers.push_back(task.writeToMode(ViewMode::Decomposition));
            } else {
                if (task.getViewMode() == ViewMode::Standart) {
                    QString taskText = "\\begin{align}\\color{sienna}{Запишите~подстановку~в~виде~произведения~транспозиций.\\\\S_{" +
                            QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, task.writeToMode(ViewMode::Decomposition), SupCommands::MultiTransposition, task.getTask()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Запишите~подстановку~в~виде~произведения~транспозиций.\\\\S_{" +
                            QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, task.writeToMode(ViewMode::Decomposition), SupCommands::MultiTransposition, task.getTask()));
                }
            } ++i;
        } break;

    case TranspositionGroupOptions::Neighbor:
        if (!this->mode)
            descriptions.push_back(std::move(QString("Запишите подстановку в виде произведения транспозиций соседних элементов")));

        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode, true);
            if (!this->mode) {
                buffer = "S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(mode) + "=~?";
                tasks.push_back(std::move(buffer));
                answers.push_back(task.writeToMode(ViewMode::Neighbors));
            } else {
                if (task.getViewMode() == ViewMode::Standart) {
                    QString taskText = "\\begin{align}\\color{sienna}{Запишите~подстановку~в~виде~произведения~транспозиций~соседних~элементов.\\\\S_{" +
                            QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, task.writeToMode(ViewMode::Neighbors), SupCommands::MultiTransposition, task.getTask()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Запишите~подстановку~в~виде~произведения~транспозиций~соседних~элементов.\\\\S_{" +
                            QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, task.writeToMode(ViewMode::Neighbors), SupCommands::MultiTransposition, task.getTask()));
                }
            } ++i;
        } break;
    }
    if (!this->mode) {
        this->tasks.push_back(std::move(tasks));
        isReadyRender();
    }
}

void GeneratorWindow::runSet(int countOfTasks, SetOptions option)
{   
    AlgebraStructures AS;
    set_type data = std::move(AS.generateTasks(countOfTasks, option));
    QVector<QString> tasks; QString buffer;
    switch (option) {
    case SetOptions::Check:
        if (!this->mode)
            descriptions.push_back(std::move(QString("Чем является данная Алгебраическая Структура")));

        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            if (!mode) {
                buffer = "\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?";
                tasks.push_back(std::move(buffer));
                answers.push_back(std::get<2>(data[i]));
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Чем~является~данная~Алгебраическая~Структура:\\\\\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, std::get<2>(data[i]), SupCommands::Name, 0));
            }
        } break;

    case SetOptions::Oper:
        if (!this->mode)
            descriptions.push_back(std::move(QString("Является ли данная операция заданной на множестве")));

        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            QString answer;
            if (QString(std::get<2>(data[i])).toInt() / 1000)
                answer = "Да";
            else answer = "Нет";
            if (!mode) {
                buffer = "\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?";
                tasks.push_back(std::move(buffer));
                answers.push_back(answer);
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Чем~является~данная~Алгебраическая~Структура:\\\\\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, answer, SupCommands::Name, 0));
            }
        } break;

    case SetOptions::Abel:
        if (!this->mode)
            descriptions.push_back(std::move(QString("Является ли данная операция коммутативной на множестве")));

        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            QString answer;
            if (QString(std::get<2>(data[i])).toInt() / 100)
                answer = "Да";
            else answer = "Нет";
            if (!mode) {
                buffer = "\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?";
                tasks.push_back(std::move(buffer));
                answers.push_back(answer);
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Чем~является~данная~Алгебраическая~Структура:\\\\\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, answer, SupCommands::Name, 0));
            }
        } break;

    case SetOptions::Associate:
        if (!this->mode)
            descriptions.push_back(std::move(QString("Является ли данная операция ассоциативной на множестве")));

        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            QString answer;
            if (QString(std::get<2>(data[i])).toInt() / 10)
                answer = "Да";
            else answer = "Нет";
            if (!mode) {
                buffer = "\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?";
                tasks.push_back(std::move(buffer));
                answers.push_back(answer);
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Чем~является~данная~Алгебраическая~Структура:\\\\\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, answer, SupCommands::Name, 0));
            }
        } break;

    case SetOptions::Neutral:
        if (!this->mode)
            descriptions.push_back(std::move(QString("Существует ли нейтральный элемент на заданной алгебраической структуре")));

        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            QString answer;
            if (QString(std::get<2>(data[i])).toInt() % 10)
                answer = "Да";
            else answer = "Нет";
            if (!mode) {
                buffer = "\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?";
                tasks.push_back(std::move(buffer));
                answers.push_back(answer);
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Чем~является~данная~Алгебраическая~Структура:\\\\\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, answer, SupCommands::Name, 0));
            }
        } break;
    }
    if (!mode) {
        this->tasks.push_back(std::move(tasks));
        isReadyRender();
    }
}

void GeneratorWindow::runGroupProperties(int countOfTasks, int minN, int maxN, GroupPropertiesOptions option)
{
    GroupProperties GP;
    QVector<QString> tasks; QString buffer;
    switch (option) {
    case GroupPropertiesOptions::Exponent:
        if (!this->mode)
            descriptions.push_back(std::move(QString("Вычислите экспоненту группы")));

        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            GP.setGroup(static_cast<Set>(random->bounded(0, GP.getGroups())), random->bounded(minN, maxN));
            if (!mode) {
                buffer = "Exp\\left(" + GP.getCode() + "\\right)" + "=~?";
                tasks.push_back(std::move(buffer));
                answers.push_back(QString::number(GP.Exp()));
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Вычислите~экспоненту~группы:\\\\Exp\\left(" + GP.getCode() + "\\right)" + "=~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, QString::number(GP.Exp()), SupCommands::Number, 0));
            }
        } break;
    }
    if (!mode) {
        this->tasks.push_back(std::move(tasks));
        isReadyRender();
    }
}

void GeneratorWindow::runMatrix(int countOfTasks, std::pair<int, int> rangeSize, std::pair<int, int> rangeK, MatrixOptions option)
{
    Matrix<int> matrix, subMat;
    QVector<QString> tasks; QString buffer;
    switch (option) {
    case MatrixOptions::Sum:
        if (!this->mode)
            descriptions.push_back(std::move(QString("Вычислите сумму матриц")));

        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            size_t rows = static_cast<size_t>(random->bounded(rangeSize.first, rangeSize.second));
            size_t cols = static_cast<size_t>(random->bounded(rangeSize.first, rangeSize.second));
            matrix.setTask(rows, cols, rangeK.first, rangeK.second);
            subMat.setTask(rows, cols, rangeK.first, rangeK.second);
            if (!mode) {
                buffer = matrix.getMatrix() + "+" + subMat.getMatrix() + "=~?";
                tasks.push_back(std::move(buffer));
                answers.push_back((matrix + subMat).getMatrix());
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Вычислите~сумму~матриц:\\\\" + matrix.getMatrix() + "+" + subMat.getMatrix() + "=~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, (matrix + subMat).getMatrix(), SupCommands::Name, 0));
            }
        } break;
    case MatrixOptions::Diff:
        if (!this->mode)
            descriptions.push_back(std::move(QString("Найдите разность матриц")));

        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            size_t rows = static_cast<size_t>(random->bounded(rangeSize.first, rangeSize.second));
            size_t cols = static_cast<size_t>(random->bounded(rangeSize.first, rangeSize.second));
            matrix.setTask(rows, cols, rangeK.first, rangeK.second);
            subMat.setTask(rows, cols, rangeK.first, rangeK.second);
            if (!mode) {
                buffer = matrix.getMatrix() + "-" + subMat.getMatrix() + "=~?";
                tasks.push_back(std::move(buffer));
                answers.push_back((matrix - subMat).getMatrix());
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Найдите~разность~матриц:\\\\" + matrix.getMatrix() + "-" + subMat.getMatrix() + "=~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, (matrix - subMat).getMatrix(), SupCommands::Name, 0));
            }
        } break;
    case MatrixOptions::Multy:
        if (!this->mode)
            descriptions.push_back(std::move(QString("Найдите произведение матриц")));

        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            size_t rows = static_cast<size_t>(random->bounded(rangeSize.first, rangeSize.second));
            size_t cols = static_cast<size_t>(random->bounded(rangeSize.first, rangeSize.second));
            matrix.setTask(rows, cols, rangeK.first, rangeK.second);
            subMat.setTask(cols, rows, rangeK.first, rangeK.second);
            if (!mode) {
                buffer = matrix.getMatrix() + "\\cdot" + subMat.getMatrix() + "=~?";
                tasks.push_back(std::move(buffer));
                answers.push_back((matrix * subMat).getMatrix());
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Найдите~произведение~матриц:\\\\" + matrix.getMatrix() + "\\cdot" + subMat.getMatrix() + "=~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, (matrix * subMat).getMatrix(), SupCommands::Name, 0));
            }
        } break;
    case MatrixOptions::Inverse:
        if (!this->mode)
            descriptions.push_back(std::move(QString("Найдите матрицу, обратную данной")));

        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            size_t rows = static_cast<size_t>(random->bounded(rangeSize.first, rangeSize.second));
            Matrix<double> matrix; matrix.setTask(rows, rows, rangeK.first, rangeK.second);
            if (!mode) {
                buffer = "{" + matrix.getMatrix() + "}^{-1}=~?";
                tasks.push_back(std::move(buffer));
                answers.push_back("\\frac{1}{" + QString::number(matrix.det()) + "}" + (~matrix).getMatrix());
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Найдите~матрицу,~обратную~данной:\\\\{" + matrix.getMatrix() + "}^{-1}=~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, "\\frac{1}{" + QString::number(matrix.det()) + "}" + (~matrix).getMatrix(), SupCommands::Name, 0));
            }
        } break;
    case MatrixOptions::Det:
        if (!this->mode)
            descriptions.push_back(std::move(QString("Вычислить детерминант матрицы")));

        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            size_t rows = static_cast<size_t>(random->bounded(rangeSize.first, rangeSize.second));
            matrix.setTask(rows, rows, rangeK.first, rangeK.second);
            if (!mode) {
                buffer = matrix.getMatrix().replace("pmatrix", "vmatrix") + "=~?";
                tasks.push_back(std::move(buffer));
                answers.push_back(QString::number(matrix.det()));
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Вычислить~детерминант~матрицы:\\\\" + matrix.getMatrix().replace("pmatrix", "vmatrix") + "=~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, QString::number(matrix.det()), SupCommands::Number, 0));
            }
        } break;
    }
    if (!mode) {
        this->tasks.push_back(std::move(tasks));
        isReadyRender();
    }
}

void GeneratorWindow::runRingResidue(int countOfTasks, int minNum, int maxNum, RingResidueOptions option)
{
    RingResidue task;
    QVector<QString> tasks; QString buffer;
    switch (option) {
    case RingResidueOptions::GenCount:
        if (!this->mode)
            descriptions.push_back(std::move(QString("Вычислите количество образующих элементов группы Вычетов")));

        for (std::size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            task.setOrder(random->bounded(minNum, maxNum));
            task.setType(RingResidueType::Summary);
            if (!mode) {
                buffer = task.getCode() + "=~?";
                tasks.push_back(std::move(buffer));
                answers.push_back(QString::number(task.countOfGenerators()));
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Вычислите~количество~образующих~элементов~группы~Вычетов:\\\\\\left(\\mathbb{Z}_{" + QString::number(task.getOrder()) + "}, +\\right)=~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.countOfGenerators()), SupCommands::Number, 0));
            }
        } break;
    case RingResidueOptions::A_in_M:
        if (!this->mode)
            descriptions.push_back(std::move(QString("Найдите значение выражения")));

        for (std::size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            task.setOrder(random->bounded(minNum, maxNum));
            task.setType(static_cast<RingResidueType>(random->bounded(0, 2)));
            int a = static_cast<int>(random->bounded(1, task.getOrder()));
            int m = static_cast<int>(random->bounded(2, 20));
            if (!mode) {
                buffer = task.getCode() + ":~" + QString::number(a) + "^{" + QString::number(m) + "}=~?";
                tasks.push_back(std::move(buffer));
                answers.push_back(QString::number(task.solve(a, m)));
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Найдите~значение~выражения:\\\\" + task.getCode() + ":~" +
                        QString::number(a) + "^{" + QString::number(m) + "}=~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve(a, m)), SupCommands::Number, 0));
            }
        } break;
    case RingResidueOptions::Order:
        if (!this->mode)
            descriptions.push_back(std::move(QString("Вычислите порядок элемента")));

        for (std::size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            task.setOrder(random->bounded(minNum, maxNum));
            task.setType(static_cast<RingResidueType>(random->bounded(0, 2)));
            int element = static_cast<int>(random->bounded(1, task.getOrder()));
            if (!mode) {
                buffer = task.getCode() + ":~ord(" + QString::number(element) + ")=~?";
                tasks.push_back(std::move(buffer));
                answers.push_back(QString::number(task.getOrd(element)));
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{\\textbf{Вычислите порядок элемента:}\\\\" +
                        task.getCode() + ":~" +"ord(" + QString::number(element) + ")=~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.getOrd(element)), SupCommands::Number, 0));
            }
        } break;
    case RingResidueOptions::A_X_equal_B:
        if (!this->mode)
            descriptions.push_back(std::move(QString("Решите линейное сравнение")));

        for (std::size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            task.setOrder(random->bounded(minNum, maxNum));
            int a, b;
            do {
                task.setOrder(random->bounded(minNum, maxNum));
                a = static_cast<int>(random->bounded(1, task.getOrder()));
                b = static_cast<int>(random->bounded(1, task.getOrder()));
            } while(task.solveLinear(a, b, task.getOrder()) == "Нет корней");
            if (!mode) {
                buffer = task.getCode() + ":~" + QString::number(a) + "\\cdot x\\equiv" + QString::number(b) + ",~x=~?";
                tasks.push_back(std::move(buffer));
                answers.push_back("\\{" + task.solveLinear(a, b, task.getOrder()) + "\\}");
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{\\textbf{Решите линейное сравнение:}\\\\" +
                        task.getCode() + ":~" + QString::number(a) + "\\cdot x\\equiv" + QString::number(b) + ",~x=~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, "\\{" + task.solveLinear(a, b, task.getOrder()) + "\\}", SupCommands::Name, 0));
            }
        } break;
    case RingResidueOptions::XX_equal_p:
        if (!this->mode)
            descriptions.push_back(std::move(QString("Решите квадратичное сравнение по простому модулю")));

        for (std::size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            task.setOrder(random->bounded(minNum, maxNum));
            int a;
            do {
                task.setOrder(random->bounded(minNum, maxNum));
                a = static_cast<int>(random->bounded(1, task.getOrder()));
            } while(task.solveSimpleQuadro(a, task.getOrder()) == "Нет корней");
            if (!mode) {
                buffer = task.getCode() + ":~x^2\\equiv" + QString::number(a) + ",~x=~?";
                tasks.push_back(std::move(buffer));
                answers.push_back("\\{" + task.solveSimpleQuadro(a, task.getOrder()) + "\\}");
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{\\textbf{Решите квадратичное сравнение по простому модулю:}\\\\" +
                        task.getCode() + ":~" + "x^2\\equiv" + QString::number(a) + ",~x=~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, "\\{" + task.solveSimpleQuadro(a, task.getOrder()) + "\\}", SupCommands::Name, 0));
            }
        } break;
    }
    if (!mode) {
        this->tasks.push_back(std::move(tasks));
        isReadyRender();
    }
}

void GeneratorWindow::runRingOfMembers(int countOfTasks, std::pair<int, int> rangeDeg, std::pair<int, int> rangeK, RingOfMembersOptions option, Set ring)
{
    RingOfMembers task(option), task_2(option);
    QVector<QString> tasks;

    QVector<QVariant> options;
    options.push_back(QVariant::fromValue(ring));
    options.push_back(QVariant::fromValue(qMakePair(rangeDeg.first, rangeDeg.second)));
    options.push_back(QVariant::fromValue(qMakePair(rangeK.first, rangeK.second)));

    for (int i = 0; i < countOfTasks; ++i) {
        task.create(options);

        QVector<QVariant> options_2 = options;
        options_2[1] = QVariant::fromValue(qMakePair(rangeDeg.first, task.getDeg() + 1));
        task_2.create(options_2);
        if (!mode) {
            tasks.push_back(task.task(task_2.getMembers()));
            answers.push_back(task.answer(QVariant::fromValue(task_2)));
        } else { /* тест */}
        task.clear(); task_2.clear();
    }
    if (!mode) {
        descriptions.push_back(std::move(task.description()));
        this->tasks.push_back(std::move(tasks));
        isReadyRender();
    }
}
