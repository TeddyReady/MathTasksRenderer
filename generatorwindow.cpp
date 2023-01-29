#include "generatorwindow.h"
#include "ui_generatorwindow.h"

GeneratorWindow::GeneratorWindow(QWidget *parent)
    : QMainWindow(parent), totalTestTasks(0),
      totalTaskCount(0), curTaskCount(0), TFWpastSize(0), mode(false),
      tasksForWork(QString("\\begin{align}\\large{\\color{sienna}{")),
      solvedWorkTasks(QString("\\begin{align}\\large{\\color{sienna}{")),
      random(QRandomGenerator::global()),
      ui(new Ui::GeneratorWindow)
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
    if (curTaskCount == totalTaskCount) {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        taskBuffer += tasksForWork.right(tasksForWork.size() - TFWpastSize);
        TFWpastSize = tasksForWork.size();
        engine->TeX2SVG(taskBuffer + "}}\\end{align}", true);
        totalTaskCount = 0;
        curTaskCount = 0;
        QApplication::restoreOverrideCursor();
    }
}

void GeneratorWindow::checkAnswers(){
    QApplication::setOverrideCursor(Qt::WaitCursor);
    engine->TeX2SVG(solvedWorkTasks + "}}\\end{align}", true);
    taskBuffer = solvedWorkTasks;
    ui->toolBar->actions().at(0)->setDisabled(true);
    QApplication::restoreOverrideCursor();
}

void GeneratorWindow::clearTasks()
{
    tasksForWork = "\\begin{align}\\large{\\color{sienna}{";
    solvedWorkTasks = tasksForWork;
    taskBuffer.clear();
    TFWpastSize = 0;
    engine->TeX2SVG("\\begin{align}\\Large{\\color{sienna}{\\Large{\\textbf{В ожидании генерации задач...}}}}\\end{align}", true);
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
    //statusBar()->showMessage("Справочник открыт", 2500);
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
        DialogEulerFunction *window = new DialogEulerFunction(this);
        window->setWindowTitle("Выберите настройки генерации");
        window->show();
        connect(window, SIGNAL(dialogEulerFunctionMeta(const int&)),
                this, SLOT(slotDialogEulerFunctionMeta(const int&)));
        connect(window, SIGNAL(dialogEulerFunction(const int&, const int&, const int&, const EulerFunctionOptions&)),
                this, SLOT(slotDialogEulerFunction(const int&, const int&, const int&, const EulerFunctionOptions&)));
    } else if (ui->tasksList->currentItem()->text() == "Функция Мёбиуса") {
        DialogMebiusFunction *window = new DialogMebiusFunction(this);
        window->setWindowTitle("Выберите настройки генерации");
        window->show();
        connect(window, SIGNAL(dialogMebiusFunctionMeta(const int&)),
                this, SLOT(slotDialogMebiusFunctionMeta(const int&)));
        connect(window, SIGNAL(dialogMebiusFunction(const int&, const int&, const int&, const MebiusFunctionOptions&)),
                this, SLOT(slotDialogMebiusFunction(const int&, const int&, const int&, const MebiusFunctionOptions&)));
    } else if (ui->tasksList->currentItem()->text() == "Символ Лежандра") {
        DialogSymbolLegandre *window = new DialogSymbolLegandre(this);
        window->setWindowTitle("Выберите настройки генерации");
        window->show();
        connect(window, SIGNAL(dialogSymbolLegandreMeta(const int&)),
                this, SLOT(slotDialogSymbolLegandreMeta(const int&)));
        connect(window, SIGNAL(dialogSymbolLegandre(const int&, const std::pair<int, int>&, const std::pair<int, int>&, const SymbolLegandreOptions&)),
                this, SLOT(slotDialogSymbolLegandre(const int&, const std::pair<int, int>&, const std::pair<int, int>&, const SymbolLegandreOptions&)));
    } else if (ui->tasksList->currentItem()->text() == "Символ Якоби") {
        DialogSymbolJacobi *window = new DialogSymbolJacobi(this);
        window->setWindowTitle("Выберите настройки генерации");
        window->show();
        connect(window, SIGNAL(dialogSymbolJacobiMeta(const int&)),
                this, SLOT(slotDialogSymbolJacobiMeta(const int&)));
        connect(window, SIGNAL(dialogSymbolJacobi(const int&, const std::pair<int, int>&, const std::pair<int, int>&, const SymbolJacobiOptions&)),
                this, SLOT(slotDialogSymbolJacobi(const int&, const std::pair<int, int>&, const std::pair<int, int>&, const SymbolJacobiOptions&)));
    } else if (ui->tasksList->currentItem()->text() == "Группа Перестановок") {
        DialogTranspositionGroup *window = new DialogTranspositionGroup(this);
        window->setWindowTitle("Выберите настройки генерации");
        window->show();
        connect(window, SIGNAL(dialogTranspositionGroupMeta(const int&)),
                this, SLOT(slotDialogTranspositionGroupMeta(const int&)));
        connect(window, SIGNAL(dialogTranspositionGroup(const int&, const int&, const int&, const TranspositionGroupOptions&, const ViewMode&)),
                this, SLOT(slotDialogTranspositionGroup(const int&, const int&, const int&, const TranspositionGroupOptions&, const ViewMode&)));
    } else if (ui->tasksList->currentItem()->text() == "Алгебраические Структуры") {
        DialogSet *window = new DialogSet(this);
        window->setWindowTitle("Выберите настройки генерации");
        window->show();
        connect(window, SIGNAL(dialogSetMeta(const int&)),
                this, SLOT(slotDialogSetMeta(const int&)));
        connect(window, SIGNAL(dialogSet(const int&, const set_type &, const SetOptions&)),
                this, SLOT(slotDialogSet(const int&, const set_type &, const SetOptions&)));
        }
}

