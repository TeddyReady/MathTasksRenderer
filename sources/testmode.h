#ifndef TEST_MODE_H
#define TEST_MODE_H

#include "dialogtesttimer.h"
#include "dialogresults.h"
#include "texengine.h"

namespace Ui {
class TestMode;
}

class TestMode : public QMainWindow {
    Q_OBJECT
private:
    void finishTest();
    void closeEvent(QCloseEvent *event);
public:
    explicit TestMode(tasks_type &tasksForTest, QTime time, QWidget *parent = nullptr);
    ~TestMode();
signals:
    void closeWindow();
private slots:
    void changeTask();
    void on_prevTask_clicked();
    void on_nextTask_clicked();
    void updateTime();
private:
    tasks_type tasks;
    QVector<QString> results;
    int curTask;
    bool isPressed, timerExist;
    TeXEngine *engine;
    QTime allTime;
    QTimer *timer;
    Ui::TestMode *ui;
};

#endif
