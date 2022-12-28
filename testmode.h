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
#include "dialogresults.h"

namespace Ui {
class TestMode;
}

class TestMode : public QMainWindow {
    Q_OBJECT
    void closeEvent(QCloseEvent *event);
public:
    explicit TestMode(QWidget *parent = nullptr, tasks_type *tasksForTest = nullptr);
    ~TestMode();
signals:
    void closeWindow();
private slots:
    void changeTask();
    void on_prevTask_clicked();
    void on_nextTask_clicked();
private:
    tasks_type* tasks;
    QVector<QString> *results;
    int curTask;
    bool isPressed;
    Ui::TestMode *ui;

    void finishTest();
};

#endif
