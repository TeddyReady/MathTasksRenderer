#ifndef DIALOGMEBIUSFUNCTION_H
#define DIALOGMEBIUSFUNCTION_H
#include <QPushButton>
#include <QDialog>
#include "mebiusfunction.h"

namespace Ui {
class DialogMebiusFunction;
}

enum class MebiusFunctionOptions {
    Default, Primes,
    EvenPrimes, NotEvenPrimes,
    PrimeDegree
};

class DialogMebiusFunction : public QDialog {
    Q_OBJECT
public:
    explicit DialogMebiusFunction(QWidget *parent = nullptr, bool mode = false);
    ~DialogMebiusFunction();
signals:
    void dialogMebiusFunctionMeta(int);
    void dialogMebiusFunction(int, int, int, MebiusFunctionOptions);
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

    void on_btnDefault_clicked();
    void on_btnPrimes_clicked();
    void on_btnEvenPrimes_clicked();
    void on_btnNotEvenPrimes_clicked();
    void on_btnPrimeDegree_clicked();
private:
    int count; bool isCancelExist;
    Ui::DialogMebiusFunction *ui;
};

#endif // DIALOGMEBIUSFUNCTION_H
