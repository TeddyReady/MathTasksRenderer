#include "generatorwindow.h"
#include "ui_generatorwindow.h"

GeneratorWindow::GeneratorWindow(QWidget *parent)
    : QMainWindow(parent), totalTestTasks(0), totalTaskCount(0),
         countOfGeneratedAnswers(0), TFWpastSize(0), mode(false),
      random(QRandomGenerator::global()), ui(new Ui::GeneratorWindow)
{
    uploadSettings();
    uploadUI();

    connect(ui->toolBar->actions().at(0), SIGNAL(triggered()), this, SLOT(checkAnswers()));
    connect(ui->toolBar->actions().at(1), SIGNAL(triggered()), this, SLOT(clearTasks()));
    connect(ui->toolBar->actions().at(3), SIGNAL(triggered()), this, SLOT(printTasks()));
    connect(ui->toolBar->actions().at(5), SIGNAL(triggered()), this, SLOT(openManual()));
    connect(ui->toolBar->actions().at(6), &QAction::triggered, [](){qApp->exit();});
}

GeneratorWindow::~GeneratorWindow()
{
    saveSettings();
    delete engine;
    delete ui;
}

void GeneratorWindow::uploadUI()
{
    ui->setupUi(this);
    ui->genButton->setEnabled(true);
    ui->genButton->setCursor(Qt::PointingHandCursor);
    ui->pushButton->setCursor(Qt::PointingHandCursor);
    ui->webView->setSource(QUrl("qrc:/general/web/theory.html"));
    ui->webView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tabWidget->tabBar()->setCursor(Qt::PointingHandCursor);
    ui->taskView->setCursor(Qt::BlankCursor);
    ui->taskView->setContextMenuPolicy(Qt::CustomContextMenu);
    engine = new TeXEngine(ui->taskView);

    ui->toolBar->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->toolBar->addAction(new QAction(QPixmap("://general/img/checkAnswers.png"), "Показать ответы", ui->toolBar));
    ui->toolBar->addAction(new QAction(QPixmap("://general/img/clearTasks.png"), "Очистить задачи", ui->toolBar));
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(new QAction(QPixmap("://general/img/printer.png"), "Подготовить печатный вариант...", ui->toolBar));
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(new QAction(QPixmap("://general/img/manual.png"), "Справочник", ui->toolBar));
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
}

void GeneratorWindow::saveSettings()
{
    settings->setValue("windowSize", geometry());
}

void GeneratorWindow::uploadSettings()
{
    settings = new QSettings("../ProjectTasksGenerator/general/settings.ini",
                             QSettings::IniFormat, this);

    setGeometry(settings->value("windowSize").toRect());
}

void GeneratorWindow::isReadyRender(){
    QApplication::setOverrideCursor(Qt::WaitCursor);
    taskBuffer += tasksForWork.right(tasksForWork.size() - TFWpastSize);
    TFWpastSize = tasksForWork.size();
    engine->TeX2SVG("\\begin{aligned}\\Large{\\color{sienna}{" + taskBuffer + "}}\\end{aligned}");
    totalTaskCount = 0;
    QApplication::restoreOverrideCursor();
}

void GeneratorWindow::checkAnswers(){
    QApplication::setOverrideCursor(Qt::WaitCursor);
    for (int i = 0; i < taskBuffer.size(); ++i)
    {
        if (taskBuffer[i] == "?")
        {
            taskBuffer.remove(i, 1);
            taskBuffer.insert(i - 1, solvedWorkTasks[countOfGeneratedAnswers++]);
        }
    }
    engine->TeX2SVG("\\begin{aligned}\\Large{\\color{sienna}{" + taskBuffer.replace("@", "").replace("#", "") + "}}\\end{aligned}", true);
    ui->toolBar->actions().at(0)->setDisabled(true);
    QApplication::restoreOverrideCursor();
}

void GeneratorWindow::clearTasks()
{
    tasksForWork.clear();
    solvedWorkTasks.clear();
    taskBuffer.clear();
    TFWpastSize = 0;
    countOfGeneratedAnswers = 0;
    engine->TeX2SVG("\\begin{aligned}\\Large{\\color{sienna}{\\Large{\\textbf{В ожидании генерации задач...}}}}\\end{aligned}", true);
    ui->toolBar->actions().at(0)->setDisabled(true);
    ui->toolBar->actions().at(1)->setDisabled(true);
    ui->toolBar->actions().at(3)->setDisabled(true);
}

void GeneratorWindow::printTasks()
{
    new DialogLatexPrinter(tasksForWork.mid(35, tasksForWork.size()), solvedWorkTasks, this);
}

