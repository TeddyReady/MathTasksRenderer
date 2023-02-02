#ifndef DIALOGBASE_H
#define DIALOGBASE_H

#include <QPushButton>
#include <QHBoxLayout>
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

#include "transpositiongroup.h"
#include "symbollegandre.h"
#include "mebiusfunction.h"
#include "eulerfunction.h"

enum class AllTasks {
    EulerFunction, MebiusFunction,
    SymbolLegandre, SymbolJacobi,
    TranspositionGroup, Set
};

enum WidgetRole { Gen, Base };

namespace Ui {
class DialogBase;
}

class DialogBase : public QDialog {
    Q_OBJECT
public:
    explicit DialogBase(AllTasks task, bool deleteMode = true, QWidget *parent = nullptr);
    ~DialogBase();
signals:
    void sendingMetaInfo(int count, bool isRepeatable, QString taskText);
    void sendingData(std::vector<int> data, AllTasks task, int subTask, ViewMode optional);
private:
    void uploadUI(AllTasks task);
    void addItem(WidgetRole role,const QString &name = "", bool option = false);
    bool isRepeatable(AllTasks task) const ;
    QString getTaskText(AllTasks task) const ;

    std::vector<std::pair<int, int>> ranges;
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
    explicit GenWidget(const QString &optionName = "", QWidget *parent = nullptr);
    std::pair<int, int> getRange() const { return std::make_pair(sbMin->value(), sbMax->value()); }
private:
    QSpinBox *sbMin, *sbMax;
};

#endif // DIALOGBASE_H
