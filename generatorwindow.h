#ifndef GENERATORWINDOW_H
#define GENERATORWINDOW_H
#include <QMouseEvent>
#include <QTextStream>
#include <QFileDialog>
#include <QSettings>
#include <QDateTime>
#include <QCursor>
#include <QPixmap>
#include <QUrl>

#include "dialogtranspositiongroup.h"
#include "dialogsymbollegandre.h"
#include "dialogmebiusfunction.h"
#include "dialogeulerfunction.h"
#include "dialogsymboljacobi.h"
#include "dialogset.h"
#include "testmode.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GeneratorWindow; }
QT_END_NAMESPACE

enum AllTasks {
    TaskEulerFunction, TaskMebiusFunction,
    TaskSymbolLegandre, TaskSymbolJacobi,
    TaskTranspositionGroup, TaskSet
};

class GeneratorWindow : public QMainWindow {
    Q_OBJECT
private:
    void uploadUI();
    void saveSettings();
    void uploadSettings();
    void isReadyRender();
public:
    explicit GeneratorWindow(QWidget *parent = nullptr);
    ~GeneratorWindow();
public slots:
    //Эйлер
    void slotDialogEulerFunctionMeta(const int&);
    void slotDialogEulerFunction(const int&, const int&, const int&, const EulerFunctionOptions&);
    //Мёбиус
    void slotDialogMebiusFunctionMeta(const int&);
    void slotDialogMebiusFunction(const int&, const int&, const int&, const MebiusFunctionOptions&);
    //Лежандр
    void slotDialogSymbolLegandreMeta(const int&);
    void slotDialogSymbolLegandre(const int&, const std::pair<int, int>&, const std::pair<int, int>&, const SymbolLegandreOptions&);
    //Якоби
    void slotDialogSymbolJacobiMeta(const int&);
    void slotDialogSymbolJacobi(const int&, const std::pair<int, int>&, const std::pair<int, int>&, const SymbolJacobiOptions&);
    //Группа перестановок
    void slotDialogTranspositionGroupMeta(const int&);
    void slotDialogTranspositionGroup(const int&, const int&, const int&, const TranspositionGroupOptions&, const ViewMode&);
    //Алгебраические структуры
    void slotDialogSetMeta(const int&);
    void slotDialogSet(const int&, const set_type&, const SetOptions&);
private slots:
    void checkAnswers();
    void clearTasks();
    void startTest(const QTime&);
    void printTasks();
    void openManual();
    void on_genButton_clicked();
    void on_comboBox_currentTextChanged(const QString &task);
    void on_tabWidget_currentChanged(int index);
    void on_pushButton_clicked();   
private:
    int totalTestTasks, totalTaskCount, curTaskCount;
    bool mode;
    QVector<QString> generatedData;
    tasks_type tasksForTest;
    QString tasksForWork, solvedWorkTasks;
    QFile file;
    QRandomGenerator *random;
    QAction *testMode, *workMode;
    QSettings *settings;
    TeXEngine *engine;
    Ui::GeneratorWindow *ui;
};
#endif //GENERATORWINDOW_H
