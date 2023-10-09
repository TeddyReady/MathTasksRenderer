#ifndef REGISTERWIDGET_H
#define REGISTERWIDGET_H
#include "environment.h"

class RegisterWidget : public QWidget {
    Q_OBJECT
public:
    explicit RegisterWidget(size_t index, int value, QWidget *parent = nullptr);
    explicit RegisterWidget(QPair<size_t, int> reg, QWidget *parent = nullptr);

    void setValue(int value) {this->value = value;}
    size_t getIndex() {return index;}
    int getValue() {return value;}

    void setSize(QSize size);

protected:
    void paintEvent(QPaintEvent *event);

private:
    size_t index;
    int value;

    QFont font;
};

#endif // REGISTERWIDGET_H
