#include "generatorwindow.h"
#include "ui_generatorwindow.h"

GeneratorWindow::GeneratorWindow(QWidget *parent)
    : QMainWindow(parent)
    , mode(Test), ui(new Ui::GeneratorWindow)
{
    random = QRandomGenerator::global();
    tasksForTest = new tasks_type;
    ui->setupUi(this);
    testMode = new QAction("Режим Теста", this);
    workMode = new QAction("Режим Задачника", this);
    workMode->setCheckable(true);
    ui->toolBar->addAction(testMode);
    ui->toolBar->addAction(workMode);
    ui->genButton->setEnabled(true);
    ui->taskView->append("<!Doctype HTML>");
    //setWindowFlags(Qt::FramelessWindowHint);
    connect(testMode, SIGNAL(triggered()), this, SLOT(runTestMode()));
    connect(workMode, SIGNAL(triggered()), this, SLOT(runWorkMode()));
    emit workMode->triggered();

    setAttribute(Qt::WA_DeleteOnClose);
}
GeneratorWindow::~GeneratorWindow()
{
    random = nullptr;
    delete tasksForTest;
    delete ui;
}

void GeneratorWindow::runTestMode(){
    if (mode != Test && tasksForTest->isEmpty()) {
        mode = Test;
        workMode->setChecked(false);
        workMode->setEnabled(true);
        ui->taskView->clear();
        ui->taskView->append("Режим генерации теста активирован!\n"
                             "Сгенерируйте задания и нажмите на кнопку повторно для запуска тестирования");
    } else if (mode == Test && tasksForTest->isEmpty()) {
        ui->taskView->append("Перед запуском теста, необходимо сгенерировать задания!\n");
    } else {
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
    }
}

void GeneratorWindow::runWorkMode(){
    if (mode != Work) {
        mode = Work;
        ui->taskView->clear();
        ui->taskView->append("Режим задачника активирован!\n"
                             "Выберите задание из списка и сгенерируйте!");
        workMode->setChecked(true);
        workMode->setDisabled(true);
        testMode->setChecked(false);
    }
}

