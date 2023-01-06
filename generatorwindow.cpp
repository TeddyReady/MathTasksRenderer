#include "generatorwindow.h"
#include "ui_generatorwindow.h"

GeneratorWindow::GeneratorWindow(QWidget *parent)
    : QMainWindow(parent), totalTestTasks(0),
      totalTaskCount(0), curTaskCount(0), mode(false),
      tasksForTest(new tasks_type),
      tasksForWork(new QString("\\color{sienna}{")),
      solvedWorkTasks(new QString("\\color{sienna}{")),
      random(QRandomGenerator::global()),
      ui(new Ui::GeneratorWindow)
{
    uploadSettings();
    uploadUI();

    connect(ui->toolBar->actions().at(0), SIGNAL(triggered()), this, SLOT(checkAnswers()));
    connect(ui->toolBar->actions().at(1), SIGNAL(triggered()), this, SLOT(clearTasks()));
    connect(ui->toolBar->actions().at(2), &QAction::triggered, [](){qApp->exit();});
}
GeneratorWindow::~GeneratorWindow()
{
    saveSettings();
    random = nullptr;
    delete engine;
    delete tasksForTest;
    delete tasksForWork;
    delete solvedWorkTasks;
    delete ui;
}

void GeneratorWindow::uploadUI()
{
    ui->setupUi(this);
    ui->genButton->setEnabled(true);
    ui->genButton->setCursor(Qt::PointingHandCursor);
    ui->pushButton->setCursor(Qt::PointingHandCursor);
    ui->webView->load(QUrl("qrc:/web/index.html"));
    ui->webView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->mainLayout->addWidget(new DialogTranspositionGroup(this, true));
    ui->tabWidget->tabBar()->setCursor(Qt::PointingHandCursor);
    ui->taskView->setCursor(Qt::BlankCursor);
    ui->taskView->setContextMenuPolicy(Qt::CustomContextMenu);
    engine = new TeXEngine(ui->taskView);
    ui->toolBar->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->toolBar->addAction(new QAction(QPixmap("://img/checkAnswers.png"), "Показать ответы", this));
    ui->toolBar->addAction(new QAction(QPixmap("://img/clearTasks.png"), "Очистить задачи", this));
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(new QAction(QPixmap("://img/quit.jpg"), "Выход...", this));
    ui->toolBar->actions().at(0)->setDisabled(true);
    ui->toolBar->actions().at(1)->setDisabled(true);

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
        engine->TeX2SVG(*tasksForWork + "}", true);
        totalTaskCount = 0;
        curTaskCount = 0;
        QApplication::restoreOverrideCursor();
    }
}
void GeneratorWindow::checkAnswers(){
    QApplication::setOverrideCursor(Qt::WaitCursor);
    engine->TeX2SVG(*solvedWorkTasks + "}", true);
    *tasksForWork = *solvedWorkTasks;
    ui->toolBar->actions().at(0)->setDisabled(true);
    QApplication::restoreOverrideCursor();
}
void GeneratorWindow::clearTasks()
{
    *tasksForWork = "\\color{sienna}{";
    *solvedWorkTasks = *tasksForWork;
    engine->TeX2SVG("\\color{sienna}{В~ожидании~генерации~задач...}", true);
    ui->toolBar->actions().at(0)->setDisabled(true);
    ui->toolBar->actions().at(1)->setDisabled(true);
}

void GeneratorWindow::on_tabWidget_currentChanged(int index)
{
    switch (index) {
    case 2:
        mode = 1;
        ui->toolBar->hide();
        break;
    default:
        mode = 0;
        if (index == 0) ui->toolBar->show();
        else ui->toolBar->hide();
        break;
    }
}

