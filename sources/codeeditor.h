#ifndef CODEEDITOR_H
#define CODEEDITOR_H
#include "include.h"

class LineNumberArea;

class CodeEditor : public QPlainTextEdit {
    Q_OBJECT
public:
    explicit CodeEditor(QTextBrowser *debugger, QWidget *parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    void compile();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    bool Z(QVector<QPair<size_t, int>> &registers, size_t reg);
    bool S(QVector<QPair<size_t, int>> &registers, size_t reg);
    bool T(QVector<QPair<size_t, int>> &registers, size_t reg1, size_t reg2);
    int J(QVector<QPair<size_t, int>> &registers, size_t reg1, size_t reg2);

signals:
    void paintRegister(QPair<size_t, int> reg);
    void clearRegisters();

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);

private:
    QTextBrowser *debugger;
    QWidget *lineNumberArea;
};

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor), codeEditor(editor)
    {}

    QSize sizeHint() const override
    {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;
};

#endif // CODEEDITOR_H
