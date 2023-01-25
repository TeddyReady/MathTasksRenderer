#include "testmode.h"
#include "ui_testmode.h"

TestMode::TestMode(tasks_type &tasksForTest, QTime time, QWidget *parent) :
    QMainWindow(parent), allTime(time),
    ui(new Ui::TestMode)
{
    ui->setupUi(this);
    ui->taskView->setContextMenuPolicy(Qt::CustomContextMenu);
    engine = new TeXEngine(ui->taskView);
    isPressed = false;
    tasks = std::move(tasksForTest);
    QAction *reloadAction = new QAction(QPixmap("://img/reload.png"), "Reload task", ui->toolBar);
    connect(reloadAction, &QAction::triggered, [&](){
        engine->TeX2SVG(std::get<0>(tasks[curTask - 1]), true);
    }); ui->toolBar->addAction(reloadAction);

    if (tasks.size() == 1) {
        ui->prevTask->setHidden(true);
        ui->nextTask->setText("Завершить");
        results.push_back("");
        ui->groupBox->setTitle("Задание 1");
        engine->TeX2SVG(std::get<0>(tasks[0]), true);
        ui->lineEdit->setValidator(new QRegExpValidator(getInstructions(std::get<2>(tasks[0])), this));
        curTask = 1;
    } else {
        for (int i = 0; i < tasks.size(); i++) {
            results.push_back("");
            QAction *taskAction = new QAction("№" + QString::number(i + 1), ui->toolBar);
            connect(taskAction, SIGNAL(triggered()), this, SLOT(changeTask()));
            ui->toolBar->addAction(taskAction);
            if (i == 0) {
                emit taskAction->triggered();
            }
        } ui->prevTask->setDisabled(true);
    }
    if (allTime != QTime(0, 0, 0, 0)) {
        timerExist = true;
        timer = new QTimer(this);
        timer->setInterval(1000);
        connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
        statusBar()->showMessage(allTime.toString("hh:mm:ss"), 1000);
        timer->start();
    } else timerExist = false;
}

TestMode::~TestMode()
{
    delete engine;
    delete ui;
}

void TestMode::changeTask()
{
    QAction *tmp = reinterpret_cast<QAction *>(sender());
    if (ui->lineEdit->text() != "") {
        results[curTask - 1] = ui->lineEdit->text();
        ui->lineEdit->clear();
    }
    curTask = static_cast<QString>(tmp->text().split("№").last()).toInt();
    ui->groupBox->setTitle("Задание " + QString::number(curTask));
    engine->TeX2SVG(std::get<0>(tasks[curTask - 1]), true);
    ui->lineEdit->setValidator(new QRegExpValidator(getInstructions(std::get<2>(tasks[curTask - 1])), this));
    if (results[curTask - 1] != "") {
        ui->lineEdit->setText(results[curTask - 1]);
    }

    if (tasks.size() > 1) {
        if (curTask  == 1) {
            ui->prevTask->setDisabled(true);
            ui->nextTask->setEnabled(true);
            ui->nextTask->setText("Вперед");
        } else if (curTask == tasks.size()) {
            ui->prevTask->setEnabled(true);
            ui->nextTask->setText("Завершить");
        } else {
            ui->prevTask->setEnabled(true);
            ui->nextTask->setEnabled(true);
            ui->nextTask->setText("Вперед");
        }
    }
}

void TestMode::on_prevTask_clicked()
{
    ui->nextTask->setEnabled(true);
    if (ui->lineEdit->text() != "") {
        results[curTask - 1] = ui->lineEdit->text();
        ui->lineEdit->clear();
    }
    curTask = static_cast<QString>(ui->groupBox->title().split(" ").last()).toInt() - 1;
    ui->groupBox->setTitle("Задание " + QString::number(curTask));
    engine->TeX2SVG(std::get<0>(tasks[curTask - 1]), true);
    ui->lineEdit->setValidator(new QRegExpValidator(getInstructions(std::get<2>(tasks[curTask - 1])), this));
    if (results[curTask - 1] != "") {
        ui->lineEdit->setText(results[curTask - 1]);
    }
    isPressed = false;
    ui->nextTask->setText("Вперед");
    if (curTask  != 1)
        ui->prevTask->setEnabled(true);
    else ui->prevTask->setDisabled(true);
}

void TestMode::on_nextTask_clicked()
{
    if (tasks.size() == 1 || tasks.size() == curTask) {
        int answer = QMessageBox::question(this, "Завершение тестирования",
                     "Вы действительно хотите завершить тест?");
        if (answer == QMessageBox::Yes) finishTest();
    } else {
        ui->prevTask->setEnabled(true);
        if (curTask != tasks.size()) {
            if (ui->lineEdit->text() != "") {
                results[curTask - 1] = ui->lineEdit->text();
                ui->lineEdit->clear();
            }
            curTask = static_cast<QString>(ui->groupBox->title().split(" ").last()).toInt() + 1;
            if (curTask == tasks.size()) {
                isPressed = true;
                ui->nextTask->setText("Завершить");
            }
            ui->groupBox->setTitle("Задание " + QString::number(curTask));
            engine->TeX2SVG(std::get<0>(tasks[curTask - 1]), true);
            ui->lineEdit->setValidator(new QRegExpValidator(getInstructions(std::get<2>(tasks[curTask - 1])), this));
            if (results[curTask - 1] != "") {
                ui->lineEdit->setText(results[curTask - 1]);
            }
        } else {
            if (isPressed) {
                int answer = QMessageBox::question(this, "Завершение тестирования",
                             "Вы действительно хотите завершить тест?");
                if (answer == QMessageBox::Yes) finishTest();
            }
        }
    }
}

void TestMode::finishTest()
{
    if (ui->lineEdit->text() != "") {
        results[curTask - 1] = ui->lineEdit->text();
        ui->lineEdit->clear();
    } if (timerExist) delete timer;
    DialogResults *window = new DialogResults(tasks, results, this);
    window->setWindowTitle("Результаты теста");
    window->setModal(true);
    window->exec();
    close();
}

void TestMode::closeEvent(QCloseEvent *event){
    event->accept();
}

void TestMode::updateTime(){
    if (allTime != QTime(0, 0, 0, 0)) {
        allTime = allTime.addSecs(-1);
        statusBar()->showMessage(allTime.toString("hh:mm:ss"), 1000);
    }
    else finishTest();
}
