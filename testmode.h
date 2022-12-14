#ifndef TEST_MODE_H
#define TEST_MODE_H
#include <QListWidgetItem>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QLineEdit>
#include <QToolBar>
#include <QMainWindow>
#include "dialogresults.h"

namespace Ui {
class TestMode;
}

class TestMode : public QMainWindow {
    Q_OBJECT
public:
    explicit TestMode(QWidget *parent = nullptr, tasks_type *tasksForTest = nullptr);
    ~TestMode();

private slots:
    void changeTask();

    void on_prevTask_clicked();
    void on_nextTask_clicked();
    void on_action_triggered();

private:
    tasks_type* tasks;
    QVector<QString> *results;
    int curTask;
    Ui::TestMode *ui;
};

#endif
