#include "generatorwindow.h"
#include "ui_generatorwindow.h"

GeneratorWindow::GeneratorWindow(QWidget *parent)
    : QMainWindow(parent), totalTestTasks(0), totalTaskCount(0),
            curTaskCount(0), TFWpastSize(0), mode(false),
      tasksForWork(QString("\\begin{aligned}\\large{\\color{sienna}{")),
      solvedWorkTasks(QString("\\begin{aligned}\\large{\\color{sienna}{")),
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
    ui->webView->load(QUrl("qrc:/general/web/theory.html"));
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
    if (curTaskCount == totalTaskCount) return;
    QApplication::setOverrideCursor(Qt::WaitCursor);
    taskBuffer += tasksForWork.right(tasksForWork.size() - TFWpastSize);
    TFWpastSize = tasksForWork.size();
    engine->TeX2SVG(taskBuffer + "}}\\end{aligned}");
    totalTaskCount = 0;
    curTaskCount = 0;
    QApplication::restoreOverrideCursor();
}

void GeneratorWindow::checkAnswers(){
    QApplication::setOverrideCursor(Qt::WaitCursor);
    taskBuffer = solvedWorkTasks;
    engine->TeX2SVG(taskBuffer.replace("@", "").replace("#", "") + "}}\\end{aligned}", true);
    ui->toolBar->actions().at(0)->setDisabled(true);
    QApplication::restoreOverrideCursor();
}

void GeneratorWindow::clearTasks()
{
    tasksForWork = "\\begin{aligned}\\large{\\color{sienna}{";
    solvedWorkTasks = tasksForWork;
    taskBuffer.clear();
    TFWpastSize = 0;
    engine->TeX2SVG("\\begin{aligned}\\Large{\\color{sienna}{\\Large{\\textbf{В ожидании генерации задач...}}}}\\end{aligned}", true);
    ui->toolBar->actions().at(0)->setDisabled(true);
    ui->toolBar->actions().at(1)->setDisabled(true);
    ui->toolBar->actions().at(3)->setDisabled(true);
}

void GeneratorWindow::printTasks()
{
    DialogLatexPrinter *window = new DialogLatexPrinter(tasksForWork.mid(35, tasksForWork.size()),
                                                        solvedWorkTasks.mid(35, tasksForWork.size()), this);
    window->setWindowTitle("Создание LaTeX & PDF файлов");
    window->setModal(true);
    window->exec();
}

void GeneratorWindow::openManual()
{
    DialogManual *window = new DialogManual(this);
    window->setWindowTitle("Руководство по условным обозначениям");
    window->show();
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
        connect(window, SIGNAL(timeRemaining(const QTime&)), this, SLOT(startTest(const QTime&)));
        window->setWindowTitle("Конфигурация Теста");
        window->exec();
    }
}

void GeneratorWindow::startTest(const QTime &time)
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
    if (ui->tasksList->currentItem()->text() == "Функция Эйлера") {
        DialogBase *window = new DialogBase(AllTasks::EulerFunction, true, this);
        connect(window, &DialogBase::sendingMetaInfo, this, &GeneratorWindow::receivedMetaInfo);
        connect(window, &DialogBase::sendingData, this, &GeneratorWindow::receivedData);

    } else if (ui->tasksList->currentItem()->text() == "Функция Мёбиуса") {
        DialogBase *window = new DialogBase(AllTasks::MebiusFunction, true, this);
        connect(window, &DialogBase::sendingMetaInfo, this, &GeneratorWindow::receivedMetaInfo);
        connect(window, &DialogBase::sendingData, this, &GeneratorWindow::receivedData);

    } else if (ui->tasksList->currentItem()->text() == "Символ Лежандра") {
        DialogBase *window = new DialogBase(AllTasks::SymbolLegandre, true, this);
        connect(window, &DialogBase::sendingMetaInfo, this, &GeneratorWindow::receivedMetaInfo);
        connect(window, &DialogBase::sendingData, this, &GeneratorWindow::receivedData);

    } else if (ui->tasksList->currentItem()->text() == "Символ Якоби") {
        DialogBase *window = new DialogBase(AllTasks::SymbolJacobi, true, this);
        connect(window, &DialogBase::sendingMetaInfo, this, &GeneratorWindow::receivedMetaInfo);
        connect(window, &DialogBase::sendingData, this, &GeneratorWindow::receivedData);

    } else if (ui->tasksList->currentItem()->text() == "Группа Перестановок") {
        DialogBase *window = new DialogBase(AllTasks::TranspositionGroup, true, this);
        connect(window, &DialogBase::sendingMetaInfo, this, &GeneratorWindow::receivedMetaInfo);
        connect(window, &DialogBase::sendingData, this, &GeneratorWindow::receivedData);

    } else if (ui->tasksList->currentItem()->text() == "Алгебраические Структуры") {
        DialogBase *window = new DialogBase(AllTasks::Set, true, this);
        connect(window, &DialogBase::sendingMetaInfo, this, &GeneratorWindow::receivedMetaInfo);
        connect(window, &DialogBase::sendingData, this, &GeneratorWindow::receivedData);
    } else if (ui->tasksList->currentItem()->text() == "Матрицы") {
        DialogBase *window = new DialogBase(AllTasks::Matrix, true, this);
        connect(window, &DialogBase::sendingMetaInfo, this, &GeneratorWindow::receivedMetaInfo);
        connect(window, &DialogBase::sendingData, this, &GeneratorWindow::receivedData);
    }
}

