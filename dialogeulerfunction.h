#ifndef DIALOGEULERFUNCTION_H
#define DIALOGEULERFUNCTION_H
#include <QPushButton>
#include <QDialog>
#include <QIcon>
#include "eulerfunction.h"

namespace Ui {
class DialogEulerFunction;
}

enum class EulerFunctionOptions {
    Default, Primes,
    PrimesDegrees, MultiplyPrimes,
    MultiplyPrimesDegrees
};

class DialogEulerFunction : public QDialog {
    Q_OBJECT
public:
    explicit DialogEulerFunction(QWidget *parent = nullptr, bool mode = false);
    ~DialogEulerFunction();
signals:
    void dialogEulerFunctionMeta(int);
    void dialogEulerFunction(int, int, int, EulerFunctionOptions);
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_btnDefault_clicked();
    void on_btnPrime_clicked();
    void on_btnPrimeDegree_clicked();
    void on_btnMultiplyPrimes_clicked();
    void on_btnMultiplyPrimesDegree_clicked();

private:
    int count; bool isCancelExist;
    Ui::DialogEulerFunction *ui;
};

#endif // DIALOGEULERFUNCTION_H
