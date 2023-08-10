#ifndef DIALOGBASE_H
#define DIALOGBASE_H
#include "itranspositiongroup.h"
#include "iringofmembers.tpp"
#include "isymbollegandre.h"
#include "imebiusfunction.h"
#include "ieulerfunction.h"
#include "iringresidue.h"
#include "imatrix.tpp"
#include "environment.h"

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
    void sendingMetaInfo(int count);
    void sendingData(std::vector<int> data, AllTasks task, int subTask, int optional);
protected slots:
    void accept();
    void reject();
private:
    void uploadUI();
    void addItem(WidgetRole role, const QString &name = "", ExoticOption type = ExoticOption::None);
    bool isHaveMoreGens();

    AllTasks task;
    bool deleteMode, isNeedEmptyColumn;
    std::vector<std::pair<int, int>> ranges;
    std::vector<BaseWidget*> widgets;
    Ui::DialogBase *ui;
};

class BaseWidget : public QWidget {
    Q_OBJECT
public:
    explicit BaseWidget(const QString &cbName, ExoticOption type = ExoticOption::None, QWidget *parent = nullptr);

    void setExoticOptions(const ExoticOption &type);
    int getCount() const { return sb->value(); }
    int getExoticOption() const { return exoticOption; }
    bool isChecked() const { return cb->isChecked(); }
    QCheckBox* getCheckBox() const { return cb; }
    QSpinBox* getSpinBox() const { return sb; }
    QPushButton* getPushButton() const { return pb; }
private:
    QCheckBox *cb;
    QSpinBox *sb;
    QPushButton *pb;
    int exoticOption;
    ExoticOption type;
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
