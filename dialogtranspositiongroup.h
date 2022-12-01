#ifndef DIALOGTRANSPOSITIONGROUP_H
#define DIALOGTRANSPOSITIONGROUP_H
#include <QDialog>
#include "transpositiongroup.h"

namespace Ui {
class DialogTranspositionGroup;
}

enum class TranspositionGroupOptions {
       Write, Multiply,
    Inverse,  Order, Mod2,
        Cycle, Count
};

class DialogTranspositionGroup : public QDialog {
    Q_OBJECT
public:
    explicit DialogTranspositionGroup(QWidget *parent = nullptr);
    ~DialogTranspositionGroup();
signals:
    void dialogTranspositionGroupMeta(int);
    void dialogTranspositionGroup(int, int, int, TranspositionGroupOptions);
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
private:
    int count;
    Ui::DialogTranspositionGroup *ui;
};

#endif // DIALOGTRANSPOSITIONGROUP_H
