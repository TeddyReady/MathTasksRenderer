#ifndef DIALOGSET_H
#define DIALOGSET_H

#include <QRandomGenerator>
#include <QPushButton>
#include <QTextStream>
#include <QMessageBox>
#include <QVector>
#include <QString>
#include <QDialog>
#include <QFile>

enum class Set {
    N,  No, Z,  Z0,
    Q,  Q0, R,  R0,
    C,  Sn, Dn, An,
    V4, O8, Zn
};
enum class SetType {
    Set, Groupoid, Semigroup,
        Monoid, Group
};
using set_type = QVector<std::tuple<QString, QString, QString>>;

namespace Ui {
class DialogSet;
}

class DialogSet : public QDialog {
    Q_OBJECT
private:
    QString getCode(Set);
    QString getCode(SetType);
public:
    explicit DialogSet(QWidget *parent = nullptr, bool mode = false);
    ~DialogSet();
signals:
    void dialogSetMeta(int);
    void dialogSet(int, set_type *);
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
private:
    bool isCancelExist;
    set_type *data;
    Ui::DialogSet *ui;
};

#endif // DIALOGSET_H
