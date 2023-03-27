#ifndef DIALOGLATEXPRINTER_H
#define DIALOGLATEXPRINTER_H

#include "libs/mathlib/common.h"
#include "texengine.h"

namespace Ui {
class DialogLatexPrinter;
}

class DialogLatexPrinter : public QDialog {
    Q_OBJECT
    void printData();
public:
    explicit DialogLatexPrinter(const QString &data, const std::vector<QString> &answers, QWidget *parent = nullptr);
    ~DialogLatexPrinter();
private slots:
    void on_btnShow_clicked();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
private:
    QString data, answers;
    TeXEngine *engine, *engineAnswers;
    Ui::DialogLatexPrinter *ui;
};

#endif // DIALOGLATEXPRINTER_H