void GeneratorWindow::on_pushButton_clicked()
{
    if (tasksForTest->isEmpty())
    statusBar()->showMessage("Перед запуском теста, необходимо сгенерировать задания!", 1500);
    else {
        //Рандомная сортировка
        for (int i = 0; i < tasksForTest->size(); i++) {
            int j = random->bounded(0, tasksForTest->size());
            auto tmp = (*tasksForTest)[i];
            (*tasksForTest)[i] = (*tasksForTest)[j];
            (*tasksForTest)[j] = tmp;
        }
        TestMode *testWindow = new TestMode(this, tasksForTest);
        testWindow->setWindowTitle("Тест");
        testWindow->show();
        totalTestTasks = 0;
        ui->lcdNumber->display(totalTestTasks);
    }
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
        connect(window, SIGNAL(dialogEulerFunctionMeta(int)),
                this, SLOT(slotDialogEulerFunctionMeta(int)));
        connect(window, SIGNAL(dialogEulerFunction(int, int, int, EulerFunctionOptions)),
                this, SLOT(slotDialogEulerFunction(int, int, int, EulerFunctionOptions)));
    } else if (ui->tasksList->currentItem()->text() == "Функция Мёбиуса") {
        DialogMebiusFunction *window = new DialogMebiusFunction(this);
        window->setWindowTitle("Выберите настройки генерации");
        window->show();
        connect(window, SIGNAL(dialogMebiusFunctionMeta(int)),
                this, SLOT(slotDialogMebiusFunctionMeta(int)));
        connect(window, SIGNAL(dialogMebiusFunction(int, int, int, MebiusFunctionOptions)),
                this, SLOT(slotDialogMebiusFunction(int, int, int, MebiusFunctionOptions)));
    } else if (ui->tasksList->currentItem()->text() == "Символ Лежандра") {
        DialogSymbolLegandre *window = new DialogSymbolLegandre(this);
        window->setWindowTitle("Выберите настройки генерации");
        window->show();
        connect(window, SIGNAL(dialogSymbolLegandreMeta(int)),
                this, SLOT(slotDialogSymbolLegandreMeta(int)));
        connect(window, SIGNAL(dialogSymbolLegandre(int, std::pair<int, int>, std::pair<int, int>, SymbolLegandreOptions)),
                this, SLOT(slotDialogSymbolLegandre(int, std::pair<int, int>, std::pair<int, int>, SymbolLegandreOptions)));
    } else if (ui->tasksList->currentItem()->text() == "Символ Якоби") {
        DialogSymbolJacobi *window = new DialogSymbolJacobi(this);
        window->setWindowTitle("Выберите настройки генерации");
        window->show();
        connect(window, SIGNAL(dialogSymbolJacobiMeta(int)),
                this, SLOT(slotDialogSymbolJacobiMeta(int)));
        connect(window, SIGNAL(dialogSymbolJacobi(int, std::pair<int, int>, std::pair<int, int>, SymbolJacobiOptions)),
                this, SLOT(slotDialogSymbolJacobi(int, std::pair<int, int>, std::pair<int, int>, SymbolJacobiOptions)));
    } else if (ui->tasksList->currentItem()->text() == "Группа Перестановок") {
        DialogTranspositionGroup *window = new DialogTranspositionGroup(this);
        window->setWindowTitle("Выберите настройки генерации");
        window->show();
        connect(window, SIGNAL(dialogTranspositionGroupMeta(int)),
                this, SLOT(slotDialogTranspositionGroupMeta(int)));
        connect(window, SIGNAL(dialogTranspositionGroup(int, int, int, TranspositionGroupOptions, ViewMode)),
                this, SLOT(slotDialogTranspositionGroup(int, int, int, TranspositionGroupOptions, ViewMode)));
    }
}

void GeneratorWindow::slotDialogEulerFunctionMeta(int countOfTasks)
{
    if (countOfTasks > 0) {
        curTaskCount = 1;
        if (mode) {
            totalTestTasks += countOfTasks;
            statusBar()->showMessage("Задания на функцию Эйлера сгенерированы!", 1500);
            ui->lcdNumber->display(totalTestTasks);
        } else {
            totalTaskCount = countOfTasks;
            *tasksForWork += "Вычислите~функцию~Эйлера:\\\\";
            *solvedWorkTasks += "Вычислите~функцию~Эйлера:\\\\";
            ui->toolBar->actions().at(0)->setEnabled(true);
            ui->toolBar->actions().at(1)->setEnabled(true);
        }
    }
}
void GeneratorWindow::slotDialogEulerFunction(int countOfTasks, int minNum, int maxNum, EulerFunctionOptions option)
{
    EulerFunction task;
    switch (option) {
    case EulerFunctionOptions::Default:
        for (int i = 0; i < countOfTasks; i++) {
            task.setTask(random->bounded(minNum, maxNum));
            if (!mode) {
                *tasksForWork += QString::number(curTaskCount)  + ")~φ(" + QString::number(task.getTask()) + ")=~?\\\\";
                *solvedWorkTasks += QString::number(curTaskCount)  + ")~φ(" + QString::number(task.getTask()) + ")=" + QString::number(task.solve()) + "\\\\";
                isReadyRender();
            } else {
                QString taskText = "\\color{sienna}{Вычислите~функцию~Эйлера:\\\\φ(" + QString::number(task.getTask()) + ")=~?}";
                tasksForTest->push_back(std::make_pair(std::make_pair(taskText, QString::number(task.solve())), SupCommands::None));
            } curTaskCount++;
        } break;
    case EulerFunctionOptions::Primes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minNum, maxNum));
            if (isPrime(task.getTask())) {
                if (!mode) {
                    *tasksForWork += QString::number(curTaskCount)  + ")~φ(" + QString::number(task.getTask()) + ")=~?\\\\";
                    *solvedWorkTasks += QString::number(curTaskCount)  + ")~φ(" + QString::number(task.getTask()) + ")=" + QString::number(task.solve()) + "\\\\";
                    isReadyRender();
                } else {
                    QString taskText = "\\color{sienna}{Вычислите~функцию~Эйлера:\\\\φ(" + QString::number(task.getTask()) + ")=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, QString::number(task.solve())), SupCommands::None));
                } i++; curTaskCount++;
            }
        } break;
    case EulerFunctionOptions::PrimesDegrees:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minNum, maxNum));
            auto data = decompositionToSimple(task.getTask());
            if (data.size() == 1 && data[0].second > 1) {
                if (!mode) {
                    *tasksForWork += QString::number(curTaskCount)  + ")~φ(" + QString::number(task.getTask()) + ")=~?\\\\";
                    *solvedWorkTasks += QString::number(curTaskCount)  + ")~φ(" + QString::number(task.getTask()) + ")=" + QString::number(task.solve()) + "\\\\";
                    isReadyRender();
                } else {
                    QString taskText = "\\color{sienna}{Вычислите~функцию~Эйлера:\\\\φ(" + QString::number(task.getTask()) + ")=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, QString::number(task.solve())), SupCommands::None));
                } i++; curTaskCount++;
            }
        } break;
    case EulerFunctionOptions::MultiplyPrimes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minNum, maxNum));
            auto data = decompositionToSimple(task.getTask());
            if (data.size() == 1) continue;
            bool accessFlag = true;
            for (int i = 0; i < data.size(); i++){
                if (data[i].second != 1) {
                    accessFlag = false;
                    break;
                }
            } if (accessFlag) {
                if (!mode) {
                    *tasksForWork += QString::number(curTaskCount)  + ")~φ(" + QString::number(task.getTask()) + ")=~?\\\\";
                    *solvedWorkTasks += QString::number(curTaskCount)  + ")~φ(" + QString::number(task.getTask()) + ")=" + QString::number(task.solve()) + "\\\\";
                    isReadyRender();
                } else {
                    QString taskText = "\\color{sienna}{Вычислите~функцию~Эйлера:\\\\φ(" + QString::number(task.getTask()) + ")=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, QString::number(task.solve())), SupCommands::None));
                } i++; curTaskCount++;
            }
        } break;
    case EulerFunctionOptions::MultiplyPrimesDegrees:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minNum, maxNum));
            auto data = decompositionToSimple(task.getTask());
            if (data.size() == 1) continue;
            bool accessFlag = true;
            for (int i = 0; i < data.size(); i++){
                if (data[i].second == 1) {
                    accessFlag = false;
                    break;
                }
            } if (accessFlag) {
                if (!mode) {
                    *tasksForWork += QString::number(curTaskCount)  + ")~φ(" + QString::number(task.getTask()) + ")=~?\\\\";
                    *solvedWorkTasks += QString::number(curTaskCount)  + ")~φ(" + QString::number(task.getTask()) + ")=" + QString::number(task.solve()) + "\\\\";
                    isReadyRender();
                } else {
                    QString taskText = "\\color{sienna}{Вычислите~функцию~Эйлера:\\\\φ(" + QString::number(task.getTask()) + ")=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, QString::number(task.solve())), SupCommands::None));
                } i++; curTaskCount++;
            }
        } break;
    }
}

