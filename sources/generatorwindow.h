#ifndef GENERATORWINDOW_H
#define GENERATORWINDOW_H

#include "libs/mathlib/environment.h"
#include "dialoglatexprinter.h"
#include "dialogfontsize.h"
#include "dialogmanual.h"
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
    void getConnection();
    void isReadyRender();
    void createTheoryImages();
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

    void prevTheoryPage();
    void nextTheoryPage();
private:
    //Main parser
    void runTaskManager(const QString &task, bool closeMode);
    //Run Algorithms
    void runEulerFunction(int, int, int, EulerFunctionOptions);
    void runMebiusFunction(int, int, int, MebiusFunctionOptions);
    void runSymbolLegandre(int, std::pair<int, int>, std::pair<int, int>, SymbolLegandreOptions);
    void runSymbolJacobi(int, std::pair<int, int>, std::pair<int, int>, SymbolJacobiOptions);
    void runTranspositionGroup(int, int, int, TranspositionGroupOptions, ViewMode);
    void runSet(int, SetOptions);
    void runGroupProperties(int, int, int, GroupPropertiesOptions);
    void runMatrix(int, std::pair<int, int>, std::pair<int, int>, MatrixOptions);
    void runRingResidue(int, int, int, RingResidueOptions);
    void runRingOfMembers(int, std::pair<int, int>, std::pair<int, int>, RingOfMembersOptions);

    int totalTestTasks, lastSizeCount, pageNumber;
    bool mode;
    QVector<QString> descriptions, answers;
    QVector<QVector<QString>> tasks;
    tasks_type tasksForTest;
    QString taskForTeX, taskFontSize, mathFontSize;
    QRandomGenerator *random;
    TeXEngine *engine;
    QGraphicsScene *scene;
    QVector<QImage> images;
    Ui::GeneratorWindow *ui;
};

#endif //GENERATORWINDOW_H
