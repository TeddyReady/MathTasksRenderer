#ifndef GENERATORWINDOW_H
#define GENERATORWINDOW_H
#include <QMouseEvent>
#include <QTextStream>
#include <QFileDialog>
#include <QDateTime>

#include "dialogtranspositiongroup.h"
#include "dialogsymbollegandre.h"
#include "dialogmebiusfunction.h"
#include "dialogeulerfunction.h"
#include "dialogsymboljacobi.h"
#include "testmode.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GeneratorWindow; }
QT_END_NAMESPACE

enum WorkMode {
    Test, Work
};

enum AllTasks {
    TaskEulerFunction, TaskMebiusFunction,
    TaskSymbolLegandre, TaskSymbolJacobi,
    TaskTranspositionGroup
};

class GeneratorWindow : public QMainWindow {
    Q_OBJECT
public:
    QPoint p;
    explicit GeneratorWindow(QWidget *parent = nullptr);
    ~GeneratorWindow();
public slots:
    //Эйлер
    void slotDialogEulerFunctionMeta(int);
    void slotDialogEulerFunction(int, int, int, EulerFunctionOptions);
    //Мёбиус
    void slotDialogMebiusFunctionMeta(int);
    void slotDialogMebiusFunction(int, int, int, MebiusFunctionOptions);
    //Лежандр
    void slotDialogSymbolLegandreMeta(int);
    void slotDialogSymbolLegandre(int, std::pair<int, int>, std::pair<int, int>, SymbolLegandreOptions);
    //Якоби
    void slotDialogSymbolJacobiMeta(int);
    void slotDialogSymbolJacobi(int, std::pair<int, int>, std::pair<int, int>, SymbolJacobiOptions);
    //Группа перестановок
    void slotDialogTranspositionGroupMeta(int);
    void slotDialogTranspositionGroup(int, int, int, TranspositionGroupOptions, ViewMode);
private slots:
    //Режимы работы
    void runTestMode();
    void runWorkMode();

    void on_genButton_clicked();
    void on_actionTXT_triggered();
    void on_actionQuit_triggered();

private:
    int count;
    WorkMode mode;
    QVector<QString> generatedTasks, generatedData;
    tasks_type *tasksForTest;
    QFile file;
    QRandomGenerator *random;
    QAction *testMode, *workMode;
    Ui::GeneratorWindow *ui;
};
#endif