void GeneratorWindow::slotDialogMebiusFunctionMeta(int countOfTasks){
    curTaskCount = 1;
    if (mode) {
        totalTestTasks += countOfTasks;
        statusBar()->showMessage("Задания на функцию Мёбиуса сгенерированы!", 1500);
        ui->lcdNumber->display(totalTestTasks);
    } else {
        totalTaskCount = countOfTasks;
        *tasksForWork += "Вычислите~функцию~Мёбиуса:\\\\";
        *solvedWorkTasks += "Вычислите~функцию~Мёбиуса:\\\\";
        ui->toolBar->actions().at(0)->setEnabled(true);
        ui->toolBar->actions().at(1)->setEnabled(true);
    }
}
void GeneratorWindow::slotDialogMebiusFunction(int countOfTasks, int minNum, int maxNum, MebiusFunctionOptions option){
    MebiusFunction task;
    switch (option) {
    case MebiusFunctionOptions::Default:
        for (int i = 0; i < countOfTasks; i++) {
            task.setTask(random->bounded(minNum, maxNum));
            if (!mode) {
                *tasksForWork += QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=~?\\\\";
                *solvedWorkTasks += QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=" + QString::number(task.solve()) + "\\\\";
                isReadyRender();
            } else {
                QString taskText = "\\color{sienna}{Вычислите~функцию~Мёбиуса:\\\\\\mu(" + QString::number(task.getTask()) + ")=~?}";
                tasksForTest->push_back(std::make_pair(std::make_pair(taskText, QString::number(task.solve())), SupCommands::None));
            } curTaskCount++;
        } break;
    case MebiusFunctionOptions::Primes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minNum, maxNum));
            if (!isPrime(task.getTask())) {
                if (!mode) {
                    *tasksForWork += QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=~?\\\\";
                    *solvedWorkTasks += QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=" + QString::number(task.solve()) + "\\\\";
                    isReadyRender();
                } else {
                    QString taskText = "\\color{sienna}{Вычислите~функцию~Мёбиуса:\\\\\\mu(" + QString::number(task.getTask()) + ")=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, QString::number(task.solve())), SupCommands::None));
                } i++; curTaskCount++;
            }
        } break;
    case MebiusFunctionOptions::EvenPrimes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minNum, maxNum));
            if (isPrime(task.getTask())) continue;
            auto data = decompositionToSimple(task.getTask());
            if (data.size() % 2 != 0) continue;
            bool accessFlag = true;
            for (int i = 0; i < data.size(); i++){
                if (data[i].second > 1) {
                    accessFlag = false;
                    break;
                }
            }
            if (accessFlag) {
                if (!mode) {
                    *tasksForWork += QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=~?\\\\";
                    *solvedWorkTasks += QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=" + QString::number(task.solve()) + "\\\\";
                    isReadyRender();
                } else {
                    QString taskText = "\\color{sienna}{Вычислите~функцию~Мёбиуса:\\\\\\mu(" + QString::number(task.getTask()) + ")=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, QString::number(task.solve())), SupCommands::None));
                } i++; curTaskCount++;
            }
        } break;
    case MebiusFunctionOptions::NotEvenPrimes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minNum, maxNum));
            if (isPrime(task.getTask())) continue;
            auto data = decompositionToSimple(task.getTask());
            if (data.size() % 2 == 0) continue;
            bool accessFlag = true;
            for (int i = 0; i < data.size(); i++){
                if (data[i].second > 1) {
                    accessFlag = false;
                    break;
                }
            }
            if (accessFlag) {
                if (!mode) {
                    *tasksForWork += QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=~?\\\\";
                    *solvedWorkTasks += QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=" + QString::number(task.solve()) + "\\\\";
                    isReadyRender();
                } else {
                    QString taskText = "\\color{sienna}{Вычислите~функцию~Мёбиуса:\\\\\\mu(" + QString::number(task.getTask()) + ")=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, QString::number(task.solve())), SupCommands::None));
                } i++; curTaskCount++;
            }
        } break;
    case MebiusFunctionOptions::PrimeDegree:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minNum, maxNum));
            auto data = decompositionToSimple(task.getTask());
            bool accessFlag = false;
            for (int i = 0; i < data.size(); i++) {
                if (data[i].second > 1) {
                    accessFlag = true;
                    break;
                }
            }
            if (accessFlag) {
                if (!mode) {
                    *tasksForWork += QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=~?\\\\";
                    *solvedWorkTasks += QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=" + QString::number(task.solve()) + "\\\\";
                    isReadyRender();
                } else {
                    QString taskText = "\\color{sienna}{Вычислите~функцию~Мёбиуса:\\\\\\mu(" + QString::number(task.getTask()) + ")=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, QString::number(task.solve())), SupCommands::None));
                } i++; curTaskCount++;
            }
        } break;
    }
}