void GeneratorWindow::openManual()
{
    new DialogManual(this);
}

void GeneratorWindow::on_tabWidget_currentChanged(int index)
{
    switch (index) {
    case 2:
        mode = 1;
        ui->toolBar->actions().at(0)->setVisible(false);
        ui->toolBar->actions().at(1)->setVisible(false);
        ui->toolBar->actions().at(2)->setVisible(false);
        ui->toolBar->actions().at(3)->setVisible(false);
        ui->toolBar->actions().at(4)->setVisible(false);
        break;
    default:
        mode = 0;
        if (index == 0) {
            ui->toolBar->actions().at(0)->setVisible(true);
            ui->toolBar->actions().at(1)->setVisible(true);
            ui->toolBar->actions().at(2)->setVisible(true);
            ui->toolBar->actions().at(3)->setVisible(true);
            ui->toolBar->actions().at(4)->setVisible(true);
        }
        else {
            ui->toolBar->actions().at(0)->setVisible(false);
            ui->toolBar->actions().at(1)->setVisible(false);
            ui->toolBar->actions().at(2)->setVisible(false);
            ui->toolBar->actions().at(3)->setVisible(false);
            ui->toolBar->actions().at(4)->setVisible(false);
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

    connect(window, &DialogBase::sendingMetaInfo, this, &GeneratorWindow::receivedMetaInfo);
    connect(window, &DialogBase::sendingData, this, &GeneratorWindow::receivedData);

    if (!closeMode)
        ui->mainLayout->addWidget(window);
}

void GeneratorWindow::receivedMetaInfo(int countOfTasks, bool isRepeatable, QString taskText)
{
    if (mode) {
        totalTestTasks += countOfTasks;
        statusBar()->showMessage("Задания сгенерированы!", 1500);
        ui->lcdNumber->display(totalTestTasks);
    } else if (isRepeatable) {
        totalTaskCount = countOfTasks;
        tasksForWork += "\\textbf{" + taskText + "}\\\\";
        ui->toolBar->actions().at(0)->setEnabled(true);
        ui->toolBar->actions().at(1)->setEnabled(true);
        ui->toolBar->actions().at(3)->setEnabled(true);
    }
}

void GeneratorWindow::receivedData(std::vector<int> data, AllTasks task, int subTask, ViewMode optional)
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
        runSet(data[0], static_cast<SetOptions>(subTask));
        break;
    case AllTasks::Matrix:
        runMatrix(data[0], std::make_pair(data[1], data[2]),
                std::make_pair(data[3], data[4]), static_cast<MatrixOptions>(subTask));
        break;
    case AllTasks::RingResidue:
        runRingResidue(data[0], data[1], data[2], static_cast<RingResidueOptions>(subTask));
    }
}

void GeneratorWindow::runEulerFunction(int countOfTasks, int minNum, int maxNum, EulerFunctionOptions option)
{
    int curTaskCount = 1;
    EulerFunction task;
    switch (option) {
    case EulerFunctionOptions::Default:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            task.setTask(random->bounded(minNum, maxNum));
            if (!mode) {
                tasksForWork += "  " + QString::number(curTaskCount)  + ")~\\phi(" + QString::number(task.getTask()) + ")=~?\\\\";
                solvedWorkTasks.emplace_back(QString::number(task.solve()));
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Вычислите~функцию~Эйлера:\\\\\\phi(" + QString::number(task.getTask()) + ")=~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
            } ++curTaskCount;
        } break;
    case EulerFunctionOptions::Primes:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minNum, maxNum));
            if (isPrime(task.getTask())) {
                if (!mode) {
                    tasksForWork += "  " + QString::number(curTaskCount)  + ")~\\phi(" + QString::number(task.getTask()) + ")=~?\\\\";
                    solvedWorkTasks.emplace_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~функцию~Эйлера:\\\\\\phi(" + QString::number(task.getTask()) + ")=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i; ++curTaskCount;
            }
        } break;
    case EulerFunctionOptions::PrimesDegrees:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minNum, maxNum));
            auto data = decompositionToSimple(task.getTask());
            if (data.size() == 1 && data[0].second > 1) {
                if (!mode) {
                    tasksForWork += "  " + QString::number(curTaskCount)  + ")~\\phi(" + QString::number(task.getTask()) + ")=~?\\\\";
                    solvedWorkTasks.emplace_back(QString::number(task.solve()));                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~функцию~Эйлера:\\\\\\phi(" + QString::number(task.getTask()) + ")=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i; ++curTaskCount;
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
                    tasksForWork += "  " + QString::number(curTaskCount)  + ")~\\phi(" + QString::number(task.getTask()) + ")=~?\\\\";
                    solvedWorkTasks.emplace_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~функцию~Эйлера:\\\\\\phi(" + QString::number(task.getTask()) + ")=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i; ++curTaskCount;
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
                    tasksForWork += "  " + QString::number(curTaskCount)  + ")~\\phi(" + QString::number(task.getTask()) + ")=~?\\\\";
                    solvedWorkTasks.emplace_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~функцию~Эйлера:\\\\\\phi(" + QString::number(task.getTask()) + ")=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i; ++curTaskCount;
            }
        } break;
    }
    if (!mode) isReadyRender();
}

