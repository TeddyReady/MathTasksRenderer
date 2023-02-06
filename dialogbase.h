#ifndef DIALOGBASE_H
#define DIALOGBASE_H

#include <QPushButton>
#include <QGridLayout>
#include <QSpacerItem>
#include <QCloseEvent>
#include <QCheckBox>
#include <QSpinBox>
#include <QString>
#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QMenu>

#include <vector>

#include "libs/mathlib/transpositiongroup.h"
#include "libs/mathlib/algebrastructures.h"
#include "libs/mathlib/groupproperties.h"
#include "libs/mathlib/symbollegandre.h"
#include "libs/mathlib/mebiusfunction.h"
#include "libs/mathlib/eulerfunction.h"
#include "libs/mathlib/matrix.tpp"

enum class AllTasks {
    EulerFunction, MebiusFunction,
    SymbolLegandre, SymbolJacobi,
    TranspositionGroup, Set,
    GroupProperties, Matrix
};

enum WidgetRole { Gen, Base };

namespace Ui {
class DialogBase;
}

class BaseWidget;
class DialogBase : public QDialog {
    Q_OBJECT
public:
    explicit DialogBase(AllTasks task, bool deleteMode = true, QWidget *parent = nullptr);
    ~DialogBase();
signals:
    void sendingMetaInfo(int count, bool isRepeatable, QString taskText);
    void sendingData(std::vector<int> data, AllTasks task, int subTask, ViewMode optional);
private:
    void uploadUI();
    void addItem(WidgetRole role, const QString &name = "", bool option = false);
    bool isRepeatable() const ;
    bool isHaveMoreGens();
    QString getTaskText() const ;

    AllTasks task;
    std::vector<std::pair<int, int>> ranges;
    std::vector<BaseWidget*> widgets;
    Ui::DialogBase *ui;
};

class BaseWidget : public QWidget {
    Q_OBJECT
public:
    explicit BaseWidget(const QString &cbName, bool option, QWidget *parent = nullptr);

    int getCount() const { return sb->value(); }
    ViewMode getViewMode() const { return vm; }
    bool isChecked() const { return cb->isChecked(); }
    bool isTransposition() const { return isTP; }
    QCheckBox* getCheckBox() const { return cb; }
    QSpinBox* getSpinBox() const { return sb; }
    QPushButton* getPushButton() const { return pb; }
private:
    QCheckBox *cb;
    QSpinBox *sb;
    QPushButton *pb;
    ViewMode vm;
    bool isTP;
};

class GenWidget : public QWidget {
    Q_OBJECT
public:
    explicit GenWidget(AllTasks task, const QString &optionName = "", QWidget *parent = nullptr);
    std::pair<int, int> getRange() const { return std::make_pair(sbMin->value(), sbMax->value() + 1); }
private:
    void loadSettings(AllTasks task, const QString &optionName);

    QSpinBox *sbMin, *sbMax;
};

#endif // DIALOGBASE_H