void GeneratorWindow::on_genButton_clicked()
{
    if (ui->tasksList->currentItem() == nullptr) {
        ui->taskView->append("Перед генерацией выберите типаж задания!");
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
void GeneratorWindow::on_actionTXT_triggered()
{
    QString path = QFileDialog::getSaveFileName(0, QObject::tr("Сохранить задания"),
        "/home/kataich75/qtprojects/TasksGenerator/tests/test_" +
        QDateTime::currentDateTime().toString("dd.MM.yyyy") + "_" +
        QDateTime::currentDateTime().toString("hh:mm:ss") + ".txt", QObject::tr("TXT files (*.txt)"));
    file.setFileName(path);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        int taskIndex = 0;
        for (int i = 0; i < generatedTasks.size(); i += 2) {
            int task = generatedTasks[i].toInt();
            int countOfTasks = generatedTasks[i + 1].toInt();
            out << task << " " << countOfTasks << "\n";
            switch (task) {
            case TaskEulerFunction:
                for (int j = 0; j < countOfTasks; j++) {
                    out << generatedData[taskIndex] << " " << generatedData[taskIndex + 1] << "\n";
                    taskIndex += 2;
                } break;
            case TaskMebiusFunction:
                for (int j = 0; j < countOfTasks; j++) {
                    out << generatedData[taskIndex] << " " << generatedData[taskIndex + 1] << "\n";
                    taskIndex += 2;
                } break;
            case TaskSymbolLegandre:
                for (int j = 0; j < countOfTasks; j++) {
                    out << generatedData[taskIndex] << " " << generatedData[taskIndex + 1]
                        << " " << generatedData[taskIndex + 2] << "\n";
                    taskIndex += 3;
                } break;
            case TaskSymbolJacobi:
                for (int j = 0; j < countOfTasks; j++) {
                    out << generatedData[taskIndex] << " " << generatedData[taskIndex + 1]
                        << " " << generatedData[taskIndex + 2] << "\n";
                    taskIndex += 3;
                } break;
            case TaskTranspositionGroup:
                for (int j = 0; j < countOfTasks; j++) {
                    out << generatedData[taskIndex] << " " << generatedData[taskIndex + 1] << "\n";
                    taskIndex += 2;
                } break;
            }
        }
    }
    file.close();
}
void GeneratorWindow::on_actionQuit_triggered()
{
    qApp->exit();
}

void GeneratorWindow::slotDialogEulerFunctionMeta(int countOfTasks)
{
    generatedTasks.push_back(QString::number(TaskEulerFunction));
    generatedTasks.push_back(QString::number(countOfTasks));
    if (countOfTasks > 0) {
        count = 1;
        switch (mode) {
        case Test:
            ui->taskView->append("<h3>Задания на функцию Эйлера сгенерированы!</h3>");
            break;
        case Work:
            ui->taskView->append("<h3>Вычислите функцию Эйлера:</h3>");
            break;
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
            if (mode == Work) {
                ui->taskView->append(QString::number(count)  + ") φ(" + QString::number(task.getTask()) + ") = " + QString::number(task.solve()));
                generatedData.push_back(QString::number(task.getTask()));
                generatedData.push_back(QString::number(task.solve()));
            } else {
                QString taskText = "Вычислите функцию Эйлера:\nφ(" + QString::number(task.getTask()) + ") = ?";
                tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
            } count++;
        } break;
    case EulerFunctionOptions::Primes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minNum, maxNum));
            if (isPrime(task.getTask())) {
                if (mode == Work) {
                    ui->taskView->append(QString::number(count)  + ") φ(" + QString::number(task.getTask()) + ") = " + QString::number(task.solve()));
                    generatedData.push_back(QString::number(task.getTask()));
                    generatedData.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "Вычислите функцию Эйлера:\nφ(" + QString::number(task.getTask()) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
                } i++; count++;
            }
        } break;
    case EulerFunctionOptions::PrimesDegrees:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minNum, maxNum));
            auto data = decompositionToSimple(task.getTask());
            if (data.size() == 1 && data[0].second > 1) {
                if (mode == Work) {
                    ui->taskView->append(QString::number(count)  + ") φ(" + QString::number(task.getTask()) + ") = " + QString::number(task.solve()));
                    generatedData.push_back(QString::number(task.getTask()));
                    generatedData.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "Вычислите функцию Эйлера:\nφ(" + QString::number(task.getTask()) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
                } i++; count++;
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
                if (mode == Work) {
                    ui->taskView->append(QString::number(count)  + ") φ(" + QString::number(task.getTask()) + ") = " + QString::number(task.solve()));
                    generatedData.push_back(QString::number(task.getTask()));
                    generatedData.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "Вычислите функцию Эйлера:\nφ(" + QString::number(task.getTask()) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
                } i++; count++;
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
                if (mode == Work) {
                    ui->taskView->append(QString::number(count)  + ") φ(" + QString::number(task.getTask()) + ") = " + QString::number(task.solve()));
                    generatedData.push_back(QString::number(task.getTask()));
                    generatedData.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "Вычислите функцию Эйлера:\nφ(" + QString::number(task.getTask()) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
                } i++; count++;
            }
        } break;
    }
}