void GeneratorWindow::runMebiusFunction(int countOfTasks, int minNum, int maxNum, MebiusFunctionOptions option)
{
    int curTaskCount = 1;
    MebiusFunction task;
    switch (option) {
    case MebiusFunctionOptions::Default:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            task.setTask(random->bounded(minNum, maxNum));
            if (!mode) {
                tasksForWork += "  " + QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=~?\\\\";
                solvedWorkTasks.emplace_back(QString::number(task.solve()));
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Вычислите~функцию~Мёбиуса:\\\\\\mu(" + QString::number(task.getTask()) + ")=~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
            } ++curTaskCount;
        } break;
    case MebiusFunctionOptions::Primes:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minNum, maxNum));
            if (!isPrime(task.getTask())) {
                if (!mode) {
                    tasksForWork += "  " + QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=~?\\\\";
                    solvedWorkTasks.emplace_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~функцию~Мёбиуса:\\\\\\mu(" + QString::number(task.getTask()) + ")=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i; ++curTaskCount;
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
                    tasksForWork += "  " + QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=~?\\\\";
                    solvedWorkTasks.emplace_back(QString::number(task.solve()));                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~функцию~Мёбиуса:\\\\\\mu(" + QString::number(task.getTask()) + ")=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i; ++curTaskCount;
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
                    tasksForWork += "  " + QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=~?\\\\";
                    solvedWorkTasks.emplace_back(QString::number(task.solve()));                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~функцию~Мёбиуса:\\\\\\mu(" + QString::number(task.getTask()) + ")=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i; ++curTaskCount;
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
                    tasksForWork += "  " + QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=~?\\\\";
                    solvedWorkTasks.emplace_back(QString::number(task.solve()));                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~функцию~Мёбиуса:\\\\\\mu(" + QString::number(task.getTask()) + ")=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i; ++curTaskCount;
            }
        } break;
    }
    if (!mode) isReadyRender();
}

void GeneratorWindow::runSymbolLegandre(int countOfTasks, std::pair<int, int> a, std::pair<int, int> p, SymbolLegandreOptions option)
{
    int curTaskCount = 1;
    SymbolLegandre task;
    switch (option) {
    case SymbolLegandreOptions::Default:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second)) {
                if (!mode) {
                    tasksForWork += "  " + QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                            "}\\right)=~?\\\\";
                    solvedWorkTasks.emplace_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~символ~Лежандра:\\\\\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i; ++curTaskCount;
            }
        } break;
    case SymbolLegandreOptions::Primes:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second) && !isPrime(task.getTask().first)) {
                if (!mode) {
                    tasksForWork += "  " + QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                            "}\\right)=~?\\\\";
                    solvedWorkTasks.emplace_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~символ~Лежандра:\\\\\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i; ++curTaskCount;
            }
        } break;
    case SymbolLegandreOptions::aEqual_1:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second)) {
                if (!mode) {
                    tasksForWork += "  " + QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                            "}\\right)=~?\\\\";
                    solvedWorkTasks.emplace_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~символ~Лежандра:\\\\\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i; ++curTaskCount;
            }
        } break;
    case SymbolLegandreOptions::aEqual2:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second)) {
                if (!mode) {
                    tasksForWork += "  " + QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                            "}\\right)=~?\\\\";
                    solvedWorkTasks.emplace_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~символ~Лежандра:\\\\\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i; ++curTaskCount;
            }
        } break;
    case SymbolLegandreOptions::NotEvenPrimes:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second) && !isPrime(task.getTask().first) && task.getTask().first % 2 != 0) {
                if (!mode) {
                    tasksForWork += "  " + QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                            "}\\right)=~?\\\\";
                    solvedWorkTasks.emplace_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~символ~Лежандра:\\\\\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i; ++curTaskCount;
            }
        } break;
    }
    if (!mode) isReadyRender();
}

