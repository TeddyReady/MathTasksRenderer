#ifndef DIALOGSET_H
#define DIALOGSET_H

#include <QRandomGenerator>
#include <QPushButton>
#include <QTextStream>
#include <QMessageBox>
#include <QString>
#include <QDialog>
#include <QFile>

#include <vector>

enum class Set {
    N,  No, Z,  Z0,
    Q,  Q0, R,  R0,
    C,  Sn, Dn, An,
    V4, O8, Zn, Mm_Z,
    Mm_R, Mn_Z, Mn_R,
    _2A, Func_C, Func_D,
    Pn_X, ABS_C_, V_2D,
    V_nD
};

enum class SetType {
    Set, Groupoid, Semigroup,
        Monoid, Group
};

enum class SetOptions {
      Check, Oper,
    Abel, Associate,
        Neutral
};

using set_type = std::vector<std::tuple<QString, QString, QString>>;

namespace Ui {
class DialogSet;
}

class DialogSet : public QDialog {
    Q_OBJECT
private:
    void uploadUI();
    void uploadData();
    QString getCode(Set) const;
    QString getCode(SetType) const;
public:
    explicit DialogSet(QWidget *parent = nullptr, bool mode = false);
    ~DialogSet();
signals:
    void dialogSetMeta(int);
    void dialogSet(int, const set_type&);
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

    void on_btnCheck_clicked();
    void on_btnOper_clicked();
    void on_btnAbel_clicked();
    void on_btnAssociate_clicked();
    void on_btnNeutral_clicked();
private:
    static const int numOfSets, numOfOperators;
    bool isCancelExist;
    set_type data, baseData;
    Ui::DialogSet *ui;
};

#endif // DIALOGSET_H
