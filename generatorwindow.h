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

#include "dialoglatexprinter.h"
#include "dialogbase.h"
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
    void isReadyRender();
public:
    explicit GeneratorWindow(QWidget *parent = nullptr);
    ~GeneratorWindow();
public slots:
    //Universal Dialog
    void receivedMetaInfo(int countOfTasks, bool isRepeatable, QString taskText);
    void receivedData(std::vector<int> data, AllTasks task, int subTask, ViewMode optional);
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
    //Run Algorithms
    void runEulerFunction(int, int, int, EulerFunctionOptions);
    void runMebiusFunction(int, int, int, MebiusFunctionOptions);
    void runSymbolLegandre(int, std::pair<int, int>, std::pair<int, int>, SymbolLegandreOptions);
    void runSymbolJacobi(int, std::pair<int, int>, std::pair<int, int>, SymbolJacobiOptions);
    void runTranspositionGroup(int, int, int, TranspositionGroupOptions, ViewMode);
    void runSet(int, SetOptions);

    int totalTestTasks, totalTaskCount, curTaskCount, TFWpastSize;
    bool mode;
    QVector<QString> generatedData;
    tasks_type tasksForTest;
    QString tasksForWork, solvedWorkTasks, taskBuffer;
    QRandomGenerator *random;
    QSettings *settings;
    TeXEngine *engine;
    Ui::GeneratorWindow *ui;
};
#endif //GENERATORWINDOW_H
