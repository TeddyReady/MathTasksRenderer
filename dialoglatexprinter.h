#ifndef DIALOGLATEXPRINTER_H
#define DIALOGLATEXPRINTER_H

#include <QTextDocument>
#include <QMessageBox>
#include <QFileDialog>
#include <QPrinter>
#include <QString>
#include <QDialog>

#include "texengine.h"

namespace Ui {
class DialogLatexPrinter;
}

class DialogLatexPrinter : public QDialog {
    Q_OBJECT
    void printData();
public:
    explicit DialogLatexPrinter(QString data, QString answers, QWidget *parent = nullptr);
    ~DialogLatexPrinter();
private slots:
    void on_btnShow_clicked();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
private:
    QString data, answers;
    TeXEngine *engine;
    Ui::DialogLatexPrinter *ui;
};

#endif // DIALOGLATEXPRINTER_H