void GeneratorWindow::slotDialogMebiusFunctionMeta(int countOfTasks){
    generatedTasks.push_back(QString::number(TaskMebiusFunction));
    generatedTasks.push_back(QString::number(countOfTasks));
    if (countOfTasks > 0) {
        count = 1;
        switch (mode) {
        case Test:
            ui->taskView->append("<h3>Задания на функцию Мёбиуса сгенерированы!</h3>");
            break;
        case Work:
            ui->taskView->append("<h3>Вычислите функцию Мёбиуса:</h3>");
            break;
        }
    }
}
void GeneratorWindow::slotDialogMebiusFunction(int countOfTasks, int minNum, int maxNum, MebiusFunctionOptions option){
    MebiusFunction task;
    switch (option) {
    case MebiusFunctionOptions::Default:
        for (int i = 0; i < countOfTasks; i++) {
            task.setTask(random->bounded(minNum, maxNum));
            if (mode == Work) {
                ui->taskView->append(QString::number(count)  + ") μ(" + QString::number(task.getTask()) + ") = " + QString::number(task.solve()));
                generatedData.push_back(QString::number(task.getTask()));
                generatedData.push_back(QString::number(task.solve()));
            } else {
                QString taskText = "Вычислите функцию Мёбиуса:\nμ(" + QString::number(task.getTask()) + ") = ?";
                tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
            } count++;
        } break;
    case MebiusFunctionOptions::Primes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minNum, maxNum));
            if (!isPrime(task.getTask())) {
                if (mode == Work) {
                    ui->taskView->append(QString::number(count)  + ") μ(" + QString::number(task.getTask()) + ") = " + QString::number(task.solve()));
                    generatedData.push_back(QString::number(task.getTask()));
                    generatedData.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "Вычислите функцию Мёбиуса:\nμ(" + QString::number(task.getTask()) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
                } i++; count++;
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
                if (mode == Work) {
                    ui->taskView->append(QString::number(count)  + ") μ(" + QString::number(task.getTask()) + ") = " + QString::number(task.solve()));
                    generatedData.push_back(QString::number(task.getTask()));
                    generatedData.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "Вычислите функцию Мёбиуса:\nμ(" + QString::number(task.getTask()) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
                } i++; count++;
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
                if (mode == Work) {
                    ui->taskView->append(QString::number(count)  + ") μ(" + QString::number(task.getTask()) + ") = " + QString::number(task.solve()));
                    generatedData.push_back(QString::number(task.getTask()));
                    generatedData.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "Вычислите функцию Мёбиуса:\nμ(" + QString::number(task.getTask()) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
                } i++; count++;
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
                if (mode == Work) {
                    ui->taskView->append(QString::number(count)  + ") μ(" + QString::number(task.getTask()) + ") = " + QString::number(task.solve()));
                    generatedData.push_back(QString::number(task.getTask()));
                    generatedData.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "Вычислите функцию Мёбиуса:\nμ(" + QString::number(task.getTask()) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
                } i++; count++;
            }
        } break;
    }
}