void GeneratorWindow::slotDialogSymbolLegandreMeta(int countOfTasks){
    curTaskCount = 1;
    if (mode) {
        totalTestTasks += countOfTasks;
        statusBar()->showMessage("Задания на символ Лежандра сгенерированы!", 1500);
        ui->lcdNumber->display(totalTestTasks);
    } else {
        totalTaskCount = countOfTasks;
        *tasksForWork += "Вычислите~символ~Лежандра:\\\\";
        *solvedWorkTasks += "Вычислите~символ~Лежандра:\\\\";
        ui->toolBar->actions().at(0)->setEnabled(true);
        ui->toolBar->actions().at(1)->setEnabled(true);
    }
}
void GeneratorWindow::slotDialogSymbolLegandre(int countOfTasks, std::pair<int, int> a, std::pair<int, int> p, SymbolLegandreOptions option){
    SymbolLegandre task;
    switch (option) {
    case SymbolLegandreOptions::Default:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second)) {
                if (!mode) {
                    *tasksForWork += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                       QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                       "}\\right)=~?\\\\";
                    *solvedWorkTasks += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                       QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                       "}\\right)=" + QString::number(task.solve()) + "\\\\"; isReadyRender();
                } else {
                    QString taskText = "\\color{sienna}{Вычислите~символ~Лежандра:\\\\\\left(\\frac{" +
                      QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, QString::number(task.solve())), SupCommands::None));
                } i++; curTaskCount++;
            }
        } break;
    case SymbolLegandreOptions::Primes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second) && !isPrime(task.getTask().first)) {
                if (!mode) {
                    *tasksForWork += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                       QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                       "}\\right)=~?\\\\";
                    *solvedWorkTasks += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                       QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                       "}\\right)=" + QString::number(task.solve()) + "\\\\"; isReadyRender();
                } else {
                    QString taskText = "\\color{sienna}{Вычислите~символ~Лежандра:\\\\\\left(\\frac{" +
                      QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, QString::number(task.solve())), SupCommands::None));
                } i++; curTaskCount++;
            }
        } break;
    case SymbolLegandreOptions::aEqual_1:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second)) {
                if (!mode) {
                    *tasksForWork += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                       QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                       "}\\right)=~?\\\\";
                    *solvedWorkTasks += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                       QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                       "}\\right)=" + QString::number(task.solve()) + "\\\\"; isReadyRender();
                } else {
                    QString taskText = "\\color{sienna}{Вычислите~символ~Лежандра:\\\\\\left(\\frac{" +
                      QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, QString::number(task.solve())), SupCommands::None));
                } i++; curTaskCount++;
            }
        } break;
    case SymbolLegandreOptions::aEqual2:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second)) {
                if (!mode) {
                    *tasksForWork += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                       QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                       "}\\right)=~?\\\\";
                    *solvedWorkTasks += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                       QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                       "}\\right)=" + QString::number(task.solve()) + "\\\\"; isReadyRender();
                } else {
                    QString taskText = "\\color{sienna}{Вычислите~символ~Лежандра:\\\\\\left(\\frac{" +
                      QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, QString::number(task.solve())), SupCommands::None));
                } i++; curTaskCount++;
            }
        } break;
    case SymbolLegandreOptions::NotEvenPrimes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second) && !isPrime(task.getTask().first) && task.getTask().first % 2 != 0) {
                if (!mode) {
                    *tasksForWork += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                       QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                       "}\\right)=~?\\\\";
                    *solvedWorkTasks += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                       QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                       "}\\right)=" + QString::number(task.solve()) + "\\\\"; isReadyRender();
                } else {
                    QString taskText = "\\color{sienna}{Вычислите~символ~Лежандра:\\\\\\left(\\frac{" +
                      QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, QString::number(task.solve())), SupCommands::None));
                } i++; curTaskCount++;
            }
        } break;
    }
}