void GeneratorWindow::runSymbolJacobi(int countOfTasks, std::pair<int, int> a, std::pair<int, int> p, SymbolJacobiOptions option)
{
    int curTaskCount = 1;
    SymbolJacobi task;
    switch (option) {
    case SymbolJacobiOptions::Default:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (!isPrime(task.getTask().second) && task.getTask().second % 2 != 0) {
                if (!mode) {
                    tasksForWork += "  " + QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                            "}\\right)=~?\\\\";
                    solvedWorkTasks.emplace_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~символ~Якоби:\\\\\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i; ++curTaskCount;
            }
        } break;
    case SymbolJacobiOptions::Primes:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (!isPrime(task.getTask().second) && !isPrime(task.getTask().first) && task.getTask().second % 2 != 0) {
                if (!mode) {
                    tasksForWork += "  " + QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                            "}\\right)=~?\\\\";
                    solvedWorkTasks.emplace_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~символ~Якоби:\\\\\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i; ++curTaskCount;
            }
        } break;
    case SymbolJacobiOptions::aEqual_1:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (!isPrime(task.getTask().second) && task.getTask().second % 2 != 0) {
                if (!mode) {
                    tasksForWork += "  " + QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                            "}\\right)=~?\\\\";
                    solvedWorkTasks.emplace_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~символ~Якоби:\\\\\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i; ++curTaskCount;
            }
        } break;
    case SymbolJacobiOptions::aEqual2:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (!isPrime(task.getTask().second) && task.getTask().second % 2 != 0) {
                if (!mode) {
                    tasksForWork += "  " + QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                            "}\\right)=~?\\\\";
                    solvedWorkTasks.emplace_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~символ~Якоби:\\\\\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i; ++curTaskCount;
            }
        } break;
    case SymbolJacobiOptions::NotEvenPrimes:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (!isPrime(task.getTask().second) && !isPrime(task.getTask().first) && task.getTask().first % 2 != 0 && task.getTask().second % 2 != 0) {
                if (!mode) {
                    tasksForWork += "  " + QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                            "}\\right)=~?\\\\";
                    solvedWorkTasks.emplace_back(QString::number(task.solve()));
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~символ~Якоби:\\\\\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i; ++curTaskCount;
            }
        } break;
    }
    if (!mode) isReadyRender();
}

