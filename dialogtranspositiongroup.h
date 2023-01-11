#ifndef DIALOGTRANSPOSITIONGROUP_H
#define DIALOGTRANSPOSITIONGROUP_H
#include <QPushButton>
#include <QMouseEvent>
#include <QDialog>
#include <QMenu>
#include "transpositiongroup.h"

namespace Ui {
class DialogTranspositionGroup;
}

enum class TranspositionGroupOptions {
       Write, Multiply,
    Inverse,  Order, Mod2,
        Cycle, Count,
        Decomposition,
          Neighbor
};

class DialogTranspositionGroup : public QDialog {
    Q_OBJECT
public:
    explicit DialogTranspositionGroup(QWidget *parent = nullptr, bool mode = false);
    ~DialogTranspositionGroup();
signals:
    void dialogTranspositionGroupMeta(int);
    void dialogTranspositionGroup(int, int, int, TranspositionGroupOptions, ViewMode);
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

    void on_btnWrite_clicked();
    void on_btnMultiply_clicked();
    void on_btnInverse_clicked();
    void on_btnCycle_clicked();
    void on_btnMod2_clicked();
    void on_btnCount_clicked();
    void on_btnOrder_clicked();
    void on_btnDec_clicked();

    void on_btnNeighbor_clicked();

private:
    int count; bool isCancelExist;
    ViewMode taskWrite, taskMultiply, taskInverse, taskOrder, taskMod2, taskCycle, taskCount, taskDec, taskNeighbor;
    Ui::DialogTranspositionGroup *ui;
};

#endif // DIALOGTRANSPOSITIONGROUP_H
