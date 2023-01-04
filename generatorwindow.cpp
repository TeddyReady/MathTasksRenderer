#include "generatorwindow.h"
#include "ui_generatorwindow.h"

GeneratorWindow::GeneratorWindow(QWidget *parent)
    : QMainWindow(parent), totalTestTasks(0),
      totalTaskCount(0), curTaskCount(0), mode(false),
      tasksForTest(new tasks_type),
      tasksForWork(new QString("\\color{sienna}{")),
      random(QRandomGenerator::global()),
      ui(new Ui::GeneratorWindow)
{
    settings = new QSettings("../ProjectTasksGenerator/general/settings.ini",
     QSettings::IniFormat, this); uploadSettings();

    ui->setupUi(this);
    ui->genButton->setEnabled(true);
    ui->webView->load(QUrl("qrc:/web/index.html"));
    ui->mainLayout->addWidget(new DialogTranspositionGroup(this, true));

    engine = new TeXEngine(ui->taskView);

    setAttribute(Qt::WA_DeleteOnClose);
}
GeneratorWindow::~GeneratorWindow()
{
    saveSettings();
    random = nullptr;
    delete engine;
    delete tasksForTest;
    delete tasksForWork;
    delete ui;
}

void GeneratorWindow::saveSettings()
{
    settings->setValue("windowSize", geometry());
}
void GeneratorWindow::uploadSettings()
{
    setGeometry(settings->value("windowSize").toRect());
}

void GeneratorWindow::isReadyRender(){
    if (curTaskCount == totalTaskCount) {
        *tasksForWork += "}";
        engine->TeX2SVG(*tasksForWork);
        tasksForWork->chop(1);
        totalTaskCount = 0;
        curTaskCount = 0;
    }
}