void GeneratorWindow::runTranspositionGroup(int countOfTasks, int minN, int maxN, TranspositionGroupOptions option, ViewMode mode)
{
    int localCount = 1;
    totalTaskCount = countOfTasks;
    ui->toolBar->actions().at(0)->setEnabled(true);
    ui->toolBar->actions().at(1)->setEnabled(true);
    ui->toolBar->actions().at(3)->setEnabled(true);
    TranspositionGroup task, task2, result;
    switch (option) {
    case TranspositionGroupOptions::Write:
        if (!this->mode) {
            if (mode == ViewMode::Standart)
                tasksForWork += "\\textbf{Запишите подстановку в виде произведения независимых циклов:}\\\\";
            else
                tasksForWork += "\\textbf{Запишите подстановку в табличном виде:}\\\\";
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=~?\\\\";
                    solvedWorkTasks.emplace_back(task.writeToMode(ViewMode::Cycle));
                } else {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "=~?\\\\";
                    solvedWorkTasks.emplace_back(task.writeToMode(ViewMode::Standart));
                } ++localCount;
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
                tasksForWork += "\\textbf{Найдите произведение подстановок, записанных в табличном виде:}\\\\";
            else
                tasksForWork += "\\textbf{Найдите произведение подстановок, записанных в циклическом виде:}\\\\";
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            int n = static_cast<int>(random->bounded(minN, maxN));
            task.setTask(n, mode);
            task2.setTask(n, mode);
            result = task * task2;
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" +
                            task.writeToMode(ViewMode::Standart) + "\\cdot" + task2.writeToMode(ViewMode::Standart) + "=~?\\\\";
                    solvedWorkTasks.emplace_back(result.writeToMode(ViewMode::Standart));
                } else {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" +
                            task.writeToMode(ViewMode::Cycle) + "\\cdot" + task2.writeToMode(ViewMode::Cycle) + "=~?\\\\";
                    solvedWorkTasks.emplace_back(result.writeToMode(ViewMode::Cycle));
                } ++localCount;
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
                tasksForWork += "\\textbf{Найдите подстановку, обратную данной, записанной в табличном виде:}\\\\";
            else
                tasksForWork += "\\textbf{Найдите подстановку, обратную~данной, записанной в циклическом виде:}\\\\";
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode);
            result = ~task;
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:{" + task.writeToMode(ViewMode::Standart) + "}^{-1}=~?\\\\";
                    solvedWorkTasks.emplace_back(result.writeToMode(ViewMode::Standart));
                } else {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:{" + task.writeToMode(ViewMode::Cycle) + "}^{-1}=~?\\\\";
                    solvedWorkTasks.emplace_back(result.writeToMode(ViewMode::Cycle));
                } ++localCount;
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
                tasksForWork += "\\textbf{Найдите цикловой тип подстановки, записанной в табличном виде:}\\\\";
            else
                tasksForWork += "\\textbf{Найдите цикловой тип подстановки, записанной в циклическом виде:}\\\\";
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=~?\\\\";
                    solvedWorkTasks.emplace_back(task.cycleType());
                } else {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "=~?\\\\";
                    solvedWorkTasks.emplace_back(task.cycleType());
                } ++localCount;
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
                tasksForWork += "\\textbf{Вычислите количество беспорядков подстановки, записанной в табличном виде:}\\\\";
            else
                tasksForWork += "\\textbf{Вычислите количество беспорядков подстановки, записанной в циклическом виде:}\\\\";
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\Delta" + task.writeToMode(ViewMode::Standart) + "=~?\\\\";
                    solvedWorkTasks.emplace_back(QString::number(task.getHaos()));
                } else {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\Delta" + task.writeToMode(ViewMode::Cycle) + "=~?\\\\";
                    solvedWorkTasks.emplace_back(QString::number(task.getHaos()));
                } ++localCount;
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
                tasksForWork += "\\textbf{Определите четность подстановки, записанной в табличном виде:}\\\\";
            else
                tasksForWork += "\\textbf{Определите четность подстановки, записанной в циклическом виде:}\\\\";
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\delta" + task.writeToMode(ViewMode::Standart) + "\\Rightarrow~?\\\\";
                    solvedWorkTasks.emplace_back(task.getEven());
                } else {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\delta" + task.writeToMode(ViewMode::Cycle) + "\\Rightarrow~?\\\\";
                    solvedWorkTasks.emplace_back(task.getEven());
                } ++localCount;
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
                tasksForWork += "\\textbf{Определите порядок подстановки, записанной в табличном виде:}\\\\";
            else
                tasksForWork += "\\textbf{Определите порядок подстановки, записанной в циклическом виде:}\\\\";
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:~ord" + task.writeToMode(ViewMode::Standart) + "=~?\\\\";
                    solvedWorkTasks.emplace_back(QString::number(task.getOrder()));
                } else {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:~ord" + task.writeToMode(ViewMode::Cycle) + "=~?\\\\";
                    solvedWorkTasks.emplace_back(QString::number(task.getOrder()));
                } ++localCount;
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
            tasksForWork += "\\textbf{Запишите подстановку в виде произведения транспозиций:}\\\\";

        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode, true);
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=~?\\\\";
                    solvedWorkTasks.emplace_back(task.writeToMode(ViewMode::Decomposition));
                } else {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "=~?\\\\";
                    solvedWorkTasks.emplace_back(task.writeToMode(ViewMode::Decomposition));
                } ++localCount;
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
            tasksForWork += "\\textbf{Запишите подстановку в виде произведения транспозиций соседних элементов:}\\\\";

        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode, true);
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=~?\\\\";
                    solvedWorkTasks.emplace_back(task.writeToMode(ViewMode::Neighbors));
                } else {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "=~?\\\\";
                    solvedWorkTasks.emplace_back(task.writeToMode(ViewMode::Neighbors));
                } ++localCount;
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
    if (!this->mode) isReadyRender();
}

