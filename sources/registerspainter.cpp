#include "registerspainter.h"
#define MAX_COLS 5
#define MAX_ROWS 6

RegistersPainter::RegistersPainter(QWidget *parent)
    : QWidget(parent)
{
    font = QFont("Monospace");
}

RegistersPainter::~RegistersPainter()
{
    qDebug() << "Remove painter...";
}

void RegistersPainter::updateRegisters(QPair<size_t, int> reg)
{
//    qDebug() << "Updating registers..." << reg.first << reg.second;
    for (int i = 0; i < registers.size(); ++i)
    {
        if (registers.at(i)->getIndex() == reg.first)
        {
            registers[i]->setValue(reg.second);
            update();
            return;
        }
    }

    registers.append(new RegisterWidget(reg, this));
    update();
}

void RegistersPainter::clearRegisters()
{
    registers.clear();
}

void RegistersPainter::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter *painter = new QPainter(this);

    for (int i = 0; i < registers.size(); ++i)
    {
        registers.at(i)->setGeometry(calculateRegLocation(i));
        registers.at(i)->update();
    }

    painter->end();
    delete painter;
}

QRect RegistersPainter::calculateRegLocation(int index)
{
    int indent = width() * 0.01;
    int w = (width() - (MAX_COLS + 1) * indent) / MAX_COLS;
    int h = (height() - (MAX_ROWS + 1) * indent) / MAX_ROWS;
    int x = (index % MAX_COLS) * w + ((index % MAX_COLS) + 1) * indent;
    int y = ((index + 1) / MAX_ROWS) * h + (((index + 1) / MAX_ROWS) + 1) * indent;
    return QRect(x, y, w, h);
}
