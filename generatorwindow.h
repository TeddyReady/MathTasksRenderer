#ifndef GENERATORWINDOW_H
#define GENERATORWINDOW_H
#include <QTextStream>
#include <QFileDialog>
#include <QMainWindow>
#include "dialogtranspositiongroup.h"
#include "dialogsymbollegandre.h"
#include "dialogmebiusfunction.h"
#include "dialogeulerfunction.h"
#include "dialogsymboljacobi.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GeneratorWindow; }
QT_END_NAMESPACE

enum AllTasks {
    TaskEulerFunction, TaskMebiusFunction,
    TaskSymbolLegandre, TaskSymbolJacobi,
    TaskTranspositionGroup
};

class GeneratorWindow : public QMainWindow {
    Q_OBJECT
public:
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
    void on_genButton_clicked();
    void on_actionTXT_triggered();
    void on_actionQuit_triggered();

private:
    QVector<QString> generatedTasks, generatedData;
    QFile file;
    QRandomGenerator *random;
    Ui::GeneratorWindow *ui;
};
#endif