void GeneratorWindow::runSet(int countOfTasks, SetOptions option)
{   
    int localCount = 1;
    totalTaskCount = countOfTasks;
    ui->toolBar->actions().at(0)->setEnabled(true);
    ui->toolBar->actions().at(1)->setEnabled(true);
    ui->toolBar->actions().at(3)->setEnabled(true);
    AlgebraStructures AS;
    set_type data = std::move(AS.generateTasks(countOfTasks, option));
    switch (option) {
    case SetOptions::Check:
        if (!this->mode)
            tasksForWork += "\\textbf{Чем является данная Алгебраическая Структура:}\\\\";

        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            if (!mode) {
                tasksForWork += "  " + QString::number(localCount)  + ")~\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?\\\\";
                solvedWorkTasks.emplace_back(std::get<2>(data[i]));
                ++localCount;
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Чем~является~данная~Алгебраическая~Структура:\\\\\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, std::get<2>(data[i]), SupCommands::Name, 0));
            }
        } break;

    case SetOptions::Oper:
        if (!this->mode)
            tasksForWork += "\\textbf{Является ли данная операция заданной на множестве:}\\\\";

        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            QString answer;
            if (QString(std::get<2>(data[i])).toInt() / 1000)
                answer = "Да";
            else answer = "Нет";
            if (!mode) {
                tasksForWork += "  " + QString::number(localCount)  + ")~\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?\\\\";
                solvedWorkTasks.emplace_back(answer);
                ++localCount;
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Чем~является~данная~Алгебраическая~Структура:\\\\\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, answer, SupCommands::Name, 0));
            }
        } break;

    case SetOptions::Abel:
        if (!this->mode)
            tasksForWork += "\\textbf{Является ли данная операция коммутативной на множестве:}\\\\";

        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            QString answer;
            if (QString(std::get<2>(data[i])).toInt() / 100)
                answer = "Да";
            else answer = "Нет";
            if (!mode) {
                tasksForWork += "  " + QString::number(localCount)  + ")~\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?\\\\";
                solvedWorkTasks.emplace_back(answer);
                ++localCount;
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Чем~является~данная~Алгебраическая~Структура:\\\\\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, answer, SupCommands::Name, 0));
            }
        } break;

    case SetOptions::Associate:
        if (!this->mode)
            tasksForWork += "\\textbf{Является ли данная операция ассоциативной на множестве:}\\\\";

        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            QString answer;
            if (QString(std::get<2>(data[i])).toInt() / 10)
                answer = "Да";
            else answer = "Нет";
            if (!mode) {
                tasksForWork += "  " + QString::number(localCount)  + ")~\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?\\\\";
                solvedWorkTasks.emplace_back(answer);
                ++localCount;
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Чем~является~данная~Алгебраическая~Структура:\\\\\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, answer, SupCommands::Name, 0));
            }
        } break;

    case SetOptions::Neutral:
        if (!this->mode)
            tasksForWork += "\\textbf{Существует ли нейтральный элемент на заданной алгебраической структуре:}\\\\";

        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            QString answer;
            if (QString(std::get<2>(data[i])).toInt() % 10)
                answer = "Да";
            else answer = "Нет";
            if (!mode) {
                tasksForWork += "  " + QString::number(localCount)  + ")~\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?\\\\";
                solvedWorkTasks.emplace_back(answer);
                ++localCount;
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Чем~является~данная~Алгебраическая~Структура:\\\\\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, answer, SupCommands::Name, 0));
            }
        } break;
    }
    if (!mode) isReadyRender();
}

void GeneratorWindow::runGroupProperties(int countOfTasks, GroupPropertiesOptions option)
{}

