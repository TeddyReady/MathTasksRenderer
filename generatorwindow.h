#ifndef GENERATORWINDOW_H
#define GENERATORWINDOW_H
#include <QtWebKitWidgets/QWebView>
#include <QMouseEvent>
#include <QTextStream>
#include <QFileDialog>
#include <QSettings>
#include <QDateTime>
#include <QUrl>

#include "dialogtranspositiongroup.h"
#include "dialogsymbollegandre.h"
#include "dialogmebiusfunction.h"
#include "dialogeulerfunction.h"
#include "dialogsymboljacobi.h"
#include "testmode.h"

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
private:
    void saveSettings();
    void uploadSettings();
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
    void on_comboBox_currentTextChanged(const QString &task);
    void on_tabWidget_currentChanged(int index);
    void on_pushButton_clicked();

private:
    int count;
    bool mode;
    QVector<QString> generatedTasks, generatedData;
    tasks_type *tasksForTest;
    QFile file;
    QRandomGenerator *random;
    QAction *testMode, *workMode;
    QSettings *settings;
    Ui::GeneratorWindow *ui;
};
#endif
