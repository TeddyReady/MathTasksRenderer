#ifndef REGISTERSPAINTER_H
#define REGISTERSPAINTER_H
#include "registerwidget.h"

class RegistersPainter : public QWidget {
    Q_OBJECT
public:
    explicit RegistersPainter(QWidget *parent = nullptr);
    ~RegistersPainter();

public slots:
    void updateRegisters(QPair<size_t, int> reg);
    void clearRegisters();

protected:
    void paintEvent(QPaintEvent *event);

private:
    QRect calculateRegLocation(int index);
    QFont font;
    QVector<RegisterWidget*> registers;
};

#endif // REGISTERSPAINTER_H