void GeneratorWindow::runMatrix(int countOfTasks, std::pair<int, int> rangeSize, std::pair<int, int> rangeK, MatrixOptions option)
{
    int localCount = 1;
    totalTaskCount = countOfTasks;
    ui->toolBar->actions().at(0)->setEnabled(true);
    ui->toolBar->actions().at(1)->setEnabled(true);
    ui->toolBar->actions().at(3)->setEnabled(true);
    Matrix<int> matrix, subMat;
    switch (option) {
    case MatrixOptions::Sum:
        if (!this->mode)
            tasksForWork += "\\textbf{Вычислите сумму матриц:}\\\\";

        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            size_t rows = static_cast<size_t>(random->bounded(rangeSize.first, rangeSize.second));
            size_t cols = static_cast<size_t>(random->bounded(rangeSize.first, rangeSize.second));
            matrix.setTask(rows, cols, rangeK.first, rangeK.second);
            subMat.setTask(rows, cols, rangeK.first, rangeK.second);
            if (!mode) {
                tasksForWork += "  " + QString::number(localCount)  + ")~" + matrix.getMatrix() + "+" + subMat.getMatrix() + "=~?\\\\";
                solvedWorkTasks.emplace_back((matrix + subMat).getMatrix());
                ++localCount;
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Вычислите~сумму~матриц:\\\\" + matrix.getMatrix() + "+" + subMat.getMatrix() + "=~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, (matrix + subMat).getMatrix(), SupCommands::Name, 0));
            }
        } break;
    case MatrixOptions::Diff:
        if (!this->mode)
            tasksForWork += "\\textbf{Найдите разность матриц:}\\\\";

        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            size_t rows = static_cast<size_t>(random->bounded(rangeSize.first, rangeSize.second));
            size_t cols = static_cast<size_t>(random->bounded(rangeSize.first, rangeSize.second));
            matrix.setTask(rows, cols, rangeK.first, rangeK.second);
            subMat.setTask(rows, cols, rangeK.first, rangeK.second);
            if (!mode) {
                tasksForWork += "  " + QString::number(localCount)  + ")~" + matrix.getMatrix() + "-" + subMat.getMatrix() + "=~?\\\\";
                solvedWorkTasks.emplace_back((matrix - subMat).getMatrix());
                ++localCount;
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Найдите~разность~матриц:\\\\" + matrix.getMatrix() + "-" + subMat.getMatrix() + "=~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, (matrix - subMat).getMatrix(), SupCommands::Name, 0));
            }
        } break;
    case MatrixOptions::Multy:
        if (!this->mode)
            tasksForWork += "\\textbf{Найдите произведение матриц:}\\\\";

        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            size_t rows = static_cast<size_t>(random->bounded(rangeSize.first, rangeSize.second));
            size_t cols = static_cast<size_t>(random->bounded(rangeSize.first, rangeSize.second));
            matrix.setTask(rows, cols, rangeK.first, rangeK.second);
            subMat.setTask(cols, rows, rangeK.first, rangeK.second);
            if (!mode) {
                tasksForWork += "  " + QString::number(localCount)  + ")~" + matrix.getMatrix() + "\\cdot" + subMat.getMatrix() + "=~?\\\\";
                solvedWorkTasks.emplace_back((matrix * subMat).getMatrix());
                ++localCount;
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Найдите~произведение~матриц:\\\\" + matrix.getMatrix() + "\\cdot" + subMat.getMatrix() + "=~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, (matrix * subMat).getMatrix(), SupCommands::Name, 0));
            }
        } break;
    case MatrixOptions::Inverse:
        if (!this->mode)
            tasksForWork += "\\textbf{Найдите матрицу, обратную данной:}\\\\";

        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            size_t rows = static_cast<size_t>(random->bounded(rangeSize.first, rangeSize.second));
            Matrix<double> matrix; matrix.setTask(rows, rows, rangeK.first, rangeK.second);
            if (!mode) {
                tasksForWork += "  " + QString::number(localCount)  + ")~{" + matrix.getMatrix() + "}^{-1}=~?\\\\";
                solvedWorkTasks.emplace_back((~matrix).getMatrix());
                ++localCount;
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Найдите~матрицу,~обратную~данной:\\\\{" + matrix.getMatrix() + "}^{-1}=~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, (~matrix).getMatrix(), SupCommands::Name, 0));
            }
        } break;
    case MatrixOptions::Det:
        if (!this->mode)
            tasksForWork += "\\textbf{Вычислить детерминант матрицы:}\\\\";

        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            size_t rows = static_cast<size_t>(random->bounded(rangeSize.first, rangeSize.second));
            matrix.setTask(rows, rows, rangeK.first, rangeK.second);
            if (!mode) {
                tasksForWork += "  " + QString::number(localCount)  + ")~" + matrix.getMatrix().replace("pmatrix", "vmatrix") + "=~?\\\\";
                solvedWorkTasks.emplace_back(QString::number(matrix.det()));
                ++localCount;
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Вычислить~детерминант~матрицы:\\\\" + matrix.getMatrix().replace("pmatrix", "vmatrix") + "=~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, QString::number(matrix.det()), SupCommands::Number, 0));
            }
        } break;
    }
    if (!mode) isReadyRender();
}

