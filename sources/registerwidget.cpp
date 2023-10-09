#include "registerwidget.h"

RegisterWidget::RegisterWidget(size_t index, int value, QWidget *parent)
    : QWidget(parent), index(index), value(value)
{
    font = QFont("Monospace");
    show();
}

RegisterWidget::RegisterWidget(QPair<size_t, int> reg, QWidget *parent)
    : RegisterWidget(reg.first, reg.second, parent) {}

void RegisterWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter *painter = new QPainter(this);
    painter->setFont(font);

    qreal radius = width() * 0.05;

    painter->setPen(QPen(QColor(30, 30, 30)));
    painter->setBrush(QBrush(QColor(222, 222, 222)));
    painter->drawRoundedRect(0, 0, width(), height(), radius, radius);

    QRect label = QRect(0, 0, width(), height() * 0.3);
    painter->setPen(QPen(QColor(30, 30, 30)));
    painter->setBrush(QBrush(QColor(96, 96, 96)));
    painter->drawRoundedRect(label, radius, radius);

    QString text = QString("Регистр %1").arg(index);
    painter->setPen(QPen(QColor(225, 225, 225)));
    painter->drawText((width() -  QFontMetrics(font).horizontalAdvance(text))/2,
                      height()/4.5, text);

    painter->setPen(QPen(QColor(67, 67, 67)));
    painter->drawText((width() -  QFontMetrics(font).horizontalAdvance(text))/2,
                      height()/1.5, QString::number(value));

    painter->end();
    delete painter;
}