void GeneratorWindow::receivedMetaInfo(int countOfTasks, bool isRepeatable, QString taskText)
{
    curTaskCount = 1;
    if (mode) {
        totalTestTasks += countOfTasks;
        statusBar()->showMessage("Задания сгенерированы!", 1500);
        ui->lcdNumber->display(totalTestTasks);
    } else if (isRepeatable) {
        totalTaskCount = countOfTasks;
        tasksForWork += "\\Large{\\textbf{" + taskText + "}}\\\\";
        solvedWorkTasks += "\\Large{\\textbf{" + taskText + "}}\\\\";
        ui->toolBar->actions().at(0)->setEnabled(true);
        ui->toolBar->actions().at(1)->setEnabled(true);
        ui->toolBar->actions().at(3)->setEnabled(true);
    }
}

void GeneratorWindow::receivedData(std::vector<int> data, AllTasks task, int subTask, ViewMode optional)
{
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
    }
}

void GeneratorWindow::on_comboBox_currentTextChanged(const QString &task)
{
    if (!ui->mainLayout->isEmpty())
        delete ui->mainLayout->takeAt(0)->widget();
    if (task == "Группа Перестановок") {
        DialogBase *window = new DialogBase(AllTasks::EulerFunction, false, this);
        connect(window, &DialogBase::sendingMetaInfo, this, &GeneratorWindow::receivedMetaInfo);
        connect(window, &DialogBase::sendingData, this, &GeneratorWindow::receivedData);
        ui->mainLayout->addWidget(window);

    } else if (task == "Функция Эйлера") {
        DialogBase *window = new DialogBase(AllTasks::EulerFunction, false, this);
        connect(window, &DialogBase::sendingMetaInfo, this, &GeneratorWindow::receivedMetaInfo);
        connect(window, &DialogBase::sendingData, this, &GeneratorWindow::receivedData);
        ui->mainLayout->addWidget(window);

    } else if (task == "Функция Мёбиуса") {
        DialogBase *window = new DialogBase(AllTasks::MebiusFunction, false, this);
        connect(window, &DialogBase::sendingMetaInfo, this, &GeneratorWindow::receivedMetaInfo);
        connect(window, &DialogBase::sendingData, this, &GeneratorWindow::receivedData);
        ui->mainLayout->addWidget(window);

    } else if (task == "Символ Лежандра") {
        DialogBase *window = new DialogBase(AllTasks::SymbolLegandre, false, this);
        connect(window, &DialogBase::sendingMetaInfo, this, &GeneratorWindow::receivedMetaInfo);
        connect(window, &DialogBase::sendingData, this, &GeneratorWindow::receivedData);
        ui->mainLayout->addWidget(window);

    } else if (task == "Символ Якоби") {
        DialogBase *window = new DialogBase(AllTasks::SymbolJacobi, false, this);
        connect(window, &DialogBase::sendingMetaInfo, this, &GeneratorWindow::receivedMetaInfo);
        connect(window, &DialogBase::sendingData, this, &GeneratorWindow::receivedData);
        ui->mainLayout->addWidget(window);

    } else if (task == "Алгебраические Структуры") {
        DialogBase *window = new DialogBase(AllTasks::Set, false, this);
        connect(window, &DialogBase::sendingMetaInfo, this, &GeneratorWindow::receivedMetaInfo);
        connect(window, &DialogBase::sendingData, this, &GeneratorWindow::receivedData);
        ui->mainLayout->addWidget(window);
    } else if (task == "Группы и их свойства") {
        DialogBase *window = new DialogBase(AllTasks::GroupProperties, false, this);
            connect(window, &DialogBase::sendingMetaInfo, this, &GeneratorWindow::receivedMetaInfo);
            connect(window, &DialogBase::sendingData, this, &GeneratorWindow::receivedData);
        ui->mainLayout->addWidget(window);
    } else if (task == "Матрицы") {
        DialogBase *window = new DialogBase(AllTasks::Matrix, false, this);
            connect(window, &DialogBase::sendingMetaInfo, this, &GeneratorWindow::receivedMetaInfo);
            connect(window, &DialogBase::sendingData, this, &GeneratorWindow::receivedData);
        ui->mainLayout->addWidget(window);
    }
}