void GeneratorWindow::slotDialogSymbolLegandreMeta(int countOfTasks){
    generatedTasks.push_back(QString::number(TaskSymbolLegandre));
    generatedTasks.push_back(QString::number(countOfTasks));
    if (countOfTasks > 0) {
        count = 1;
        switch (mode) {
        case Test:
            ui->taskView->append("<h3>Задания на символ Лежандра сгенерированы!</h3>");
            break;
        case Work:
            ui->taskView->append("<h3>Вычислите символ Лежандра:</h3>");
            break;
        }
    }
}
void GeneratorWindow::slotDialogSymbolLegandre(int countOfTasks, std::pair<int, int> a, std::pair<int, int> p, SymbolLegandreOptions option){
    SymbolLegandre task;
    switch (option) {
    case SymbolLegandreOptions::Default:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second)) {
                if (mode == Work) {
                    ui->taskView->append(QString::number(count)  + ") (<sup>" + QString::number(task.getTask().first) +
                        "</sup>/<sub>" + QString::number(task.getTask().second) + "</sub>) = " + QString::number(task.solve()));
                    generatedData.push_back(QString::number(task.getTask().first));
                    generatedData.push_back(QString::number(task.getTask().second));
                    generatedData.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "Вычислите символ Лежандра:\nL(" + QString::number(task.getTask().first) +
                            " / " + QString::number(task.getTask().second) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
                } i++; count++;
            }
        } break;
    case SymbolLegandreOptions::Primes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second) && !isPrime(task.getTask().first)) {
                if (mode == Work) {
                    ui->taskView->append(QString::number(count)  + ") (<sup>" + QString::number(task.getTask().first) +
                        "</sup>/<sub>" + QString::number(task.getTask().second) + "</sub>) = " + QString::number(task.solve()));
                    generatedData.push_back(QString::number(task.getTask().first));
                    generatedData.push_back(QString::number(task.getTask().second));
                    generatedData.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "Вычислите символ Лежандра:\nL(" + QString::number(task.getTask().first) +
                            " / " + QString::number(task.getTask().second) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
                } i++; count++;
            }
        } break;
    case SymbolLegandreOptions::aEqual_1:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second)) {
                if (mode == Work) {
                    ui->taskView->append(QString::number(count)  + ") (<sup>" + QString::number(task.getTask().first) +
                        "</sup>/<sub>" + QString::number(task.getTask().second) + "</sub>) = " + QString::number(task.solve()));
                    generatedData.push_back(QString::number(task.getTask().first));
                    generatedData.push_back(QString::number(task.getTask().second));
                    generatedData.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "Вычислите символ Лежандра:\nL(" + QString::number(task.getTask().first) +
                            " / " + QString::number(task.getTask().second) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
                } i++; count++;
            }
        } break;
    case SymbolLegandreOptions::aEqual2:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second)) {
                if (mode == Work) {
                    ui->taskView->append(QString::number(count)  + ") (<sup>" + QString::number(task.getTask().first) +
                        "</sup>/<sub>" + QString::number(task.getTask().second) + "</sub>) = " + QString::number(task.solve()));
                    generatedData.push_back(QString::number(task.getTask().first));
                    generatedData.push_back(QString::number(task.getTask().second));
                    generatedData.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "Вычислите символ Лежандра:\nL(" + QString::number(task.getTask().first) +
                            " / " + QString::number(task.getTask().second) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
                } i++; count++;
            }
        } break;
    case SymbolLegandreOptions::NotEvenPrimes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (isPrime(task.getTask().second) && !isPrime(task.getTask().first) && task.getTask().first % 2 != 0) {
                if (mode == Work) {
                    ui->taskView->append(QString::number(count)  + ") (<sup>" + QString::number(task.getTask().first) +
                        "</sup>/<sub>" + QString::number(task.getTask().second) + "</sub>) = " + QString::number(task.solve()));
                    generatedData.push_back(QString::number(task.getTask().first));
                    generatedData.push_back(QString::number(task.getTask().second));
                    generatedData.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "Вычислите символ Лежандра:\nL(" + QString::number(task.getTask().first) +
                            " / " + QString::number(task.getTask().second) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
                } i++; count++;
            }
        } break;
    }
}

