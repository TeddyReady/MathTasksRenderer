#ifndef GENERATORWINDOW_H
#define GENERATORWINDOW_H
#include "dialoglatexprinter.h"
#include "dialogfontsize.h"
#include "turingmachine.h"
#include "dialogmanual.h"
#include "environment.h"
#include "dialogbase.h"
#include "pdfviewer.h"
#include "testmode.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GeneratorWindow; }
QT_END_NAMESPACE

class GeneratorWindow : public QMainWindow {
    Q_OBJECT
private:
    void uploadUI();
    void saveSettings();
    void uploadSettings();
    void getConnection();
    void isReadyRender();
public:
    explicit GeneratorWindow(QWidget *parent = nullptr);
    ~GeneratorWindow();
protected:
    void closeEvent(QCloseEvent *event);
public slots:
    void receivedMetaInfo(int countOfTasks);
    void receivedData(std::vector<int> data, AllTasks task, int subTask, int optional);

private slots:
    void checkAnswers();
    void clearTasks();
    void printTasks();
    void changeFontSize();
    void openManual();

    void startTest(QTime time);
    void generateTask();
    void switchTask(const QString &task);
    void switchTab(int index);
    void runTest();
private:
    //Main parser
    void runTaskManager(const QString &task, bool closeMode);

    int totalTestTasks, lastSizeCount;
    bool mode;
    QVector<QString> descriptions, answers;
    QVector<QVector<QString>> tasks;
    tasks_type tasksForTest;
    QString taskForTeX, taskFontSize, mathFontSize;
    QRandomGenerator *random;
    TeXEngine *engine;
    Ui::GeneratorWindow *ui;
};

#endif //GENERATORWINDOW_H
