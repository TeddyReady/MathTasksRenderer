#ifndef TURINGMACHINE_H
#define TURINGMACHINE_H
#include "registerspainter.h"
#include "codeeditor.h"

class TuringMachine : public QWidget {
    Q_OBJECT
public:
    explicit TuringMachine(QWidget *parent = nullptr);
    ~TuringMachine();

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
public slots:
    void do_compile();
    void do_info();

private:
    CodeEditor *editor;
    QTextBrowser *debugger;
    RegistersPainter *painter;
};

#endif // TURINGMACHINE_H