void GeneratorWindow::slotDialogSymbolJacobiMeta(int countOfTasks){
    curTaskCount = 1;
    if (mode) {
        totalTestTasks += countOfTasks;
        statusBar()->showMessage("Задания на символ Якоби сгенерированы!", 1500);
        ui->lcdNumber->display(totalTestTasks);
    } else {
        totalTaskCount = countOfTasks;
        *tasksForWork += "Вычислите~символ~Якоби:\\\\";
        *solvedWorkTasks += "Вычислите~символ~Якоби:\\\\";
        ui->toolBar->actions().at(0)->setEnabled(true);
        ui->toolBar->actions().at(1)->setEnabled(true);
    }
}
void GeneratorWindow::slotDialogSymbolJacobi(int countOfTasks, std::pair<int, int> a, std::pair<int, int> p, SymbolJacobiOptions option){
    SymbolJacobi task;
    switch (option) {
    case SymbolJacobiOptions::Default:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (!isPrime(task.getTask().second) && task.getTask().second % 2 != 0) {
                if (!mode) {
                    *tasksForWork += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                       QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                       "}\\right)=~?\\\\";
                    *solvedWorkTasks += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                       QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                       "}\\right)=" + QString::number(task.solve()) + "\\\\"; isReadyRender();
                } else {
                    QString taskText = "\\color{sienna}{Вычислите~символ~Якоби:\\\\\\left(\\frac{" +
                      QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, QString::number(task.solve())), SupCommands::None));
                } i++; curTaskCount++;
            }
        } break;
    case SymbolJacobiOptions::Primes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (!isPrime(task.getTask().second) && !isPrime(task.getTask().first) && task.getTask().second % 2 != 0) {
                if (!mode) {
                    *tasksForWork += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                       QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                       "}\\right)=~?\\\\";
                    *solvedWorkTasks += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                       QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                       "}\\right)=" + QString::number(task.solve()) + "\\\\"; isReadyRender();
                } else {
                    QString taskText = "\\color{sienna}{Вычислите~символ~Якоби:\\\\\\left(\\frac{" +
                      QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, QString::number(task.solve())), SupCommands::None));
                } i++; curTaskCount++;
            }
        } break;
    case SymbolJacobiOptions::aEqual_1:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (!isPrime(task.getTask().second) && task.getTask().second % 2 != 0) {
                if (!mode) {
                    *tasksForWork += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                       QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                       "}\\right)=~?\\\\";
                    *solvedWorkTasks += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                       QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                       "}\\right)=" + QString::number(task.solve()) + "\\\\"; isReadyRender();
                } else {
                    QString taskText = "\\color{sienna}{Вычислите~символ~Якоби:\\\\\\left(\\frac{" +
                      QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, QString::number(task.solve())), SupCommands::None));
                } i++; curTaskCount++;
            }
        } break;
    case SymbolJacobiOptions::aEqual2:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (!isPrime(task.getTask().second) && task.getTask().second % 2 != 0) {
                if (!mode) {
                    *tasksForWork += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                       QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                       "}\\right)=~?\\\\";
                    *solvedWorkTasks += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                       QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                       "}\\right)=" + QString::number(task.solve()) + "\\\\"; isReadyRender();
                } else {
                    QString taskText = "\\color{sienna}{Вычислите~символ~Якоби:\\\\\\left(\\frac{" +
                      QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, QString::number(task.solve())), SupCommands::None));
                } i++; curTaskCount++;
            }
        } break;
    case SymbolJacobiOptions::NotEvenPrimes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (!isPrime(task.getTask().second) && !isPrime(task.getTask().first) && task.getTask().first % 2 != 0 && task.getTask().second % 2 != 0) {
                if (!mode) {
                    *tasksForWork += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                       QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                       "}\\right)=~?\\\\";
                    *solvedWorkTasks += QString::number(curTaskCount)  + ")~\\left(\\frac{" +
                       QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) +
                       "}\\right)=" + QString::number(task.solve()) + "\\\\"; isReadyRender();
                } else {
                    QString taskText = "\\color{sienna}{Вычислите~символ~Якоби:\\\\\\left(\\frac{" +
                      QString::number(task.getTask().first) + "}{" + QString::number(task.getTask().second) + "}\\right)=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, QString::number(task.solve())), SupCommands::None));
                } i++; curTaskCount++;
            }
        } break;
    }
}

