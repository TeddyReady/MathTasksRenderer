#ifndef DIALOGTESTTIMER_H
#define DIALOGTESTTIMER_H

#include <QDialog>
#include <QTime>

namespace Ui {
class DialogTestTimer;
}

class DialogTestTimer : public QDialog {
    Q_OBJECT
public:
    explicit DialogTestTimer(QWidget *parent = nullptr);
    ~DialogTestTimer();
signals:
    void timeRemaining(QTime);
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
private:
    Ui::DialogTestTimer *ui;
};

#endif // DIALOGTESTTIMER_H
