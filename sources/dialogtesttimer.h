#ifndef DIALOGTESTTIMER_H
#define DIALOGTESTTIMER_H
#include "include.h"

namespace Ui {
class DialogTestTimer;
}

class DialogTestTimer : public QDialog {
    Q_OBJECT
public:
    explicit DialogTestTimer(QWidget *parent = nullptr);
    ~DialogTestTimer();
signals:
    void timeRemaining(QTime time);
private slots:
    void accept();
    void reject();
private:
    Ui::DialogTestTimer *ui;
};

#endif // DIALOGTESTTIMER_H