void GeneratorWindow::slotDialogEulerFunctionMeta(const int &countOfTasks)
{
    if (!countOfTasks) return;
    curTaskCount = 1;
    if (mode) {
        totalTestTasks += countOfTasks;
        statusBar()->showMessage("Задания на функцию Эйлера сгенерированы!", 1500);
        ui->lcdNumber->display(totalTestTasks);
    } else {
        totalTaskCount = countOfTasks;
        tasksForWork += "\\Large{\\textbf{Вычислите функцию Эйлера:}}\\\\";
        solvedWorkTasks += "\\Large{\\textbf{Вычислите функцию Эйлера:}}\\\\";
        ui->toolBar->actions().at(0)->setEnabled(true);
        ui->toolBar->actions().at(1)->setEnabled(true);
        ui->toolBar->actions().at(3)->setEnabled(true);
    }
}
void GeneratorWindow::slotDialogEulerFunction(const int &countOfTasks, const int &minNum, const int &maxNum, const EulerFunctionOptions &option)
{
    if (!countOfTasks) return;
    EulerFunction task;
    switch (option) {
    case EulerFunctionOptions::Default:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {            
            task.setTask(random->bounded(minNum, maxNum));
            if (!mode) {
                tasksForWork += "  " + QString::number(curTaskCount)  + ")~\\phi(" + QString::number(task.getTask()) + ")=~?\\\\";
                solvedWorkTasks += "  " + QString::number(curTaskCount)  + ")~\\phi(" + QString::number(task.getTask()) + ")=" + QString::number(task.solve()) + "\\\\";
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

void GeneratorWindow::slotDialogMebiusFunctionMeta(const int &countOfTasks){
    if (!countOfTasks) return;
    curTaskCount = 1;
    if (mode) {
        totalTestTasks += countOfTasks;
        statusBar()->showMessage("Задания на функцию Мёбиуса сгенерированы!", 1500);
        ui->lcdNumber->display(totalTestTasks);
    } else {
        totalTaskCount = countOfTasks;
        tasksForWork += "\\Large{\\textbf{Вычислите функцию Мёбиуса:}}\\\\";
        solvedWorkTasks += "\\Large{\\textbf{Вычислите функцию Мёбиуса:}}\\\\";
        ui->toolBar->actions().at(0)->setEnabled(true);
        ui->toolBar->actions().at(1)->setEnabled(true);
        ui->toolBar->actions().at(3)->setEnabled(true);
    }
}
void GeneratorWindow::slotDialogMebiusFunction(const int &countOfTasks, const int &minNum, const int &maxNum, const MebiusFunctionOptions &option){
    if (!countOfTasks) return;
    MebiusFunction task;
    switch (option) {
    case MebiusFunctionOptions::Default:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            task.setTask(random->bounded(minNum, maxNum));
            if (!mode) {
                tasksForWork += QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=~?\\\\";
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
                    tasksForWork += QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=~?\\\\";
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
                    tasksForWork += QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=~?\\\\";
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
                    tasksForWork += QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=~?\\\\";
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
                    tasksForWork += QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=~?\\\\";
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

void GeneratorWindow::slotDialogSymbolLegandreMeta(const int &countOfTasks){
    if (!countOfTasks) return;
    curTaskCount = 1;
    if (mode) {
        totalTestTasks += countOfTasks;
        statusBar()->showMessage("Задания на символ Лежандра сгенерированы!", 1500);
        ui->lcdNumber->display(totalTestTasks);
    } else {
        totalTaskCount = countOfTasks;
        tasksForWork += "\\Large{\\textbf{Вычислите символ Лежандра:}}\\\\";
        solvedWorkTasks += "\\Large{\\textbf{Вычислите символ Лежандра:}}\\\\";
        ui->toolBar->actions().at(0)->setEnabled(true);
        ui->toolBar->actions().at(1)->setEnabled(true);
        ui->toolBar->actions().at(3)->setEnabled(true);
    }
}
void GeneratorWindow::slotDialogSymbolLegandre(const int &countOfTasks, const std::pair<int, int> &a, const std::pair<int, int> &p, const SymbolLegandreOptions &option){
    if (!countOfTasks) return;
    SymbolLegandre task;
    switch (option) {
    case SymbolLegandreOptions::Default:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second)) {
                if (!mode) {
                    tasksForWork += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
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
                    tasksForWork += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
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
                    tasksForWork += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
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
                    tasksForWork += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
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
                    tasksForWork += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
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

void GeneratorWindow::slotDialogSymbolJacobiMeta(const int &countOfTasks){
    if (!countOfTasks) return;
    curTaskCount = 1;
    if (mode) {
        totalTestTasks += countOfTasks;
        statusBar()->showMessage("Задания на символ Якоби сгенерированы!", 1500);
        ui->lcdNumber->display(totalTestTasks);
    } else {
        totalTaskCount = countOfTasks;
        tasksForWork += "\\Large{\\textbf{Вычислите символ Якоби:}}\\\\";
        solvedWorkTasks += "\\Large{\\textbf{Вычислите символ Якоби:}}\\\\";
        ui->toolBar->actions().at(0)->setEnabled(true);
        ui->toolBar->actions().at(1)->setEnabled(true);
        ui->toolBar->actions().at(3)->setEnabled(true);
    }
}
void GeneratorWindow::slotDialogSymbolJacobi(const int &countOfTasks, const std::pair<int, int> &a, const std::pair<int, int> &p, const SymbolJacobiOptions &option){
    if (!countOfTasks) return;
    SymbolJacobi task;
    switch (option) {
    case SymbolJacobiOptions::Default:
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (!isPrime(task.getTask().second) && task.getTask().second % 2 != 0) {
                if (!mode) {
                    tasksForWork += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
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
                    tasksForWork += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
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
                    tasksForWork += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
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
                    tasksForWork += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
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
                    tasksForWork += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
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

void GeneratorWindow::slotDialogTranspositionGroupMeta(const int &countOfTasks){
    if (!countOfTasks) return;
    curTaskCount = 1;
    totalTaskCount = countOfTasks;
    if (mode) {
        totalTestTasks += countOfTasks;
        statusBar()->showMessage("Задачи на группу подстановок сгенерированы!", 1500);
        ui->lcdNumber->display(totalTestTasks);
    }
}
void GeneratorWindow::slotDialogTranspositionGroup(const int &countOfTasks, const int &minN, const int &maxN, const TranspositionGroupOptions &option, const ViewMode &mode){
    if (!countOfTasks) return;
    TranspositionGroup task, task2, result; int localCount = 1;
    ui->toolBar->actions().at(0)->setEnabled(true);
    ui->toolBar->actions().at(1)->setEnabled(true);
    ui->toolBar->actions().at(3)->setEnabled(true);
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
                    tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=~?\\\\";
                    solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=" + task.writeToMode(ViewMode::Cycle) + "\\\\";
                } else {
                    tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "=~?\\\\";
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
                    tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" +
                      task.writeToMode(ViewMode::Standart) + "·" + task2.writeToMode(ViewMode::Standart) + "=~?\\\\";
                    solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" +
                      task.writeToMode(ViewMode::Standart) + "·" + task2.writeToMode(ViewMode::Standart) + "=" + result.writeToMode(ViewMode::Standart) + "\\\\";
                } else {
                    tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" +
                      task.writeToMode(ViewMode::Cycle) + "·" + task2.writeToMode(ViewMode::Cycle) + "=~?\\\\";
                    solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" +
                      task.writeToMode(ViewMode::Cycle) + "·" + task2.writeToMode(ViewMode::Cycle) + "=" + result.writeToMode(ViewMode::Cycle) + "\\\\";
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
                tasksForWork += "Найдите~подстановку,~обратную~данной,~записанной~в~табличном~виде:\\\\";
                solvedWorkTasks += "Найдите~подстановку,~обратную~данной,~записанной~в~табличном~виде:\\\\";
            } else {
                tasksForWork += "Найдите~подстановку,~обратную~данной,~записанной~в~циклическом~виде:\\\\";
                solvedWorkTasks += "Найдите~подстановку,~обратную~данной,~записанной~в~циклическом~виде:\\\\";
            }
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode);
            result = ~task;
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:{" + task.writeToMode(ViewMode::Standart) + "}^{-1}=~?\\\\";
                    solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:{" + task.writeToMode(ViewMode::Standart) + "}^{-1}=" +
                      result.writeToMode(ViewMode::Standart) + "\\\\";
                } else {
                    tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:{" + task.writeToMode(ViewMode::Cycle) + "}^{-1}=~?\\\\";
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
                tasksForWork += "Найдите~цикловой~тип~подстановки,~записанной~в~табличном~виде:\\\\";
                solvedWorkTasks += "Найдите~цикловой~тип~подстановки,~записанной~в~табличном~виде:\\\\";
            } else {
                tasksForWork += "Найдите~цикловой~тип~подстановки,~записанной~в~циклическом~виде:\\\\";
                solvedWorkTasks += "Найдите~цикловой~тип~подстановки,~записанной~в~циклическом~виде:\\\\";
            }
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=~?\\\\";
                    solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=" +
                         task.cycleType() + "\\\\";
                } else {
                    tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "=~?\\\\";
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
                tasksForWork += "Вычислите~количество~беспорядков~подстановки,~записанной~в~табличном~виде:\\\\";
                solvedWorkTasks += "Вычислите~количество~беспорядков~подстановки,~записанной~в~табличном~виде:\\\\";
            } else {
                tasksForWork += "Вычислите~количество~беспорядков~подстановки,~записанной~в~циклическом~виде:\\\\";
                solvedWorkTasks += "Вычислите~количество~беспорядков~подстановки,~записанной~в~циклическом~виде:\\\\";
            }
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\Delta" + task.writeToMode(ViewMode::Standart) + "=~?\\\\";
                    solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\Delta" + task.writeToMode(ViewMode::Standart) + "=" +
                         QString::number(task.getHaos()) + "\\\\";
                } else {
                    tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\Delta" + task.writeToMode(ViewMode::Cycle) + "=~?\\\\";
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
                tasksForWork += "Определите~четность~подстановки,~записанной~в~табличном~виде:\\\\";
                solvedWorkTasks += "Определите~четность~подстановки,~записанной~в~табличном~виде:\\\\";
            } else {
                tasksForWork += "Определите~четность~подстановки,~записанной~в~циклическом~виде:\\\\";
                solvedWorkTasks += "Определите~четность~подстановки,~записанной~в~циклическом~виде:\\\\";
            }
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\delta" + task.writeToMode(ViewMode::Standart) + "\\Rightarrow~?\\\\";
                    solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\delta" + task.writeToMode(ViewMode::Standart) + "\\Rightarrow" +
                         task.getEven() + "\\\\";
                } else {
                    tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\delta" + task.writeToMode(ViewMode::Cycle) + "\\Rightarrow~?\\\\";
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
                tasksForWork += "Определите~порядок~подстановки,~записанной~в~табличном~виде:\\\\";
                solvedWorkTasks += "Определите~порядок~подстановки,~записанной~в~табличном~виде:\\\\";
            } else {
                tasksForWork += "Определите~порядок~подстановки,~записанной~в~циклическом~виде:\\\\";
                solvedWorkTasks += "Определите~порядок~подстановки,~записанной~в~циклическом~виде:\\\\";
            }
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:~ord" + task.writeToMode(ViewMode::Standart) + "=~?\\\\";
                    solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:~ord" + task.writeToMode(ViewMode::Standart) + "=" +
                         QString::number(task.getOrder()) + "\\\\";
                } else {
                    tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:~ord" + task.writeToMode(ViewMode::Cycle) + "=~?\\\\";
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
            tasksForWork += "Запишите~подстановку~в~виде~произведения~транспозиций:\\\\";
            solvedWorkTasks += "Запишите~подстановку~в~виде~произведения~транспозиций:\\\\";
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode, true);
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=~?\\\\";
                    solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=" + task.writeToMode(ViewMode::Decomposition) + "\\\\";
                } else {
                    tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "=~?\\\\";
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
            tasksForWork += "Запишите~подстановку~в~виде~произведения~транспозиций~соседних~элементов:\\\\";
            solvedWorkTasks += "Запишите~подстановку~в~виде~произведения~транспозиций~соседних~элементов:\\\\";
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks);) {
            task.setTask(random->bounded(minN, maxN), mode, true);
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=~?\\\\";
                    solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=" + task.writeToMode(ViewMode::Neighbors) + "\\\\";
                } else {
                    tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "=~?\\\\";
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

void GeneratorWindow::slotDialogSetMeta(const int &countOfTasks)
{
    if (!countOfTasks) return;
    curTaskCount = 1;
    totalTaskCount = countOfTasks;
    if (mode) {
        totalTestTasks += countOfTasks;
        statusBar()->showMessage("Задания на Множества сгенерированы!", 1500);
        ui->lcdNumber->display(totalTestTasks);
    }
}
void GeneratorWindow::slotDialogSet(const int &countOfTasks, const set_type &data, const SetOptions &option)
{
    int localCount = 1;
    ui->toolBar->actions().at(0)->setEnabled(true);
    ui->toolBar->actions().at(1)->setEnabled(true);
    ui->toolBar->actions().at(3)->setEnabled(true);
    switch (option) {
    case SetOptions::Check:
        if (!this->mode) {
            tasksForWork += "Чем~является~данная~Алгебраическая~Структура:\\\\";
            solvedWorkTasks += "Чем~является~данная~Алгебраическая~Структура:\\\\";
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            if (!mode) {
                tasksForWork += QString::number(localCount)  + ")~\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?\\\\";
                solvedWorkTasks += QString::number(localCount)  + ")~\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow" + std::get<2>(data[i]) + "\\\\";
                isReadyRender(); ++localCount;
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Чем~является~данная~Алгебраическая~Структура:\\\\\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, std::get<2>(data[i]), SupCommands::Name, 0));
            } ++curTaskCount;
        } break;

    case SetOptions::Oper:
        if (!this->mode) {
            tasksForWork += "Является~ли~данная~операция~заданной~на~множестве:\\\\";
            solvedWorkTasks += "Является~ли~данная~операция~заданной~на~множестве:\\\\";
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            QString answer;
            if (QString(std::get<2>(data[i])).toInt() / 1000)
                answer = "Да";
            else answer = "Нет";
            if (!mode) {
                tasksForWork += QString::number(localCount)  + ")~\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?\\\\";
                solvedWorkTasks += QString::number(localCount)  + ")~\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow" + answer + "\\\\";
                isReadyRender(); ++localCount;
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Чем~является~данная~Алгебраическая~Структура:\\\\\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, answer, SupCommands::Name, 0));
            } ++curTaskCount;
        } break;

    case SetOptions::Abel:
        if (!this->mode) {
            tasksForWork += "Является~ли~данная~операция~коммутативной~на~множестве:\\\\";
            solvedWorkTasks += "Является~ли~данная~операция~коммутативной~на~множестве:\\\\";
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            QString answer;
            if (QString(std::get<2>(data[i])).toInt() / 100)
                answer = "Да";
            else answer = "Нет";
            if (!mode) {
                tasksForWork += QString::number(localCount)  + ")~\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?\\\\";
                solvedWorkTasks += QString::number(localCount)  + ")~\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow" + answer + "\\\\";
                isReadyRender(); ++localCount;
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Чем~является~данная~Алгебраическая~Структура:\\\\\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, answer, SupCommands::Name, 0));
            } ++curTaskCount;
        } break;

    case SetOptions::Associate:
        if (!this->mode) {
            tasksForWork += "Является~ли~данная~операция~ассоциативной~на~множестве:\\\\";
            solvedWorkTasks += "Является~ли~данная~операция~ассоциативной~на~множестве:\\\\";
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            QString answer;
            if (QString(std::get<2>(data[i])).toInt() / 10)
                answer = "Да";
            else answer = "Нет";
            if (!mode) {
                tasksForWork += QString::number(localCount)  + ")~\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?\\\\";
                solvedWorkTasks += QString::number(localCount)  + ")~\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow" + answer + "\\\\";
                isReadyRender(); ++localCount;
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Чем~является~данная~Алгебраическая~Структура:\\\\\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, answer, SupCommands::Name, 0));
            } ++curTaskCount;
        } break;

    case SetOptions::Neutral:
        if (!this->mode) {
            tasksForWork += "Существует~ли~нейтральный~элемент~на~заданной~алгебраической~структуре:\\\\";
            solvedWorkTasks += "Существует~ли~нейтральный~элемент~на~заданной~алгебраической~структуре:\\\\";
        }
        for (size_t i = 0; i < static_cast<size_t>(countOfTasks); ++i) {
            QString answer;
            if (QString(std::get<2>(data[i])).toInt() % 10)
                answer = "Да";
            else answer = "Нет";
            if (!mode) {
                tasksForWork += QString::number(localCount)  + ")~\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?\\\\";
                solvedWorkTasks += QString::number(localCount)  + ")~\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow" + answer + "\\\\";
                isReadyRender(); ++localCount;
            } else {
                QString taskText = "\\begin{align}\\color{sienna}{Чем~является~данная~Алгебраическая~Структура:\\\\\\left(" + std::get<0>(data[i]) + "," + std::get<1>(data[i]) + "\\right)\\Rightarrow~?}\\end{align}";
                tasksForTest.push_back(std::make_tuple(taskText, answer, SupCommands::Name, 0));
            } ++curTaskCount;
        } break;
    }
}

