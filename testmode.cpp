#include "testmode.h"
#include "ui_testmode.h"

TestMode::TestMode(QWidget *parent, tasks_type* tasksForTest) :
    QMainWindow(parent),
    ui(new Ui::TestMode)
{
    ui->setupUi(this);
    engine = new TeXEngine(ui->taskView);
    isPressed = false;
    tasks = tasksForTest;
    results = new QVector<QString>;
    if (tasks->size() == 1) {
        ui->prevTask->setHidden(true);
        ui->nextTask->setText("Завершить");
        ui->toolBar->hide();
        results->push_back("");
        ui->groupBox->setTitle("Задание 1");
        engine->TeX2SVG((*tasks)[0].first.first, true);
        ui->lineEdit->setValidator(new QRegExpValidator(getInstructions((*tasks)[0].second.first), this));
        curTask = 1;
    } else {
        for (int i = 0; i < tasks->size(); i++) {
            results->push_back("");
            QAction *taskAction = new QAction("№" + QString::number(i + 1), ui->toolBar);
            ui->toolBar->addAction(taskAction);
            connect(taskAction, SIGNAL(triggered()), this, SLOT(changeTask()));
            if (i == 0) {
                emit taskAction->triggered();
            }
        } ui->prevTask->setDisabled(true);
    }
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
        (*results)[curTask - 1] = ui->lineEdit->text();
        ui->lineEdit->clear();
    }
    curTask = static_cast<QString>(tmp->text().split("№").last()).toInt();
    ui->groupBox->setTitle("Задание " + QString::number(curTask));
    engine->TeX2SVG((*tasks)[curTask - 1].first.first, true);
    ui->lineEdit->setValidator(new QRegExpValidator(getInstructions((*tasks)[curTask - 1].second.first), this));
    if ((*results)[curTask - 1] != "") {
        ui->lineEdit->setText((*results)[curTask - 1]);
    }

    if (tasks->size() > 1) {
        if (curTask  == 1) {
            ui->prevTask->setDisabled(true);
            ui->nextTask->setEnabled(true);
            ui->nextTask->setText("Вперед");
        } else if (curTask == tasks->size()) {
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
        (*results)[curTask - 1] = ui->lineEdit->text();
        ui->lineEdit->clear();
    }
    curTask = static_cast<QString>(ui->groupBox->title().split(" ").last()).toInt() - 1;
    ui->groupBox->setTitle("Задание " + QString::number(curTask));
    engine->TeX2SVG((*tasks)[curTask - 1].first.first, true);
    ui->lineEdit->setValidator(new QRegExpValidator(getInstructions((*tasks)[curTask - 1].second.first), this));
    if ((*results)[curTask - 1] != "") {
        ui->lineEdit->setText((*results)[curTask - 1]);
    }
    isPressed = false;
    ui->nextTask->setText("Вперед");
    if (curTask  != 1)
        ui->prevTask->setEnabled(true);
    else ui->prevTask->setDisabled(true);
}

void TestMode::on_nextTask_clicked()
{
    if (tasks->size() == 1 || tasks->size() == curTask) {
        int answer = QMessageBox::question(this, "Завершение тестирования",
                     "Вы действительно хотите завершить тест?");
        if (answer == QMessageBox::Yes) finishTest();
    } else {
        ui->prevTask->setEnabled(true);
        if (curTask != tasks->size()) {
            if (ui->lineEdit->text() != "") {
                (*results)[curTask - 1] = ui->lineEdit->text();
                ui->lineEdit->clear();
            }
            curTask = static_cast<QString>(ui->groupBox->title().split(" ").last()).toInt() + 1;
            if (curTask == tasks->size()) {
                isPressed = true;
                ui->nextTask->setText("Завершить");
            }
            ui->groupBox->setTitle("Задание " + QString::number(curTask));
            engine->TeX2SVG((*tasks)[curTask - 1].first.first, true);
            ui->lineEdit->setValidator(new QRegExpValidator(getInstructions((*tasks)[curTask - 1].second.first), this));
            if ((*results)[curTask - 1] != "") {
                ui->lineEdit->setText((*results)[curTask - 1]);
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
        (*results)[curTask - 1] = ui->lineEdit->text();
        ui->lineEdit->clear();
    }
    DialogResults *window = new DialogResults(this, tasks, results);
    window->setWindowTitle("Результаты теста");
    window->setModal(true);
    window->exec();
    close();
}

void TestMode::closeEvent(QCloseEvent *event){
    delete results;
    tasks->clear();
    tasks = nullptr;
    event->accept();
}