void GeneratorWindow::runEulerFunction(int countOfTasks, int minNum, int maxNum, EulerFunctionOptions option)
{
    EulerFunction task;
    switch (option) {
    case EulerFunctionOptions::Default:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            task.setTask(random->bounded(minNum, maxNum));
            if (!mode) {
                tasksForWork += "  " + QString::number(curTaskCount)  + ")~\\phi(" + QString::number(task.getTask()) + ")=~?\\\\";
                solvedWorkTasks += QString::number(curTaskCount)  + "@)~\\phi(" + QString::number(task.getTask()) + ")=#" + QString::number(task.solve()) + "@\\\\#";
                isReadyRender();
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
                    solvedWorkTasks += "  " + QString::number(curTaskCount)  + ")~\\phi(" + QString::number(task.getTask()) + ")=" + QString::number(task.solve()) + "\\\\";
                    isReadyRender();
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
                    solvedWorkTasks += "  " + QString::number(curTaskCount)  + ")~\\phi(" + QString::number(task.getTask()) + ")=" + QString::number(task.solve()) + "\\\\";
                    isReadyRender();
                } else {
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
                    solvedWorkTasks += "  " + QString::number(curTaskCount)  + ")~\\phi(" + QString::number(task.getTask()) + ")=" + QString::number(task.solve()) + "\\\\";
                    isReadyRender();
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
                    solvedWorkTasks += "  " + QString::number(curTaskCount)  + ")~\\phi(" + QString::number(task.getTask()) + ")=" + QString::number(task.solve()) + "\\\\";
                    isReadyRender();
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~функцию~Эйлера:\\\\\\phi(" + QString::number(task.getTask()) + ")=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i; ++curTaskCount;
            }
        } break;
    }
}

void GeneratorWindow::runMebiusFunction(int countOfTasks, int minNum, int maxNum, MebiusFunctionOptions option)
{
    MebiusFunction task;
    switch (option) {
    case MebiusFunctionOptions::Default:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            task.setTask(random->bounded(minNum, maxNum));
            if (!mode) {
                tasksForWork += "  " + QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=~?\\\\";
                solvedWorkTasks += QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=" + QString::number(task.solve()) + "\\\\";
                isReadyRender();
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
                    solvedWorkTasks += QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=" + QString::number(task.solve()) + "\\\\";
                    isReadyRender();
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
                    solvedWorkTasks += QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=" + QString::number(task.solve()) + "\\\\";
                    isReadyRender();
                } else {
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
                    solvedWorkTasks += QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=" + QString::number(task.solve()) + "\\\\";
                    isReadyRender();
                } else {
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
                    solvedWorkTasks += QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=" + QString::number(task.solve()) + "\\\\";
                    isReadyRender();
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~функцию~Мёбиуса:\\\\\\mu(" + QString::number(task.getTask()) + ")=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i; ++curTaskCount;
            }
        } break;
    }
}

void GeneratorWindow::runSymbolLegandre(int countOfTasks, std::pair<int, int> a, std::pair<int, int> p, SymbolLegandreOptions option)
{
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
                    solvedWorkTasks += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                            "}\\right)=" + QString::number(task.solve()) + "\\\\"; isReadyRender();
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
                    solvedWorkTasks += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                            "}\\right)=" + QString::number(task.solve()) + "\\\\"; isReadyRender();
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
                    solvedWorkTasks += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                            "}\\right)=" + QString::number(task.solve()) + "\\\\"; isReadyRender();
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
                    solvedWorkTasks += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                            "}\\right)=" + QString::number(task.solve()) + "\\\\"; isReadyRender();
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
                    solvedWorkTasks += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                            "}\\right)=" + QString::number(task.solve()) + "\\\\"; isReadyRender();
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~символ~Лежандра:\\\\\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i; ++curTaskCount;
            }
        } break;
    }
}

