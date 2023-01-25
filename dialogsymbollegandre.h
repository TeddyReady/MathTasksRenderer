#ifndef DIALOGSYMBOLLEGANDRE_H
#define DIALOGSYMBOLLEGANDRE_H

#include <QPushButton>
#include <QDialog>

#include "symbollegandre.h"

namespace Ui {
class DialogSymbolLegandre;
}

enum class SymbolLegandreOptions {
    Default, Primes,
    aEqual_1, aEqual2,
    NotEvenPrimes
};

class DialogSymbolLegandre : public QDialog {
    Q_OBJECT
public:
    explicit DialogSymbolLegandre(QWidget *parent = nullptr, bool mode = false);
    ~DialogSymbolLegandre();
signals:
    void dialogSymbolLegandreMeta(const int&);
    void dialogSymbolLegandre(const int&, const std::pair<int, int>&, const std::pair<int, int>&, const SymbolLegandreOptions&);
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
    Ui::DialogSymbolLegandre *ui;
};

#endif // DIALOGSYMBOLLEGANDRE_H
