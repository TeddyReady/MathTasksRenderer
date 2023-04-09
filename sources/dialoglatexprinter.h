#ifndef DIALOGLATEXPRINTER_H
#define DIALOGLATEXPRINTER_H

#include "libs/mathlib/environment.h"
#include "texengine.h"

namespace Ui {
class DialogLatexPrinter;
}

class DialogLatexPrinter : public QDialog {
    Q_OBJECT
public:
    explicit DialogLatexPrinter(const QVector<QString> &descriptions, const QVector<QVector<QString>> &tasks,
                        const QVector<QString> &answers, QWidget *parent = nullptr);
    ~DialogLatexPrinter();
private slots:
    void on_btnShow_clicked();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
private:
    void printData();
    void showTasks();
    void showAnswers();

    QString tasksForView, taskFontSize, mathFontSize;
    QVector<QVector<QString>> tasks;
    QVector<QString> descriptions, answers;
    TeXEngine *engine, *engineAnswers;
    Ui::DialogLatexPrinter *ui;
};

#endif // DIALOGLATEXPRINTER_H
