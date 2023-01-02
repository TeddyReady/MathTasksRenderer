#ifndef DIALOGSYMBOLJACOBI_H
#define DIALOGSYMBOLJACOBI_H
#include <QPushButton>
#include <QDialog>
#include "symboljacobi.h"

namespace Ui {
class DialogSymbolJacobi;
}

enum class SymbolJacobiOptions {
    Default, Primes,
    aEqual_1, aEqual2,
    NotEvenPrimes
};

class DialogSymbolJacobi : public QDialog {
    Q_OBJECT
public:
    explicit DialogSymbolJacobi(QWidget *parent = nullptr, bool mode = false);
    ~DialogSymbolJacobi();
signals:
    void dialogSymbolJacobiMeta(int);
    void dialogSymbolJacobi(int, std::pair<int, int>, std::pair<int, int>, SymbolJacobiOptions);
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

    void on_btnDefault_clicked();
    void on_btnPrimes_clicked();
    void on_btn1_clicked();
    void on_btn2_clicked();
    void on_btnNotEvenPrimes_clicked();

private:
    int count; bool isCancelExist;
    Ui::DialogSymbolJacobi *ui;
};

#endif // DIALOGSYMBOLJACOBI_H