void GeneratorWindow::on_tabWidget_currentChanged(int index)
{
    switch (index) {
    case 2:
        mode = 1;
        break;
    default:
        mode = 0;
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

void GeneratorWindow::on_actionQuit_triggered()
{
    qApp->exit();
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
                *tasksForWork += QString::number(curTaskCount)  + ")~φ(" + QString::number(task.getTask()) + ")=" + QString::number(task.solve()) + "\\\\";
                isReadyRender();
            } else {
                QString taskText = "Вычислите функцию Эйлера:\nφ(" + QString::number(task.getTask()) + ") = ?";
                tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
            } curTaskCount++;
        } break;
    case EulerFunctionOptions::Primes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minNum, maxNum));
            if (isPrime(task.getTask())) {
                if (!mode) {
                    *tasksForWork += QString::number(curTaskCount)  + ")~φ(" + QString::number(task.getTask()) + ")=" + QString::number(task.solve()) + "\\\\";
                    isReadyRender();
                } else {
                    QString taskText = "Вычислите функцию Эйлера:\nφ(" + QString::number(task.getTask()) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
                } i++; curTaskCount++;
            }
        } break;
    case EulerFunctionOptions::PrimesDegrees:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minNum, maxNum));
            auto data = decompositionToSimple(task.getTask());
            if (data.size() == 1 && data[0].second > 1) {
                if (!mode) {
                    *tasksForWork += QString::number(curTaskCount)  + ")~φ(" + QString::number(task.getTask()) + ")=" + QString::number(task.solve()) + "\\\\";
                    isReadyRender();
                } else {
                    QString taskText = "Вычислите функцию Эйлера:\nφ(" + QString::number(task.getTask()) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
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
                    *tasksForWork += QString::number(curTaskCount)  + ")~φ(" + QString::number(task.getTask()) + ")=" + QString::number(task.solve()) + "\\\\";
                    isReadyRender();
                } else {
                    QString taskText = "Вычислите функцию Эйлера:\nφ(" + QString::number(task.getTask()) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
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
                    *tasksForWork += QString::number(curTaskCount)  + ")~φ(" + QString::number(task.getTask()) + ")=" + QString::number(task.solve()) + "\\\\";
                    isReadyRender();
                } else {
                    QString taskText = "Вычислите функцию Эйлера:\nφ(" + QString::number(task.getTask()) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
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
    }
}
void GeneratorWindow::slotDialogMebiusFunction(int countOfTasks, int minNum, int maxNum, MebiusFunctionOptions option){
    MebiusFunction task;
    switch (option) {
    case MebiusFunctionOptions::Default:
        for (int i = 0; i < countOfTasks; i++) {
            task.setTask(random->bounded(minNum, maxNum));
            if (!mode) {
                *tasksForWork += QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=" + QString::number(task.solve()) + "\\\\";
                isReadyRender();
            } else {
                QString taskText = "Вычислите функцию Мёбиуса:\nμ(" + QString::number(task.getTask()) + ") = ?";
                tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
            } curTaskCount++;
        } break;
    case MebiusFunctionOptions::Primes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minNum, maxNum));
            if (!isPrime(task.getTask())) {
                if (!mode) {
                    *tasksForWork += QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=" + QString::number(task.solve()) + "\\\\";
                    isReadyRender();
                } else {
                    QString taskText = "Вычислите функцию Мёбиуса:\nμ(" + QString::number(task.getTask()) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
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
                    *tasksForWork += QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=" + QString::number(task.solve()) + "\\\\";
                    isReadyRender();
                } else {
                    QString taskText = "Вычислите функцию Мёбиуса:\nμ(" + QString::number(task.getTask()) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
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
                    *tasksForWork += QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=" + QString::number(task.solve()) + "\\\\";
                    isReadyRender();
                } else {
                    QString taskText = "Вычислите функцию Мёбиуса:\nμ(" + QString::number(task.getTask()) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
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
                    *tasksForWork += QString::number(curTaskCount)  + ")~\\mu(" + QString::number(task.getTask()) + ")=" + QString::number(task.solve()) + "\\\\";
                    isReadyRender();
                } else {
                    QString taskText = "Вычислите функцию Мёбиуса:\nμ(" + QString::number(task.getTask()) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
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
                       "}\\right)=" + QString::number(task.solve()) + "\\\\"; isReadyRender();
                } else {
                    QString taskText = "Вычислите символ Лежандра:\nL(" + QString::number(task.getTask().first) +
                            " / " + QString::number(task.getTask().second) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
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
                       "}\\right)=" + QString::number(task.solve()) + "\\\\"; isReadyRender();
                } else {
                    QString taskText = "Вычислите символ Лежандра:\nL(" + QString::number(task.getTask().first) +
                            " / " + QString::number(task.getTask().second) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
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
                       "}\\right)=" + QString::number(task.solve()) + "\\\\"; isReadyRender();
                } else {
                    QString taskText = "Вычислите символ Лежандра:\nL(" + QString::number(task.getTask().first) +
                            " / " + QString::number(task.getTask().second) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
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
                       "}\\right)=" + QString::number(task.solve()) + "\\\\"; isReadyRender();
                } else {
                    QString taskText = "Вычислите символ Лежандра:\nL(" + QString::number(task.getTask().first) +
                            " / " + QString::number(task.getTask().second) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
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
                       "}\\right)=" + QString::number(task.solve()) + "\\\\"; isReadyRender();
                } else {
                    QString taskText = "Вычислите символ Лежандра:\nL(" + QString::number(task.getTask().first) +
                            " / " + QString::number(task.getTask().second) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
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
                       "}\\right)=" + QString::number(task.solve()) + "\\\\"; isReadyRender();
                } else {
                    QString taskText = "Вычислите символ Якоби:\nJ(" + QString::number(task.getTask().first) +
                            " / " + QString::number(task.getTask().second) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
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
                       "}\\right)=" + QString::number(task.solve()) + "\\\\"; isReadyRender();
                } else {
                    QString taskText = "Вычислите символ Якоби:\nJ(" + QString::number(task.getTask().first) +
                            " / " + QString::number(task.getTask().second) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
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
                       "}\\right)=" + QString::number(task.solve()) + "\\\\"; isReadyRender();
                } else {
                    QString taskText = "Вычислите символ Якоби:\nJ(" + QString::number(task.getTask().first) +
                            " / " + QString::number(task.getTask().second) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
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
                       "}\\right)=" + QString::number(task.solve()) + "\\\\"; isReadyRender();
                } else {
                    QString taskText = "Вычислите символ Якоби:\nJ(" + QString::number(task.getTask().first) +
                            " / " + QString::number(task.getTask().second) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
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
                       "}\\right)=" + QString::number(task.solve()) + "\\\\"; isReadyRender();
                } else {
                    QString taskText = "Вычислите символ Якоби:\nJ(" + QString::number(task.getTask().first) +
                            " / " + QString::number(task.getTask().second) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
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
    TranspositionGroup task, task2, result;
    switch (option) {
    case TranspositionGroupOptions::Write:
        if (countOfTasks > 0) curTaskCount = 1;
        else return;

        if (mode == ViewMode::Standart)
              *tasksForWork += "Запишите~подстановку~в~виде~произведения~независимых~циклов:\\\\";
        else *tasksForWork += "Запишите~подстановку~в~табличном~виде:\\\\";
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart)
                    *tasksForWork += QString::number(curTaskCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=" + task.writeToMode(ViewMode::Cycle) + "\\\\";
                else *tasksForWork += QString::number(curTaskCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "=" + task.writeToMode(ViewMode::Standart) + "\\\\";
                isReadyRender(); i++; curTaskCount++;
            } else {}
        } break;

    case TranspositionGroupOptions::Multiply:
        if (countOfTasks > 0) curTaskCount = 1;
        else return;

        if (mode == ViewMode::Standart)
              *tasksForWork += "Найдите~произведение~подстановок,~записанных~в~табличном~виде:\\\\";
        else *tasksForWork += "Найдите~произведение~подстановок,~записанных~в~циклическом~виде:\\\\";
        for (int i = 0; i < countOfTasks;) {
            int n = static_cast<int>(random->bounded(minN, maxN));
            task.setTask(n, mode);
            task2.setTask(n, mode);
            result = task * task2;
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart)
                    *tasksForWork += QString::number(curTaskCount) + ")~S_{" + QString::number(task.getTask()) + "}:" +
                     task.writeToMode(ViewMode::Standart) + "·" + task2.writeToMode(ViewMode::Standart) + "=" + result.writeToMode(ViewMode::Standart) + "\\\\";
                else *tasksForWork += QString::number(curTaskCount) + ")~S_{" + QString::number(task.getTask()) + "}:" +
                     task.writeToMode(ViewMode::Cycle) + "·" + task2.writeToMode(ViewMode::Cycle) + "=" + result.writeToMode(ViewMode::Cycle) + "\\\\";
                isReadyRender(); i++; curTaskCount++;
            } else {}
        } break;

    case TranspositionGroupOptions::Inverse:
        if (countOfTasks > 0) curTaskCount = 1;
        else return;

        if (mode == ViewMode::Standart)
              *tasksForWork += "Найдите~подстановку,~обратную~данной,~записанной~в~табличном~виде:\\\\";
        else *tasksForWork += "Найдите~подстановку,~обратную~данной,~записанной~в~циклическом~виде:\\\\";
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minN, maxN), mode);
            result = ~task;
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart)
                    *tasksForWork += QString::number(curTaskCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=" +
                         result.writeToMode(ViewMode::Standart) + "\\\\";
                else *tasksForWork += QString::number(curTaskCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "=" +
                         result.writeToMode(ViewMode::Cycle) + "\\\\";
                isReadyRender(); i++; curTaskCount++;
            } else {}
        } break;

    case TranspositionGroupOptions::Cycle:
        if (countOfTasks > 0) curTaskCount = 1;
        else return;

        if (mode == ViewMode::Standart)
              *tasksForWork += "Выйдите~цикловой~тип~подстановки,~записанной~в~табличном~виде:\\\\";
        else *tasksForWork += "Выйдите~цикловой~тип~подстановки,~записанной~в~циклическом~виде:\\\\";
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart)
                    *tasksForWork += QString::number(curTaskCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Standart) + "=" +
                         task.cycleType() + "\\\\";
                else *tasksForWork += QString::number(curTaskCount) + ")~S_{" + QString::number(task.getTask()) + "}:" + task.writeToMode(ViewMode::Cycle) + "=" +
                         task.cycleType() + "\\\\";
                isReadyRender(); i++; curTaskCount++;
            } else {}
        } break;

    case TranspositionGroupOptions::Count:
        if (countOfTasks > 0) curTaskCount = 1;
        else return;

        if (mode == ViewMode::Standart)
              *tasksForWork += "Вычислите~количество~беспорядков~подстановки,~записанной~в~табличном~виде:\\\\";
        else *tasksForWork += "Вычислите~количество~беспорядков~подстановки,~записанной~в~циклическом~виде:\\\\";
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart)
                    *tasksForWork += QString::number(curTaskCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\Delta" + task.writeToMode(ViewMode::Standart) + "=" +
                         QString::number(task.getHaos()) + "\\\\";
                else *tasksForWork += QString::number(curTaskCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\Delta" + task.writeToMode(ViewMode::Cycle) + "=" +
                         QString::number(task.getHaos()) + "\\\\";
                isReadyRender(); i++; curTaskCount++;
            } else {}
        } break;

    case TranspositionGroupOptions::Mod2:
        if (countOfTasks > 0) curTaskCount = 1;
        else return;

        if (mode == ViewMode::Standart)
              *tasksForWork += "Определите~четность~подстановки,~записанной~в~табличном~виде:\\\\";
        else *tasksForWork += "Определите~четность~подстановки,~записанной~в~циклическом~виде:\\\\";
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (!this->mode) {
                if (task.getViewMode() == ViewMode::Standart)
                    *tasksForWork += QString::number(curTaskCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\delta" + task.writeToMode(ViewMode::Standart) + "\\Rightarrow" +
                         task.getEven() + "\\\\";
                else *tasksForWork += QString::number(curTaskCount) + ")~S_{" + QString::number(task.getTask()) + "}:\\delta" + task.writeToMode(ViewMode::Cycle) + "\\Rightarrow" +
                        task.getEven() + "\\\\";
                isReadyRender(); i++; curTaskCount++;
            } else {}
        } break;

    case TranspositionGroupOptions::Order:
        if (countOfTasks > 0) curTaskCount = 1;
        else return;

        if (mode == ViewMode::Standart)
              *tasksForWork += "Определите~порядок~подстановки,~записанной~в~табличном~виде:\\\\";
        else *tasksForWork += "Определите~порядок~подстановки,~записанной~в~циклическом~виде:\\\\";
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (!this->mode) {
            if (task.getViewMode() == ViewMode::Standart)
                *tasksForWork += QString::number(curTaskCount) + ")~S_{" + QString::number(task.getTask()) + "}:~ord" + task.writeToMode(ViewMode::Standart) + "=" +
                     QString::number(task.getOrder()) + "\\\\";
            else *tasksForWork += QString::number(curTaskCount) + ")~S_{" + QString::number(task.getTask()) + "}:~ord" + task.writeToMode(ViewMode::Cycle) + "=" +
                    QString::number(task.getOrder()) + "\\\\";
            isReadyRender(); i++; curTaskCount++;
            } else {}
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