void GeneratorWindow::runRingResidue(int countOfTasks, int minNum, int maxNum, RingResidueOptions option)
{
    int curTaskCount = 1;
    totalTaskCount = countOfTasks;
    ui->toolBar->actions().at(0)->setEnabled(true);
    ui->toolBar->actions().at(1)->setEnabled(true);
    ui->toolBar->actions().at(3)->setEnabled(true);
    RingResidue task;
    switch (option) {
    case RingResidueOptions::GenCount:
        if (!this->mode)
            tasksForWork += "\\textbf{Вычислите количество образующих элементов группы Вычетов:}\\\\";

        for (std::size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            task.setOrder(random->bounded(minNum, maxNum));
            task.setType(RingResidueType::Summary);
            if (!mode) {
                tasksForWork += "  " + QString::number(curTaskCount)  + ")~" + task.getCode() + "=~?\\\\";
                solvedWorkTasks.emplace_back(QString::number(task.countOfGenerators()));
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Вычислите~количество~образующих~элементов~группы~Вычетов:\\\\\\left(\\mathbb{Z}_{" + QString::number(task.getOrder()) + "}, +\\right)=~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.countOfGenerators()), SupCommands::Number, 0));
            } ++curTaskCount;
        } break;
    case RingResidueOptions::A_in_M:
        if (!this->mode)
            tasksForWork += "\\textbf{Найдите значение выражения:}\\\\";

        for (std::size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            task.setOrder(random->bounded(minNum, maxNum));
            task.setType(static_cast<RingResidueType>(random->bounded(0, 2)));
            int a = static_cast<int>(random->bounded(1, task.getOrder()));
            int m = static_cast<int>(random->bounded(2, 20));
            if (!mode) {
                tasksForWork += "  " + QString::number(curTaskCount)  + ")~" + task.getCode() + ":~" +
                        QString::number(a) + "^{" + QString::number(m) + "}=~?\\\\";
                solvedWorkTasks.emplace_back(QString::number(task.solve(a, m)));
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Найдите~значение~выражения:\\\\" + task.getCode() + ":~" +
                        QString::number(a) + "^{" + QString::number(m) + "}=~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve(a, m)), SupCommands::Number, 0));
            } ++curTaskCount;
        } break;
    case RingResidueOptions::Order:
        if (!this->mode)
            tasksForWork += "\\textbf{Вычислите порядок элемента:}\\\\";

        for (std::size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            task.setOrder(random->bounded(minNum, maxNum));
            task.setType(static_cast<RingResidueType>(random->bounded(0, 2)));
            int element = static_cast<int>(random->bounded(1, task.getOrder()));
            if (!mode) {
                tasksForWork += "  " + QString::number(curTaskCount)  + ")~" + task.getCode() + ":~" +
                            "ord(" + QString::number(element) + ")=~?\\\\";
                solvedWorkTasks.emplace_back(QString::number(task.getOrd(element)));
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{\\textbf{Вычислите порядок элемента:}\\\\" +
                        task.getCode() + ":~" +"ord(" + QString::number(element) + ")=~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.getOrd(element)), SupCommands::Number, 0));
            } ++curTaskCount;
        } break;
    case RingResidueOptions::A_X_equal_B:
        if (!this->mode)
            tasksForWork += "\\textbf{Решите линейное сравнение:}\\\\";

        for (std::size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            task.setOrder(random->bounded(minNum, maxNum));
            int a, b;
            do {
                task.setOrder(random->bounded(minNum, maxNum));
                a = static_cast<int>(random->bounded(1, task.getOrder()));
                b = static_cast<int>(random->bounded(1, task.getOrder()));
            } while(task.solveLinear(a, b, task.getOrder()) == "Нет корней");
            if (!mode) {
                tasksForWork += "  " + QString::number(curTaskCount) + ")~" + task.getCode() + ":~" +
                            QString::number(a) + "\\cdot x\\equiv" + QString::number(b) + ",~x=~?\\\\";
                solvedWorkTasks.emplace_back("\\{" + task.solveLinear(a, b, task.getOrder()) + "\\}");
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{\\textbf{Решите линейное сравнение:}\\\\" +
                        task.getCode() + ":~" + QString::number(a) + "\\cdot x\\equiv" + QString::number(b) + ",~x=~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, "\\{" + task.solveLinear(a, b, task.getOrder()) + "\\}", SupCommands::Name, 0));
            } ++curTaskCount;
        } break;
    case RingResidueOptions::XX_equal_p:
        if (!this->mode)
            tasksForWork += "\\textbf{Решите квадратичное сравнение по простому модулю:}\\\\";

        for (std::size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            task.setOrder(random->bounded(minNum, maxNum));
            int a;
            do {
                task.setOrder(random->bounded(minNum, maxNum));
                a = static_cast<int>(random->bounded(1, task.getOrder()));
            } while(task.solveSimpleQuadro(a, task.getOrder()) == "Нет корней");
            if (!mode) {
                tasksForWork += "  " + QString::number(curTaskCount) + ")~" + task.getCode() + ":~" +
                            "x^2\\equiv" + QString::number(a) + ",~x=~?\\\\";
                solvedWorkTasks.emplace_back("\\{" + task.solveSimpleQuadro(a, task.getOrder()) + "\\}");
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{\\textbf{Решите квадратичное сравнение по простому модулю:}\\\\" +
                        task.getCode() + ":~" + "x^2\\equiv" + QString::number(a) + ",~x=~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, "\\{" + task.solveSimpleQuadro(a, task.getOrder()) + "\\}", SupCommands::Name, 0));
            } ++curTaskCount;
        } break;
    }
    if (!mode) isReadyRender();
}
