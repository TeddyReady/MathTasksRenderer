#include "turingmachine.h"

TuringMachine::TuringMachine(QWidget *parent) : QWidget(parent)
{
    debugger = new QTextBrowser(this);
    editor = new CodeEditor(debugger, this);
    painter = new RegistersPainter(this);

    connect(editor, &CodeEditor::paintRegister, painter, &RegistersPainter::updateRegisters);
    connect(editor, &CodeEditor::clearRegisters, painter, &RegistersPainter::clearRegisters);

    editor->show();
    debugger->show();
    painter->show();
}

TuringMachine::~TuringMachine()
{
    editor = nullptr;
}

void TuringMachine::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    editor->update();
    debugger->update();
    painter->update();

//    qDebug() << "Paint registers...";
//    QPainter *painter = new QPainter(this);
//    painter->setPen(QPen(QColor(70, 0, 0)));
//    painter->setBrush(QBrush(QColor(0, 0, 222)));

//    painter->drawRect(geometry());

//    painter->setPen(QPen(QColor(0, 0, 222)));
//    painter->setBrush(QBrush(QColor(70, 0, 0)));

//    painter->drawRect(this->painter->geometry());

//    painter->end();
//    delete painter;
}

void TuringMachine::resizeEvent(QResizeEvent *event)
{
    int indent = this->width() * 0.005;
    editor->setGeometry(x() + indent, y() + indent, width() * 0.25, height() * 0.66);
    debugger->setGeometry(x() + indent, y() + 2*indent + editor->height(), editor->width(), height() - 3*indent - editor->height());
    painter->setGeometry(x() + editor->width() + 2*indent, y() + indent, width() - x() - editor->width() - 3*indent, height() - 2*indent);

    qDebug() << this->geometry();
    qDebug() << editor->geometry();
    qDebug() << painter->geometry();
}

void TuringMachine::do_compile()
{
    editor->compile();
}

void TuringMachine::do_info()
{
    qDebug() << "do_info()...";
}
