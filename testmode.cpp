#include "testmode.h"
#include "ui_testmode.h"

TestMode::TestMode(QWidget *parent, tasks_type* tasksForTest) :
    QMainWindow(parent),
    ui(new Ui::TestMode)
{
    setWindowTitle("Тест");
    ui->setupUi(this);
    tasks = tasksForTest;
    results = new QVector<QString>;
    for (int i = 0; i < tasks->size(); i++) {
        results->push_back("");
        QAction *taskAction = new QAction("№" + QString::number(i + 1), ui->toolBar);
        ui->toolBar->addAction(taskAction);
        connect(taskAction, SIGNAL(triggered()), this, SLOT(changeTask()));
        if (i == 0) {
            emit taskAction->triggered();
        }
    }
}

TestMode::~TestMode()
{
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
    ui->taskView->clear();
    ui->taskView->append("<!Doctype HTML>");
    ui->taskView->append((*tasks)[curTask - 1].first);
    if ((*results)[curTask - 1] != "") {
        ui->lineEdit->setText((*results)[curTask - 1]);
    }

    if (tasks->size() == 1) {
        ui->prevTask->setHidden(true);
        ui->nextTask->setHidden(true);
    } else {
        if (curTask  == 1) {
            ui->prevTask->setDisabled(true);
            ui->nextTask->setEnabled(true);
        } else if (curTask == tasks->size()) {
            ui->prevTask->setEnabled(true);
            ui->nextTask->setDisabled(true);
        } else {
            ui->prevTask->setEnabled(true);
            ui->nextTask->setEnabled(true);
        }
    }
}

void TestMode::on_prevTask_clicked()
{
    if (ui->lineEdit->text() != "") {
        (*results)[curTask - 1] = ui->lineEdit->text();
        ui->lineEdit->clear();
    }
    curTask = static_cast<QString>(ui->groupBox->title().split(" ").last()).toInt() - 1;
    ui->groupBox->setTitle("Задание " + QString::number(curTask));
    ui->taskView->clear();
    ui->taskView->append("<!Doctype HTML>");
    ui->taskView->append((*tasks)[curTask - 1].first);
    if ((*results)[curTask - 1] != "") {
        ui->lineEdit->setText((*results)[curTask - 1]);
    }

    if (tasks->size() == 1) {
        ui->prevTask->setHidden(true);
        ui->nextTask->setHidden(true);
    } else {
        if (curTask  == 1) {
            ui->prevTask->setDisabled(true);
            ui->nextTask->setEnabled(true);
        } else if (curTask == tasks->size()) {
            ui->prevTask->setEnabled(true);
            ui->nextTask->setDisabled(true);
        } else {
            ui->prevTask->setEnabled(true);
            ui->nextTask->setEnabled(true);
        }
    }
}

void TestMode::on_nextTask_clicked()
{
    if (ui->lineEdit->text() != "") {
        (*results)[curTask - 1] = ui->lineEdit->text();
        ui->lineEdit->clear();
    }
    curTask = static_cast<QString>(ui->groupBox->title().split(" ").last()).toInt() + 1;
    ui->groupBox->setTitle("Задание " + QString::number(curTask));
    ui->taskView->clear();
    ui->taskView->append("<!Doctype HTML>");
    ui->taskView->append((*tasks)[curTask - 1].first);
    if ((*results)[curTask - 1] != "") {
        ui->lineEdit->setText((*results)[curTask - 1]);
    }

    if (tasks->size() == 1) {
        ui->prevTask->setHidden(true);
        ui->nextTask->setHidden(true);
    } else {
        if (curTask  == 1) {
            ui->prevTask->setDisabled(true);
            ui->nextTask->setEnabled(true);
        } else if (curTask == tasks->size()) {
            ui->prevTask->setEnabled(true);
            ui->nextTask->setDisabled(true);
        } else {
            ui->prevTask->setEnabled(true);
            ui->nextTask->setEnabled(true);
        }
    }
}

void TestMode::on_action_triggered()
{
    if (ui->lineEdit->text() != "") {
        (*results)[curTask - 1] = ui->lineEdit->text();
        ui->lineEdit->clear();
    }
    DialogResults *window = new DialogResults(this, tasks, results);
    window->setModal(true);
    window->exec();
    tasks->clear();
    close();
}
