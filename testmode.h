#ifndef TEST_MODE_H
#define TEST_MODE_H
#include <QListWidgetItem>
#include <QTextBrowser>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QCloseEvent>
#include <QMessageBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QToolBar>
#include <QWebView>
#include <QRegExp>
#include <QTimer>

#include "qtmathjax/texengine.h"
#include "dialogtesttimer.h"
#include "dialogresults.h"

namespace Ui {
class TestMode;
}

class TestMode : public QMainWindow {
    Q_OBJECT
private:
    void finishTest();
    void closeEvent(QCloseEvent *event);
public:
    explicit TestMode(QWidget *parent = nullptr, tasks_type *tasksForTest = nullptr, QTime time = QTime(0, 0, 0, 0));
    ~TestMode();
signals:
    void closeWindow();
private slots:
    void changeTask();
    void on_prevTask_clicked();
    void on_nextTask_clicked();
    void updateTime();
private:
    tasks_type* tasks;
    QVector<QString> *results;
    int curTask;
    bool isPressed;
    TeXEngine *engine;
    QTime allTime;
    QTimer *timer;
    Ui::TestMode *ui;
};

#endif