void GeneratorWindow::slotDialogSymbolJacobiMeta(int countOfTasks){
    generatedTasks.push_back(QString::number(TaskSymbolJacobi));
    generatedTasks.push_back(QString::number(countOfTasks));
    if (countOfTasks > 0) {
        count = 1;
        switch (mode) {
        case Test:
            ui->taskView->append("<h3>Задания на символ Якоби сгенерированы!</h3>");
            break;
        case Work:
            ui->taskView->append("<h3>Вычислите символ Якоби:</h3>");
            break;
        }
    }
}
void GeneratorWindow::slotDialogSymbolJacobi(int countOfTasks, std::pair<int, int> a, std::pair<int, int> p, SymbolJacobiOptions option){
    SymbolJacobi task;
    switch (option) {
    case SymbolJacobiOptions::Default:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (!isPrime(task.getTask().second) && task.getTask().second % 2 != 0) {
                if (mode == Work) {
                    ui->taskView->append(QString::number(count)  + ") (<sup>" + QString::number(task.getTask().first) +
                        "</sup>/<sub>" + QString::number(task.getTask().second) + "</sub>) = " + QString::number(task.solve()));
                    generatedData.push_back(QString::number(task.getTask().first));
                    generatedData.push_back(QString::number(task.getTask().second));
                    generatedData.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "Вычислите символ Якоби:\nJ(" + QString::number(task.getTask().first) +
                            " / " + QString::number(task.getTask().second) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
                } i++; count++;
            }
        } break;
    case SymbolJacobiOptions::Primes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (!isPrime(task.getTask().second) && !isPrime(task.getTask().first) && task.getTask().second % 2 != 0) {
                if (mode == Work) {
                    ui->taskView->append(QString::number(count)  + ") (<sup>" + QString::number(task.getTask().first) +
                        "</sup>/<sub>" + QString::number(task.getTask().second) + "</sub>) = " + QString::number(task.solve()));
                    generatedData.push_back(QString::number(task.getTask().first));
                    generatedData.push_back(QString::number(task.getTask().second));
                    generatedData.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "Вычислите символ Якоби:\nJ(" + QString::number(task.getTask().first) +
                            " / " + QString::number(task.getTask().second) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
                } i++; count++;
            }
        } break;
    case SymbolJacobiOptions::aEqual_1:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (!isPrime(task.getTask().second) && task.getTask().second % 2 != 0) {
                if (mode == Work) {
                    ui->taskView->append(QString::number(count)  + ") (<sup>" + QString::number(task.getTask().first) +
                        "</sup>/<sub>" + QString::number(task.getTask().second) + "</sub>) = " + QString::number(task.solve()));
                    generatedData.push_back(QString::number(task.getTask().first));
                    generatedData.push_back(QString::number(task.getTask().second));
                    generatedData.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "Вычислите символ Якоби:\nJ(" + QString::number(task.getTask().first) +
                            " / " + QString::number(task.getTask().second) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
                } i++; count++;
            }
        } break;
    case SymbolJacobiOptions::aEqual2:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (!isPrime(task.getTask().second) && task.getTask().second % 2 != 0) {
                if (mode == Work) {
                    ui->taskView->append(QString::number(count)  + ") (<sup>" + QString::number(task.getTask().first) +
                        "</sup>/<sub>" + QString::number(task.getTask().second) + "</sub>) = " + QString::number(task.solve()));
                    generatedData.push_back(QString::number(task.getTask().first));
                    generatedData.push_back(QString::number(task.getTask().second));
                    generatedData.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "Вычислите символ Якоби:\nJ(" + QString::number(task.getTask().first) +
                            " / " + QString::number(task.getTask().second) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
                } i++; count++;
            }
        } break;
    case SymbolJacobiOptions::NotEvenPrimes:
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(a.first, a.second), random->bounded(p.first, p.second));
            if (!isPrime(task.getTask().second) && !isPrime(task.getTask().first) && task.getTask().first % 2 != 0 && task.getTask().second % 2 != 0) {
                if (mode == Work) {
                    ui->taskView->append(QString::number(count)  + ") (<sup>" + QString::number(task.getTask().first) +
                        "</sup>/<sub>" + QString::number(task.getTask().second) + "</sub>) = " + QString::number(task.solve()));
                    generatedData.push_back(QString::number(task.getTask().first));
                    generatedData.push_back(QString::number(task.getTask().second));
                    generatedData.push_back(QString::number(task.solve()));
                } else {
                    QString taskText = "Вычислите символ Якоби:\nJ(" + QString::number(task.getTask().first) +
                            " / " + QString::number(task.getTask().second) + ") = ?";
                    tasksForTest->push_back(std::make_pair(taskText, QString::number(task.solve())));
                } i++; count++;
            }
        } break;
    }
}