void GeneratorWindow::slotDialogTranspositionGroupMeta(int countOfTasks){
    curTaskCount = 1;
    totalTaskCount = countOfTasks;
    if (mode) {
        totalTestTasks += countOfTasks;
        statusBar()->showMessage("Задачи на группу подстановок сгенерированы!", 1500);
        ui->lcdNumber->display(totalTestTasks);
    }
}
void GeneratorWindow::slotDialogTranspositionGroup(int countOfTasks, int minN, int maxN, TranspositionGroupOptions option, ViewMode mode){
    TranspositionGroup task, task2, result; int localCount;
    if (!countOfTasks) return;
    localCount = 1;
    ui->toolBar->actions().at(0)->setEnabled(true);
    ui->toolBar->actions().at(1)->setEnabled(true);
    switch (option) {
    case TranspositionGroupOptions::Write:
        if (!this->mode) {
            if (mode == ViewMode::Standart) {
                *tasksForWork += "Запишите~подстановку~в~виде~произведения~независимых~циклов:\\\\";
                *solvedWorkTasks += "Запишите~подстановку~в~виде~произведения~независимых~циклов:\\\\";
            } else {
                *tasksForWork += "Запишите~подстановку~в~табличном~виде:\\\\";
                *solvedWorkTasks += "Запишите~подстановку~в~табличном~виде:\\\\";
            }
        }
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    *tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=~?\\\\";
                    *solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=" + task.writeToMode(ViewMode::Cycle) + "\\\\";
                } else {
                    *tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "=~?\\\\";
                    *solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "=" + task.writeToMode(ViewMode::Standart) + "\\\\";
                } isReadyRender(); localCount++;
            } else {
                if (task.getViewMode() == ViewMode::Standart) {
                    QString taskText = "\\color{sienna}{Запишите~подстановку~в~виде~произведения~независимых~циклов.\\\\S_{" +
                      QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, task.writeToMode(ViewMode::Cycle)), SupCommands::Transposition));
                } else {
                    QString taskText = "\\color{sienna}{Запишите~подстановку~в~табличном~виде.\\\\В~ответе~укажите~нижнюю~строку~получившейся~подстановки.\\\\S_{" +
                      QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, task.writeToMode(ViewMode::Standart)), SupCommands::Transposition));
                }
            } i++; curTaskCount++;
        } break;

    case TranspositionGroupOptions::Multiply:
        if (!this->mode) {
            if (mode == ViewMode::Standart) {
                *tasksForWork += "Найдите~произведение~подстановок,~записанных~в~табличном~виде:\\\\";
                *solvedWorkTasks += "Найдите~произведение~подстановок,~записанных~в~табличном~виде:\\\\";
            } else {
                *tasksForWork += "Найдите~произведение~подстановок,~записанных~в~циклическом~виде:\\\\";
                *solvedWorkTasks += "Найдите~произведение~подстановок,~записанных~в~циклическом~виде:\\\\";
            }
        }
        for (int i = 0; i < countOfTasks;) {
            int n = static_cast<int>(random->bounded(minN, maxN));
            task.setTask(n, mode);
            task2.setTask(n, mode);
            result = task * task2;
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    *tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" +
                      task.writeToMode(ViewMode::Standart) + "·" + task2.writeToMode(ViewMode::Standart) + "=~?\\\\";
                    *solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" +
                      task.writeToMode(ViewMode::Standart) + "·" + task2.writeToMode(ViewMode::Standart) + "=" + result.writeToMode(ViewMode::Standart) + "\\\\";
                } else {
                    *tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" +
                      task.writeToMode(ViewMode::Cycle) + "·" + task2.writeToMode(ViewMode::Cycle) + "=~?\\\\";
                    *solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" +
                      task.writeToMode(ViewMode::Cycle) + "·" + task2.writeToMode(ViewMode::Cycle) + "=" + result.writeToMode(ViewMode::Cycle) + "\\\\";
                } isReadyRender(); localCount++;
            } else {
                if (task.getViewMode() == ViewMode::Standart) {
                    QString taskText = "\\color{sienna}{Найдите~произведение~подстановок,~записанных~в~табличном~виде.\\\\В~ответе~укажите~нижнюю~строку~получившейся~подстановки.\\\\S_{" +
                      QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "·" + task2.writeToMode(ViewMode::Standart) + "=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, result.writeToMode(ViewMode::Standart)), SupCommands::Transposition));
                } else {
                    QString taskText = "\\color{sienna}{Найдите~произведение~подстановок,~записанных~в~циклическом~виде.\\\\S_{" +
                      QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "·" + task2.writeToMode(ViewMode::Cycle) + "=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, result.writeToMode(ViewMode::Cycle)), SupCommands::Transposition));
                }
            } i++; curTaskCount++;
        } break;

    case TranspositionGroupOptions::Inverse:
        if (!this->mode) {
            if (mode == ViewMode::Standart) {
                *tasksForWork += "Найдите~подстановку,~обратную~данной,~записанной~в~табличном~виде:\\\\";
                *solvedWorkTasks += "Найдите~подстановку,~обратную~данной,~записанной~в~табличном~виде:\\\\";
            } else {
                *tasksForWork += "Найдите~подстановку,~обратную~данной,~записанной~в~циклическом~виде:\\\\";
                *solvedWorkTasks += "Найдите~подстановку,~обратную~данной,~записанной~в~циклическом~виде:\\\\";
            }
        }
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minN, maxN), mode);
            result = ~task;
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    *tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\overline{" + task.writeToMode(ViewMode::Standart) + "}=~?\\\\";
                    *solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\overline{" + task.writeToMode(ViewMode::Standart) + "}=" +
                      result.writeToMode(ViewMode::Standart) + "\\\\";
                } else {
                    *tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\overline{" + task.writeToMode(ViewMode::Cycle) + "}=~?\\\\";
                    *solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\overline{" + task.writeToMode(ViewMode::Cycle) + "}=" +
                      result.writeToMode(ViewMode::Cycle) + "\\\\";
                } isReadyRender(); localCount++;
            } else {
                if (task.getViewMode() == ViewMode::Standart) {
                    QString taskText = "\\color{sienna}{Найдите~подстановку,~обратную~данной,~записанной~в~табличном~виде.\\\\В~ответе~укажите~нижнюю~строку~получившейся~подстановки.\\\\S_{" +
                      QString::number(task.getTask()) + "}:\\overline{" + task.writeToMode(ViewMode::Standart) + "}=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, result.writeToMode(ViewMode::Standart)), SupCommands::Transposition));
                } else {
                    QString taskText = "\\color{sienna}{Найдите~подстановку,~обратную~данной,~записанной~в~циклическом~виде.\\\\S_{" +
                      QString::number(task.getTask()) + "}:\\overline{" + task.writeToMode(ViewMode::Cycle) + "}=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, result.writeToMode(ViewMode::Cycle)), SupCommands::Transposition));
                }
            } i++; curTaskCount++;
        } break;

    case TranspositionGroupOptions::Cycle:
        if (!this->mode) {
            if (mode == ViewMode::Standart) {
                *tasksForWork += "Найдите~цикловой~тип~подстановки,~записанной~в~табличном~виде:\\\\";
                *solvedWorkTasks += "Найдите~цикловой~тип~подстановки,~записанной~в~табличном~виде:\\\\";
            } else {
                *tasksForWork += "Найдите~цикловой~тип~подстановки,~записанной~в~циклическом~виде:\\\\";
                *solvedWorkTasks += "Найдите~цикловой~тип~подстановки,~записанной~в~циклическом~виде:\\\\";
            }
        }
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    *tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=~?\\\\";
                    *solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=" +
                         task.cycleType() + "\\\\";
                } else {
                    *tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "=~?\\\\";
                    *solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "=" +
                         task.cycleType() + "\\\\";
                } isReadyRender(); localCount++;
            } else {
                if (task.getViewMode() == ViewMode::Standart) {
                    QString taskText = "\\color{sienna}{Найдите~цикловой~тип~подстановки,~записанной~в~табличном~виде.\\\\Ответ~дайте~в~формате~[1^i,2^j,...,n^k],~где~i,j,k\\neq0.\\\\S_{" +
                      QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, task.cycleType()), SupCommands::None));
                } else {
                    QString taskText = "\\color{sienna}{Найдите~цикловой~тип~подстановки,~записанной~в~циклическом~виде.\\\\Ответ~дайте~в~формате~[1^i,2^j,...,n^k],~где~i,j,k\\neq0.\\\\S_{" +
                      QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, task.cycleType()), SupCommands::None));
                }
            } i++; curTaskCount++;
        } break;

    case TranspositionGroupOptions::Count:
        if (!this->mode) {
            if (mode == ViewMode::Standart) {
                *tasksForWork += "Вычислите~количество~беспорядков~подстановки,~записанной~в~табличном~виде:\\\\";
                *solvedWorkTasks += "Вычислите~количество~беспорядков~подстановки,~записанной~в~табличном~виде:\\\\";
            } else {
                *tasksForWork += "Вычислите~количество~беспорядков~подстановки,~записанной~в~циклическом~виде:\\\\";
                *solvedWorkTasks += "Вычислите~количество~беспорядков~подстановки,~записанной~в~циклическом~виде:\\\\";
            }
        }
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    *tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\Delta" + task.writeToMode(ViewMode::Standart) + "=~?\\\\";
                    *solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\Delta" + task.writeToMode(ViewMode::Standart) + "=" +
                         QString::number(task.getHaos()) + "\\\\";
                } else {
                    *tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\Delta" + task.writeToMode(ViewMode::Cycle) + "=~?\\\\";
                    *solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\Delta" + task.writeToMode(ViewMode::Cycle) + "=" +
                         QString::number(task.getHaos()) + "\\\\";
                } isReadyRender(); localCount++;
            } else {
                if (task.getViewMode() == ViewMode::Standart) {
                    QString taskText = "\\color{sienna}{Вычислите~количество~беспорядков~подстановки,~записанной~в~табличном~виде.\\\\S_{" +
                      QString::number(task.getTask()) + "}:\\Delta" + task.writeToMode(ViewMode::Standart) + "=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, QString::number(task.getHaos())), SupCommands::None));
                } else {
                    QString taskText = "\\color{sienna}{Вычислите~количество~беспорядков~подстановки,~записанной~в~циклическом~виде.\\\\S_{" +
                      QString::number(task.getTask()) + "}:\\Delta" + task.writeToMode(ViewMode::Cycle) + "=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, QString::number(task.getHaos())), SupCommands::None));
                }
            } i++; curTaskCount++;
        } break;

    case TranspositionGroupOptions::Mod2:
        if (!this->mode) {
            if (mode == ViewMode::Standart && this->mode) {
                *tasksForWork += "Определите~четность~подстановки,~записанной~в~табличном~виде:\\\\";
                *solvedWorkTasks += "Определите~четность~подстановки,~записанной~в~табличном~виде:\\\\";
            } else {
                *tasksForWork += "Определите~четность~подстановки,~записанной~в~циклическом~виде:\\\\";
                *solvedWorkTasks += "Определите~четность~подстановки,~записанной~в~циклическом~виде:\\\\";
            }
        }
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    *tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\delta" + task.writeToMode(ViewMode::Standart) + "\\Rightarrow~?\\\\";
                    *solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\delta" + task.writeToMode(ViewMode::Standart) + "\\Rightarrow" +
                         task.getEven() + "\\\\";
                } else {
                    *tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\delta" + task.writeToMode(ViewMode::Cycle) + "\\Rightarrow~?\\\\";
                    *solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\delta" + task.writeToMode(ViewMode::Cycle) + "\\Rightarrow" +
                        task.getEven() + "\\\\";
                } isReadyRender(); localCount++;
            } else {
                if (task.getViewMode() == ViewMode::Standart) {
                    QString taskText = "\\color{sienna}{Определите~четность~подстановки,~записанной~в~табличном~виде.\\\\В~ответе~укажите~Ч,~если~подстановка~четная,~и~Н,~в~противном~случае.\\\\S_{" +
                      QString::number(task.getTask()) + "}:\\delta" + task.writeToMode(ViewMode::Standart) + "=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, task.getEven()), SupCommands::None));
                } else {
                    QString taskText = "\\color{sienna}{Определите~четность~подстановки,~записанной~в~циклическом~виде.\\\\В~ответе~укажите~Ч,~если~подстановка~четная,~и~Н,~в~противном~случае.\\\\S_{" +
                      QString::number(task.getTask()) + "}:\\delta" + task.writeToMode(ViewMode::Cycle) + "=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, task.getEven()), SupCommands::None));
                }
            } i++; curTaskCount++;
        } break;

    case TranspositionGroupOptions::Order:
        if (!this->mode) {
            if (mode == ViewMode::Standart) {
                *tasksForWork += "Определите~порядок~подстановки,~записанной~в~табличном~виде:\\\\";
                *solvedWorkTasks += "Определите~порядок~подстановки,~записанной~в~табличном~виде:\\\\";
            } else {
                *tasksForWork += "Определите~порядок~подстановки,~записанной~в~циклическом~виде:\\\\";
                *solvedWorkTasks += "Определите~порядок~подстановки,~записанной~в~циклическом~виде:\\\\";
            }
        }
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart) {
                    *tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:~ord" + task.writeToMode(ViewMode::Standart) + "=~?\\\\";
                    *solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:~ord" + task.writeToMode(ViewMode::Standart) + "=" +
                         QString::number(task.getOrder()) + "\\\\";
                } else {
                    *tasksForWork += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:~ord" + task.writeToMode(ViewMode::Cycle) + "=~?\\\\";
                    *solvedWorkTasks += QString::number(localCount) + ")~S_{" + QString::number(task.getTask()) + "}:~ord" + task.writeToMode(ViewMode::Cycle) + "=" +
                        QString::number(task.getOrder()) + "\\\\";
                } isReadyRender(); localCount++;
            } else {
                if (task.getViewMode() == ViewMode::Standart) {
                    QString taskText = "\\color{sienna}{Определите~порядок~подстановки,~записанной~в~табличном~виде.\\\\S_{" +
                      QString::number(task.getTask()) + "}:~ord" + task.writeToMode(ViewMode::Standart) + "=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, QString::number(task.getOrder())), SupCommands::None));
                } else {
                    QString taskText = "\\color{sienna}{Определите~порядок~подстановки,~записанной~в~циклическом~виде.\\\\S_{" +
                      QString::number(task.getTask()) + "}:~ord" + task.writeToMode(ViewMode::Cycle) + "=~?}";
                    tasksForTest->push_back(std::make_pair(std::make_pair(taskText, QString::number(task.getOrder())), SupCommands::None));
                }
            } i++; curTaskCount++;
        } break;
    }
}