void GeneratorWindow::on_comboBox_currentTextChanged(const QString &task)
{
    if (!ui->mainLayout->isEmpty())
        delete ui->mainLayout->takeAt(0)->widget();
    if (task == "Группа Перестановок") {
        DialogTranspositionGroup *window = new DialogTranspositionGroup(this, mode);
        connect(window, SIGNAL(dialogTranspositionGroupMeta(const int&)),
                this, SLOT(slotDialogTranspositionGroupMeta(const int&)));
        connect(window, SIGNAL(dialogTranspositionGroup(const int&, const int&, const int&, const TranspositionGroupOptions&, const ViewMode&)),
                this, SLOT(slotDialogTranspositionGroup(const int&, const int&, const int&, const TranspositionGroupOptions&, const ViewMode&)));
        ui->mainLayout->addWidget(window);
    } else if (task == "Функция Эйлера") {
        DialogEulerFunction *window = new DialogEulerFunction(this, mode);
        connect(window, SIGNAL(dialogEulerFunctionMeta(const int&)),
                this, SLOT(slotDialogEulerFunctionMeta(const int&)));
        connect(window, SIGNAL(dialogEulerFunction(const int&, const int&, const int&, const EulerFunctionOptions&)),
                this, SLOT(slotDialogEulerFunction(const int&, const int&, const int&, const EulerFunctionOptions&)));
        ui->mainLayout->addWidget(window);
    } else if (task == "Функция Мёбиуса") {
        DialogMebiusFunction *window = new DialogMebiusFunction(this, mode);
        connect(window, SIGNAL(dialogMebiusFunctionMeta(const int&)),
                this, SLOT(slotDialogMebiusFunctionMeta(const int&)));
        connect(window, SIGNAL(dialogMebiusFunction(const int&, const int&, const int&, const MebiusFunctionOptions&)),
                this, SLOT(slotDialogMebiusFunction(const int&, const int&, const int&, const MebiusFunctionOptions&)));
        ui->mainLayout->addWidget(window);
    } else if (task == "Символ Лежандра") {
        DialogSymbolLegandre *window = new DialogSymbolLegandre(this, mode);
        connect(window, SIGNAL(dialogSymbolLegandreMeta(const int&)),
                this, SLOT(slotDialogSymbolLegandreMeta(const int&)));
        connect(window, SIGNAL(dialogSymbolLegandre(const int&, const std::pair<int, int>&, const std::pair<int, int>&, const SymbolLegandreOptions&)),
                this, SLOT(slotDialogSymbolLegandre(const int&, const std::pair<int, int>&, const std::pair<int, int>&, const SymbolLegandreOptions&)));
        ui->mainLayout->addWidget(window);
    } else if (task == "Символ Якоби") {
        DialogSymbolJacobi *window = new DialogSymbolJacobi(this, mode);
        connect(window, SIGNAL(dialogSymbolJacobiMeta(const int&)),
                this, SLOT(slotDialogSymbolJacobiMeta(const int&)));
        connect(window, SIGNAL(dialogSymbolJacobi(const int&, const std::pair<int, int>&, const std::pair<int, int>&, const SymbolJacobiOptions&)),
                this, SLOT(slotDialogSymbolJacobi(const int&, const std::pair<int, int>&, const std::pair<int, int>&, const SymbolJacobiOptions&)));
        ui->mainLayout->addWidget(window);
    } else if (task == "Алгебраические Структуры") {
        DialogSet *window = new DialogSet(this, mode);
        connect(window, SIGNAL(dialogSetMeta(const int&)),
                this, SLOT(slotDialogSetMeta(const int&)));
        connect(window, SIGNAL(dialogSet(const int&, const set_type &, const SetOptions&)),
                this, SLOT(slotDialogSet(const int&, const set_type &, const SetOptions&)));
        ui->mainLayout->addWidget(window);
    }
}