void GeneratorWindow::runSymbolJacobi(int countOfTasks, std::pair<int, int> a, std::pair<int, int> p, SymbolJacobiOptions option)
{
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
                    solvedWorkTasks += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                            "}\\right)=" + QString::number(task.solve()) + "\\\\"; isReadyRender();
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
                    solvedWorkTasks += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                            "}\\right)=" + QString::number(task.solve()) + "\\\\"; isReadyRender();
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
                    solvedWorkTasks += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                            "}\\right)=" + QString::number(task.solve()) + "\\\\"; isReadyRender();
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
                    solvedWorkTasks += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                            "}\\right)=" + QString::number(task.solve()) + "\\\\"; isReadyRender();
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
                    solvedWorkTasks += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                            "}\\right)=" + QString::number(task.solve()) + "\\\\"; isReadyRender();
                } else {
                    QString taskText = "\\begin{align}\\color{sienna}{Вычислите~символ~Якоби:\\\\\\left(\\frac{" +
                            QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}\\end{align}";
                    tasksForTest.push_back(std::make_tuple(taskText, QString::number(task.solve()), SupCommands::Number, 0));
                } ++i; ++curTaskCount;
            }
        } break;
    }
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
            if (mode == ViewMode::Standart) {
                tasksForWork += "\\Large{\\textbf{Запишите подстановку в виде произведения независимых циклов:}}\\\\";
                solvedWorkTasks += "\\Large{\\textbf{Запишите подстановку в виде произведения независимых циклов:}}\\\\";
            } else {
                tasksForWork += "\\Large{\\textbf{Запишите подстановку в табличном виде:}}\\\\";
                solvedWorkTasks += "\\Large{\\textbf{Запишите подстановку в табличном виде:}}\\\\";
            }
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=~?\\\\";
                    solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=" + task.writeToMode(ViewMode::Cycle) + "\\\\";
                } else {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "=~?\\\\";
                    solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "=" + task.writeToMode(ViewMode::Standart) + "\\\\";
                } isReadyRender(); ++localCount;
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
            } ++i; ++curTaskCount;
        } break;

    case TranspositionGroupOptions::Multiply:
        if (!this->mode) {
            if (mode == ViewMode::Standart) {
                tasksForWork += "\\Large{\\textbf{Найдите произведение подстановок, записанных в табличном виде:}}\\\\";
                solvedWorkTasks += "\\Large{\\textbf{Найдите произведение подстановок, записанных в табличном виде:}}\\\\";
            } else {
                tasksForWork += "\\Large{\\textbf{Найдите произведение подстановок, записанных в циклическом виде:}}\\\\";
                solvedWorkTasks += "\\Large{\\textbf{Найдите произведение подстановок, записанных в циклическом виде:}}\\\\";
            }
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
                    solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" +
                            task.writeToMode(ViewMode::Standart) + "\\cdot" + task2.writeToMode(ViewMode::Standart) + "=" + result.writeToMode(ViewMode::Standart) + "\\\\";
                } else {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" +
                            task.writeToMode(ViewMode::Cycle) + "\\cdot" + task2.writeToMode(ViewMode::Cycle) + "=~?\\\\";
                    solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" +
                            task.writeToMode(ViewMode::Cycle) + "\\cdot" + task2.writeToMode(ViewMode::Cycle) + "=" + result.writeToMode(ViewMode::Cycle) + "\\\\";
                } isReadyRender(); ++localCount;
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
            } ++i; ++curTaskCount;
        } break;

    case TranspositionGroupOptions::Inverse:
        if (!this->mode) {
            if (mode == ViewMode::Standart) {
                tasksForWork += "\\Large{\\textbf{Найдите подстановку, обратную данной, записанной в табличном виде:}}\\\\";
                solvedWorkTasks += "\\Large{\\textbf{Найдите подстановку, обратную данной, записанной в табличном виде:}}\\\\";
            } else {
                tasksForWork += "\\Large{\\textbf{Найдите подстановку, обратную~данной, записанной в циклическом виде:}}\\\\";
                solvedWorkTasks += "\\Large{\\textbf{Найдите подстановку, обратную~данной, записанной в циклическом виде:}}\\\\";
            }
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode);
            result = ~task;
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:{" + task.writeToMode(ViewMode::Standart) + "}^{-1}=~?\\\\";
                    solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:{" + task.writeToMode(ViewMode::Standart) + "}^{-1}=" +
                            result.writeToMode(ViewMode::Standart) + "\\\\";
                } else {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:{" + task.writeToMode(ViewMode::Cycle) + "}^{-1}=~?\\\\";
                    solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:{" + task.writeToMode(ViewMode::Cycle) + "}^{-1}=" +
                            result.writeToMode(ViewMode::Cycle) + "\\\\";
                } isReadyRender(); ++localCount;
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
            } ++i; ++curTaskCount;
        } break;

    case TranspositionGroupOptions::Cycle:
        if (!this->mode) {
            if (mode == ViewMode::Standart) {
                tasksForWork += "\\Large{\\textbf{Найдите цикловой тип подстановки, записанной в табличном виде:}}\\\\";
                solvedWorkTasks += "\\Large{\\textbf{Найдите цикловой тип подстановки, записанной в табличном виде:}}\\\\";
            } else {
                tasksForWork += "\\Large{\\textbf{Найдите цикловой тип подстановки, записанной в циклическом виде:}}\\\\";
                solvedWorkTasks += "\\Large{\\textbf{Найдите цикловой тип подстановки, записанной в циклическом виде:}}\\\\";
            }
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=~?\\\\";
                    solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=" +
                            task.cycleType() + "\\\\";
                } else {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "=~?\\\\";
                    solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "=" +
                            task.cycleType() + "\\\\";
                } isReadyRender(); ++localCount;
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
            } ++i; ++curTaskCount;
        } break;

    case TranspositionGroupOptions::Count:
        if (!this->mode) {
            if (mode == ViewMode::Standart) {
                tasksForWork += "\\Large{\\textbf{Вычислите количество беспорядков подстановки, записанной в табличном виде:}}\\\\";
                solvedWorkTasks += "\\Large{\\textbf{Вычислите количество беспорядков подстановки, записанной в табличном виде:}}\\\\";
            } else {
                tasksForWork += "\\Large{\\textbf{Вычислите количество беспорядков подстановки, записанной в циклическом виде:}}\\\\";
                solvedWorkTasks += "\\Large{\\textbf{Вычислите количество беспорядков подстановки, записанной в циклическом виде:}}\\\\";
            }
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\Delta" + task.writeToMode(ViewMode::Standart) + "=~?\\\\";
                    solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\Delta" + task.writeToMode(ViewMode::Standart) + "=" +
                            QString::number(task.getHaos()) + "\\\\";
                } else {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\Delta" + task.writeToMode(ViewMode::Cycle) + "=~?\\\\";
                    solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\Delta" + task.writeToMode(ViewMode::Cycle) + "=" +
                            QString::number(task.getHaos()) + "\\\\";
                } isReadyRender(); ++localCount;
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
            } ++i; ++curTaskCount;
        } break;

    case TranspositionGroupOptions::Mod2:
        if (!this->mode) {
            if (mode == ViewMode::Standart && this->mode) {
                tasksForWork += "\\Large{\\textbf{Определите четность подстановки, записанной в табличном виде:}}\\\\";
                solvedWorkTasks += "\\Large{\\textbf{Определите четность подстановки, записанной в табличном виде:}}\\\\";
            } else {
                tasksForWork += "\\Large{\\textbf{Определите четность подстановки, записанной в циклическом виде:}}\\\\";
                solvedWorkTasks += "\\Large{\\textbf{Определите четность подстановки, записанной в циклическом виде:}}\\\\";
            }
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\delta" + task.writeToMode(ViewMode::Standart) + "\\Rightarrow~?\\\\";
                    solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\delta" + task.writeToMode(ViewMode::Standart) + "\\Rightarrow" +
                            task.getEven() + "\\\\";
                } else {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\delta" + task.writeToMode(ViewMode::Cycle) + "\\Rightarrow~?\\\\";
                    solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\delta" + task.writeToMode(ViewMode::Cycle) + "\\Rightarrow" +
                            task.getEven() + "\\\\";
                } isReadyRender(); ++localCount;
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
            } ++i; ++curTaskCount;
        } break;

    case TranspositionGroupOptions::Order:
        if (!this->mode) {
            if (mode == ViewMode::Standart) {
                tasksForWork += "\\Large{\\textbf{Определите порядок подстановки, записанной в табличном виде:}}\\\\";
                solvedWorkTasks += "\\Large{\\textbf{Определите порядок подстановки, записанной в табличном виде:}}\\\\";
            } else {
                tasksForWork += "\\Large{\\textbf{Определите порядок подстановки, записанной в циклическом виде:}}\\\\";
                solvedWorkTasks += "\\Large{\\textbf{Определите порядок подстановки, записанной в циклическом виде:}}\\\\";
            }
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:~ord" + task.writeToMode(ViewMode::Standart) + "=~?\\\\";
                    solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:~ord" + task.writeToMode(ViewMode::Standart) + "=" +
                            QString::number(task.getOrder()) + "\\\\";
                } else {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:~ord" + task.writeToMode(ViewMode::Cycle) + "=~?\\\\";
                    solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:~ord" + task.writeToMode(ViewMode::Cycle) + "=" +
                            QString::number(task.getOrder()) + "\\\\";
                } isReadyRender(); ++localCount;
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
            } ++i; ++curTaskCount;
        } break;

    case TranspositionGroupOptions::Decomposition:
        if (!this->mode) {
            tasksForWork += "\\Large{\\textbf{Запишите подстановку в виде произведения транспозиций:}}\\\\";
            solvedWorkTasks += "\\Large{\\textbf{Запишите подстановку в виде произведения транспозиций:}}\\\\";
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode, true);
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=~?\\\\";
                    solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=" + task.writeToMode(ViewMode::Decomposition) + "\\\\";
                } else {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "=~?\\\\";
                    solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "=" + task.writeToMode(ViewMode::Decomposition) + "\\\\";
                } isReadyRender(); ++localCount;
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
            } ++i; ++curTaskCount;
        } break;

    case TranspositionGroupOptions::Neighbor:
        if (!this->mode) {
            tasksForWork += "\\Large{\\textbf{Запишите подстановку в виде произведения транспозиций соседних элементов:}}\\\\";
            solvedWorkTasks += "\\Large{\\textbf{Запишите подстановку в виде произведения транспозиций соседних элементов:}}\\\\";
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode, true);
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=~?\\\\";
                    solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=" + task.writeToMode(ViewMode::Neighbors) + "\\\\";
                } else {
                    tasksForWork += "  " + QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "=~?\\\\";
                    solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "=" + task.writeToMode(ViewMode::Neighbors) + "\\\\";
                } isReadyRender(); ++localCount;
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
            } ++i; ++curTaskCount;
        } break;
    }
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
        if (!this->mode) {
            tasksForWork += "\\Large{\\textbf{Чем является данная Алгебраическая Структура:}}\\\\";
            solvedWorkTasks += "\\Large{\\textbf{Чем является данная Алгебраическая Структура:}}\\\\";
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            if (!mode) {
                tasksForWork += "  " + QString::number(localCount)  + ")~\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?\\\\";
                solvedWorkTasks += QString::number(localCount)  + ")~\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow" + std::get<2>(data[i]) + "\\\\";
                isReadyRender(); ++localCount;
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Чем~является~данная~Алгебраическая~Структура:\\\\\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, std::get<2>(data[i]), SupCommands::Name, 0));
            } ++curTaskCount;
        } break;

    case SetOptions::Oper:
        if (!this->mode) {
            tasksForWork += "\\Large{\\textbf{Является ли данная операция заданной на множестве:}}\\\\";
            solvedWorkTasks += "\\Large{\\textbf{Является ли данная операция заданной на множестве:}}\\\\";
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            QString answer;
            if (QString(std::get<2>(data[i])).toInt() / 1000)
                answer = "Да";
            else answer = "Нет";
            if (!mode) {
                tasksForWork += "  " + QString::number(localCount)  + ")~\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?\\\\";
                solvedWorkTasks += QString::number(localCount)  + ")~\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow" + answer + "\\\\";
                isReadyRender(); ++localCount;
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Чем~является~данная~Алгебраическая~Структура:\\\\\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, answer, SupCommands::Name, 0));
            } ++curTaskCount;
        } break;

    case SetOptions::Abel:
        if (!this->mode) {
            tasksForWork += "\\Large{\\textbf{Является ли данная операция коммутативной на множестве:}}\\\\";
            solvedWorkTasks += "\\Large{\\textbf{Является ли данная операция коммутативной на множестве:}}\\\\";
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            QString answer;
            if (QString(std::get<2>(data[i])).toInt() / 100)
                answer = "Да";
            else answer = "Нет";
            if (!mode) {
                tasksForWork += "  " + QString::number(localCount)  + ")~\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?\\\\";
                solvedWorkTasks += QString::number(localCount)  + ")~\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow" + answer + "\\\\";
                isReadyRender(); ++localCount;
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Чем~является~данная~Алгебраическая~Структура:\\\\\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, answer, SupCommands::Name, 0));
            } ++curTaskCount;
        } break;

    case SetOptions::Associate:
        if (!this->mode) {
            tasksForWork += "\\Large{\\textbf{Является ли данная операция ассоциативной на множестве:}}\\\\";
            solvedWorkTasks += "\\Large{\\textbf{Является ли данная операция ассоциативной на множестве:}}\\\\";
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            QString answer;
            if (QString(std::get<2>(data[i])).toInt() / 10)
                answer = "Да";
            else answer = "Нет";
            if (!mode) {
                tasksForWork += "  " + QString::number(localCount)  + ")~\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?\\\\";
                solvedWorkTasks += QString::number(localCount)  + ")~\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow" + answer + "\\\\";
                isReadyRender(); ++localCount;
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Чем~является~данная~Алгебраическая~Структура:\\\\\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, answer, SupCommands::Name, 0));
            } ++curTaskCount;
        } break;

    case SetOptions::Neutral:
        if (!this->mode) {
            tasksForWork += "\\Large{\\textbf{Существует ли нейтральный элемент на заданной алгебраической структуре:}}\\\\";
            solvedWorkTasks += "\\Large{\\textbf{Существует ли нейтральный элемент на заданной алгебраической структуре:}}\\\\";
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            QString answer;
            if (QString(std::get<2>(data[i])).toInt() % 10)
                answer = "Да";
            else answer = "Нет";
            if (!mode) {
                tasksForWork += "  " + QString::number(localCount)  + ")~\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?\\\\";
                solvedWorkTasks += QString::number(localCount)  + ")~\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow" + answer + "\\\\";
                isReadyRender(); ++localCount;
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Чем~является~данная~Алгебраическая~Структура:\\\\\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, answer, SupCommands::Name, 0));
            } ++curTaskCount;
        } break;
    }
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
        if (!this->mode) {
            tasksForWork += "\\Large{\\textbf{Вычислите сумму матриц:}}\\\\";
            solvedWorkTasks += "\\Large{\\textbf{Вычислите сумму матриц:}}\\\\";
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            size_t rows = static_cast<size_t>(random->bounded(rangeSize.first, rangeSize.second));
            size_t cols = static_cast<size_t>(random->bounded(rangeSize.first, rangeSize.second));
            matrix.setTask(rows, cols, rangeK.first, rangeK.second);
            subMat.setTask(rows, cols, rangeK.first, rangeK.second);
            if (!mode) {
                tasksForWork += "  " + QString::number(localCount)  + ")~" + matrix.getMatrix() + "+" + subMat.getMatrix() + "=~?\\\\";
                solvedWorkTasks += QString::number(localCount)  + ")~" + matrix.getMatrix() + "+" + subMat.getMatrix() + "=~" + (matrix + subMat).getMatrix() + "\\\\";
                isReadyRender(); ++localCount;
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Вычислите~сумму~матриц:\\\\" + matrix.getMatrix() + "+" + subMat.getMatrix() + "=~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, (matrix + subMat).getMatrix(), SupCommands::Name, 0));
            } ++curTaskCount;
        } break;
    case MatrixOptions::Diff:
        if (!this->mode) {
            tasksForWork += "\\Large{\\textbf{Найдите разность матриц:}}\\\\";
            solvedWorkTasks += "\\Large{\\textbf{Найдите разность матриц:}}\\\\";
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            size_t rows = static_cast<size_t>(random->bounded(rangeSize.first, rangeSize.second));
            size_t cols = static_cast<size_t>(random->bounded(rangeSize.first, rangeSize.second));
            matrix.setTask(rows, cols, rangeK.first, rangeK.second);
            subMat.setTask(rows, cols, rangeK.first, rangeK.second);
            if (!mode) {
                tasksForWork += "  " + QString::number(localCount)  + ")~" + matrix.getMatrix() + "-" + subMat.getMatrix() + "=~?\\\\";
                solvedWorkTasks += QString::number(localCount)  + ")~" + matrix.getMatrix() + "-" + subMat.getMatrix() + "=~" + (matrix - subMat).getMatrix() + "\\\\";
                isReadyRender(); ++localCount;
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Найдите~разность~матриц:\\\\" + matrix.getMatrix() + "-" + subMat.getMatrix() + "=~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, (matrix - subMat).getMatrix(), SupCommands::Name, 0));
            } ++curTaskCount;
        } break;
    case MatrixOptions::Multy:
        if (!this->mode) {
            tasksForWork += "\\Large{\\textbf{Найдите произведение матриц:}}\\\\";
            solvedWorkTasks += "\\Large{\\textbf{Найдите произведение матриц:}}\\\\";
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            size_t rows = static_cast<size_t>(random->bounded(rangeSize.first, rangeSize.second));
            size_t cols = static_cast<size_t>(random->bounded(rangeSize.first, rangeSize.second));
            matrix.setTask(rows, cols, rangeK.first, rangeK.second);
            subMat.setTask(cols, rows, rangeK.first, rangeK.second);
            if (!mode) {
                tasksForWork += "  " + QString::number(localCount)  + ")~" + matrix.getMatrix() + "\\cdot" + subMat.getMatrix() + "=~?\\\\";
                solvedWorkTasks += QString::number(localCount)  + ")~" + matrix.getMatrix() + "\\cdot" + subMat.getMatrix() + "=~" + (matrix * subMat).getMatrix() + "\\\\";
                isReadyRender(); ++localCount;
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Найдите~произведение~матриц:\\\\" + matrix.getMatrix() + "\\cdot" + subMat.getMatrix() + "=~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, (matrix * subMat).getMatrix(), SupCommands::Name, 0));
            } ++curTaskCount;
        } break;
    case MatrixOptions::Inverse:
        if (!this->mode) {
            tasksForWork += "\\Large{\\textbf{Найдите матрицу, обратную данной:}}\\\\";
            solvedWorkTasks += "\\Large{\\textbf{Найдите матрицу, обратную данной:}}\\\\";
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            size_t rows = static_cast<size_t>(random->bounded(rangeSize.first, rangeSize.second));
            Matrix<double> matrix; matrix.setTask(rows, rows, rangeK.first, rangeK.second);
            if (!mode) {
                tasksForWork += "  " + QString::number(localCount)  + ")~{" + matrix.getMatrix() + "}^{-1}=~?\\\\";
                solvedWorkTasks += QString::number(localCount)  + ")~{" + matrix.getMatrix() + "}^{-1}=~" + (~matrix).getMatrix() + "\\\\";
                isReadyRender(); ++localCount;
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Найдите~матрицу,~обратную~данной:\\\\{" + matrix.getMatrix() + "}^{-1}=~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, (~matrix).getMatrix(), SupCommands::Name, 0));
            } ++curTaskCount;
        } break;
    case MatrixOptions::Det:
        if (!this->mode) {
            tasksForWork += "\\Large{\\textbf{Вычислить детерминант матрицы:}}\\\\";
            solvedWorkTasks += "\\Large{\\textbf{Вычислить детерминант матрицы:}}\\\\";
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            size_t rows = static_cast<size_t>(random->bounded(rangeSize.first, rangeSize.second));
            matrix.setTask(rows, rows, rangeK.first, rangeK.second);
            if (!mode) {
                tasksForWork += "  " + QString::number(localCount)  + ")~" + matrix.getMatrix().replace("pmatrix", "vmatrix") + "=~?\\\\";
                solvedWorkTasks += QString::number(localCount)  + ")~" + matrix.getMatrix().replace("pmatrix", "vmatrix") + "=~" + QString::number(matrix.det()) + "\\\\";
                isReadyRender(); ++localCount;
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Вычислить~детерминант~матрицы:\\\\" + matrix.getMatrix().replace("pmatrix", "vmatrix") + "=~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, QString::number(matrix.det()), SupCommands::Number, 0));
            } ++curTaskCount;
        } break;
    }
}