void GeneratorWindow::slotDialogTranspositionGroupMeta(int countOfTasks){
    generatedTasks.push_back(QString::number(TaskTranspositionGroup));
    generatedTasks.push_back(QString::number(countOfTasks));
    if (countOfTasks > 0 && this->mode == Test)
            ui->taskView->append("<h3>Задачи на группу подстановок сгенерированы!</h3>");
}
void GeneratorWindow::slotDialogTranspositionGroup(int countOfTasks, int minN, int maxN, TranspositionGroupOptions option, ViewMode mode){
    TranspositionGroup task, task2, result;
    switch (option) {
    case TranspositionGroupOptions::Write:
        if (countOfTasks > 0) count = 1;
        if (mode == ViewMode::Standart)
              ui->taskView->append("<h3>Запишите подстановку в произведение независимых циклов:</h3>");
        else ui->taskView->append("<h3>Запишите подстановку в каноническом виде:</h3>");
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (task.getViewMode() == ViewMode::Standart) {
                ui->taskView->append(QString::number(count)  + ") S<sub>" + QString::number(task.getTask()) + "</sub>: " + task.writeToMode(ViewMode::Standart) + " = " + task.writeToMode(ViewMode::Cycle));
                generatedData.push_back(task.writeToMode(ViewMode::Standart));
                generatedData.push_back(task.writeToMode(ViewMode::Cycle));
            } else {
                ui->taskView->append(QString::number(count)  + ") S<sub>" + QString::number(task.getTask()) + "</sub>: " + task.writeToMode(ViewMode::Cycle) + " = " + task.writeToMode(ViewMode::Standart));
                generatedData.push_back(task.writeToMode(ViewMode::Cycle));
                generatedData.push_back(task.writeToMode(ViewMode::Standart));
            } i++; count++;
        } break;
    case TranspositionGroupOptions::Multiply:
        if (countOfTasks > 0) count = 1;
        if (mode == ViewMode::Standart)
              ui->taskView->append("<h3>Найдите произведение подстановок, записанных в каноническом виде:</h3>");
        else ui->taskView->append("<h3>Найдите произведение подстановок, записанных в циклическом виде:</h3>");
        for (int i = 0; i < countOfTasks;) {
            int n = static_cast<int>(random->bounded(minN, maxN));
            task.setTask(n, mode);
            task2.setTask(n, mode);
            result = task * task2;
            if (task.getViewMode() == ViewMode::Standart) {
                ui->taskView->append(QString::number(count)  + ") S<sub>" + QString::number(task.getTask()) + "</sub>: " + task.writeToMode(ViewMode::Standart) + " · " +
                     task2.writeToMode(ViewMode::Standart) + " = " + result.writeToMode(ViewMode::Standart));
                generatedData.push_back(task.writeToMode(ViewMode::Standart) + " " + task2.writeToMode(ViewMode::Standart));
                generatedData.push_back(result.writeToMode(ViewMode::Standart));
            } else {
                ui->taskView->append(QString::number(count)  + ") S<sub>" + QString::number(task.getTask()) + "</sub>: " + task.writeToMode(ViewMode::Cycle) + " · " +
                     task2.writeToMode(ViewMode::Cycle) + " = " + result.writeToMode(ViewMode::Cycle));
                generatedData.push_back(task.writeToMode(ViewMode::Cycle) + " " + task2.writeToMode(ViewMode::Cycle));
                generatedData.push_back(result.writeToMode(ViewMode::Cycle));
            } i++; count++;
        } break;
    case TranspositionGroupOptions::Inverse:
        if (countOfTasks > 0) count = 1;
        if (mode == ViewMode::Standart)
              ui->taskView->append("<h3>Найдите подстановку, обратную данной, записанной в каноническом виде:</h3>");
        else ui->taskView->append("<h3>Найдите подстановку, обратную данной, записанной в циклическом виде:</h3>");
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minN, maxN), mode);
            result = ~task;
            if (task.getViewMode() == ViewMode::Standart) {
                ui->taskView->append(QString::number(count)  + ") S<sub>" + QString::number(task.getTask()) + "</sub>: " + task.writeToMode(ViewMode::Standart) + " = " +
                     result.writeToMode(ViewMode::Standart));
                generatedData.push_back(task.writeToMode(ViewMode::Standart));
                generatedData.push_back(result.writeToMode(ViewMode::Standart));
            } else {
                ui->taskView->append(QString::number(count)  + ") S<sub>" + QString::number(task.getTask()) + "</sub>: " + task.writeToMode(ViewMode::Cycle) + " = " +
                     result.writeToMode(ViewMode::Cycle));
                generatedData.push_back(task.writeToMode(ViewMode::Cycle));
                generatedData.push_back(result.writeToMode(ViewMode::Cycle));
            } i++; count++;
        } break;
    case TranspositionGroupOptions::Cycle:
        if (countOfTasks > 0) count = 1;
        if (mode == ViewMode::Standart)
              ui->taskView->append("<h3>Найдите цикловой тип подстановки, записанной в каноническом виде:</h3>");
        else ui->taskView->append("<h3>Найдите цикловой тип подстановки, записанной в циклическом виде:</h3>");
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (task.getViewMode() == ViewMode::Standart) {
                ui->taskView->append(QString::number(count)  + ") S<sub>" + QString::number(task.getTask()) + "</sub>: " + task.writeToMode(ViewMode::Standart) + " = " +
                     task.cycleType());
                generatedData.push_back(task.writeToMode(ViewMode::Standart));
            } else {
                ui->taskView->append(QString::number(count)  + ") S<sub>" + QString::number(task.getTask()) + "</sub>: " + task.writeToMode(ViewMode::Cycle) + " = " +
                     task.cycleType());
                generatedData.push_back(task.writeToMode(ViewMode::Cycle));
            } i++; count++; generatedData.push_back(task.cycleType());
        } break;
    case TranspositionGroupOptions::Count:
        if (countOfTasks > 0) count = 1;
        if (mode == ViewMode::Standart)
              ui->taskView->append("<h3>Вычислите количество беспорядков подстановки, записанной в каноническом виде:</h3>");
        else ui->taskView->append("<h3>Вычислите количество беспорядков подстановки, записанной в циклическом виде:</h3>");
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (task.getViewMode() == ViewMode::Standart) {
                ui->taskView->append(QString::number(count)  + ") S<sub>" + QString::number(task.getTask()) + "</sub>: " + task.writeToMode(ViewMode::Standart) + " = " +
                     QString::number(task.getHaos()));
                generatedData.push_back(task.writeToMode(ViewMode::Standart));
            } else {
                ui->taskView->append(QString::number(count)  + ") S<sub>" + QString::number(task.getTask()) + "</sub>: " + task.writeToMode(ViewMode::Cycle) + " = " +
                     QString::number(task.getHaos()));
                generatedData.push_back(task.writeToMode(ViewMode::Cycle));
            } i++; count++; generatedData.push_back(QString::number(task.getHaos()));
        } break;
    case TranspositionGroupOptions::Mod2:
        if (countOfTasks > 0) count = 1;
        if (mode == ViewMode::Standart)
              ui->taskView->append("<h3>Определите четность подстановки, записанной в каноническом виде:</h3>");
        else ui->taskView->append("<h3>Определите четность подстановки, записанной в циклическом виде:</h3>");
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (task.getViewMode() == ViewMode::Standart) {
                ui->taskView->append(QString::number(count)  + ") S<sub>" + QString::number(task.getTask()) + "</sub>: " + task.writeToMode(ViewMode::Standart) + " - " +
                     task.getEven());
                generatedData.push_back(task.writeToMode(ViewMode::Standart));
            } else {
                ui->taskView->append(QString::number(count)  + ") S<sub>" + QString::number(task.getTask()) + "</sub>: " + task.writeToMode(ViewMode::Cycle) + " - " +
                     task.getEven());
                generatedData.push_back(task.writeToMode(ViewMode::Cycle));
            } i++; count++; generatedData.push_back(task.getEven());
        } break;
    case TranspositionGroupOptions::Order:
        if (countOfTasks > 0) count = 1;
        if (mode == ViewMode::Standart)
              ui->taskView->append("<h3>Определите порядок подстановки, записанной в каноническом виде:</h3>");
        else ui->taskView->append("<h3>Определите порядок подстановки, записанной в циклическом виде:</h3>");
        for (int i = 0; i < countOfTasks;) {
            task.setTask(random->bounded(minN, maxN), mode);
            if (task.getViewMode() == ViewMode::Standart) {
                ui->taskView->append(QString::number(count)  + ") S<sub>" + QString::number(task.getTask()) + "</sub>: " + task.writeToMode(ViewMode::Standart) + " = " +
                     QString::number(task.getOrder()));
                generatedData.push_back(task.writeToMode(ViewMode::Standart));
            } else {
                ui->taskView->append(QString::number(count)  + ") S<sub>" + QString::number(task.getTask()) + "</sub>: " + task.writeToMode(ViewMode::Cycle) + " = " +
                     QString::number(task.getOrder()));
                generatedData.push_back(task.writeToMode(ViewMode::Cycle));
            } i++; count++; generatedData.push_back(QString::number(task.getOrder()));
        } break;
    }
}