void GeneratorWindow::on_comboBox_currentTextChanged(const QString &task)
{
    delete ui->mainLayout->takeAt(0)->widget();
    if (task == "Группа Перестановок") {
        DialogTranspositionGroup *window = new DialogTranspositionGroup(this, mode);
        connect(window, SIGNAL(dialogTranspositionGroupMeta(int)),
                this, SLOT(slotDialogTranspositionGroupMeta(int)));
        connect(window, SIGNAL(dialogTranspositionGroup(int, int, int, TranspositionGroupOptions, ViewMode)),
                this, SLOT(slotDialogTranspositionGroup(int, int, int, TranspositionGroupOptions, ViewMode)));
        ui->mainLayout->addWidget(window);
    } else if (task == "Функция Эйлера") {
        DialogEulerFunction *window = new DialogEulerFunction(this, mode);
        connect(window, SIGNAL(dialogEulerFunctionMeta(int)),
                this, SLOT(slotDialogEulerFunctionMeta(int)));
        connect(window, SIGNAL(dialogEulerFunction(int, int, int, EulerFunctionOptions)),
                this, SLOT(slotDialogEulerFunction(int, int, int, EulerFunctionOptions)));
        ui->mainLayout->addWidget(window);
    } else if (task == "Функция Мёбиуса") {
        DialogMebiusFunction *window = new DialogMebiusFunction(this, mode);
        connect(window, SIGNAL(dialogMebiusFunctionMeta(int)),
                this, SLOT(slotDialogMebiusFunctionMeta(int)));
        connect(window, SIGNAL(dialogMebiusFunction(int, int, int, MebiusFunctionOptions)),
                this, SLOT(slotDialogMebiusFunction(int, int, int, MebiusFunctionOptions)));
        ui->mainLayout->addWidget(window);
    } else if (task == "Символ Лежандра") {
        DialogSymbolLegandre *window = new DialogSymbolLegandre(this, mode);
        connect(window, SIGNAL(dialogSymbolLegandreMeta(int)),
                this, SLOT(slotDialogSymbolLegandreMeta(int)));
        connect(window, SIGNAL(dialogSymbolLegandre(int, std::pair<int, int>, std::pair<int, int>, SymbolLegandreOptions)),
                this, SLOT(slotDialogSymbolLegandre(int, std::pair<int, int>, std::pair<int, int>, SymbolLegandreOptions)));
        ui->mainLayout->addWidget(window);
    } else if (task == "Символ Якоби") {
        DialogSymbolJacobi *window = new DialogSymbolJacobi(this, mode);
        connect(window, SIGNAL(dialogSymbolJacobiMeta(int)),
                this, SLOT(slotDialogSymbolJacobiMeta(int)));
        connect(window, SIGNAL(dialogSymbolJacobi(int, std::pair<int, int>, std::pair<int, int>, SymbolJacobiOptions)),
                this, SLOT(slotDialogSymbolJacobi(int, std::pair<int, int>, std::pair<int, int>, SymbolJacobiOptions)));
        ui->mainLayout->addWidget(window);
    }
}
