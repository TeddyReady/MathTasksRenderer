#include "codeeditor.h"

CodeEditor::CodeEditor(QTextBrowser *debugger, QWidget *parent) : QPlainTextEdit(parent)
{
    this->debugger = debugger;
    lineNumberArea = new LineNumberArea(this);

    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
    connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

void CodeEditor::compile()
{
    debugger->clear();

    /* Parsing code */
    QString code = toPlainText().replace(" ", "").replace("\n", "");
    QStringList statements = code.split(";");

    /* Syntax check */
    QRegExp Z("^Z\\(\\d+\\)$"),S("^S\\(\\d+\\)$"), T("^T\\(\\d+,\\d+\\)$"),J("^J\\(\\d+,\\d+,\\d+\\)$");
    for (int i = 0; i < statements.size(); ++i)
    {
        if (statements.at(i) == "")
        {
            statements.removeAt(i);
            continue;
        }

        if (not
                (
            Z.exactMatch(statements.at(i)) ||
            S.exactMatch(statements.at(i)) ||
            T.exactMatch(statements.at(i)) ||
            J.exactMatch(statements.at(i))
                )
            )
        {
            debugger->setTextColor(QColor(165, 0, 0));
            debugger->append("Ошибка синтаксиса!");
            return;
        }
    }

    /* Running code */
    emit clearRegisters();
    QVector<QPair<size_t, int>> registers;
    QRegExp number("\\d+");
    for (int i = 0; i < statements.size(); ++i)
    {
        QString curState = statements.at(i);
        qDebug() << curState << i;
        if (Z.exactMatch(curState))
        {
            int pos = number.indexIn(curState);
            QStringList numbers = number.capturedTexts();
            this->Z(registers, numbers.first().toUInt());
        }
        else if (S.exactMatch(curState))
        {
            int pos = number.indexIn(curState);
            QStringList numbers = number.capturedTexts();
            if (not this->S(registers, numbers.first().toUInt()))
            {
                debugger->setTextColor(QColor(165, 0, 0));
                debugger->append(QString("Комманда суммирования выполнена неправильно! Регистр %1 не определен!").arg(numbers.first().toUInt()));
                return;
            }
        }
        else if (T.exactMatch(curState))
        {
            QStringList numbers;
            int pos = 0;

            while ((pos = number.indexIn(curState, pos)) != -1)
            {
                numbers << number.cap(0);
                pos += number.matchedLength();
            }

            if (not this->T(registers, numbers.first().toUInt(), numbers.at(1).toUInt()))
            {
                debugger->setTextColor(QColor(165, 0, 0));
                debugger->append("Ошибка команды присваивания! Один из регистров не существует");
                return;
            }
        }
        else if (J.exactMatch(curState))
        {
            QStringList numbers;
            int pos = 0;

            while ((pos = number.indexIn(curState, pos)) != -1)
            {
                numbers << number.cap(0);
                pos += number.matchedLength();
            }
            int result = this->J(registers, numbers.at(0).toUInt(),numbers.at(1).toUInt());
            if (result == -1)
            {
                debugger->setTextColor(QColor(165, 0, 0));
                debugger->append("Ошибка команды Jump! Один из регистров не существует!");
                return;
            }
            else if (result == 1)
            {
                i = numbers.at(2).toInt() - 1;
                qDebug() << numbers;
            }
        }
        else
        {
            qDebug() << "Captured undefined command - " << curState;
        }
    }

    foreach (auto pair, registers)
        if (pair.first == 0)
        {
            debugger->setTextColor(QColor(0, 165, 0));
            debugger->append(QString("Программа успешно завершена. Результат нулевого регистра: %1").arg(pair.second));
            return;
        }
    debugger->setTextColor(QColor(0, 165, 0));
    debugger->append(QString("Программа успешно завершена. Результат нулевого регистра: неопределено"));
}

bool CodeEditor::Z(QVector<QPair<size_t, int>> &registers, size_t reg)
{
    emit paintRegister(qMakePair(reg, 0));

    for (int i = 0; i < registers.size(); ++i)
        if (registers.at(i).first == reg)
        {
            registers[i].second = 0;
            return true;
        }

    registers.append(qMakePair(reg, 0));
    return true;
}

bool CodeEditor::S(QVector<QPair<size_t, int>> &registers, size_t reg)
{
    for (int i = 0; i < registers.size(); ++i)
        if (registers.at(i).first == reg)
        {
            ++registers[i].second;
            emit paintRegister(qMakePair(reg, registers[i].second));
            return true;
        }

    return false;
}
bool CodeEditor::T(QVector<QPair<size_t, int>> &registers, size_t reg1, size_t reg2)
{
    int counter = 0, value;
    size_t storage_index;
    for (int i = 0; i < registers.size(); ++i)
    {
        if (registers.at(i).first == reg1)
        {
            ++counter;
            value = registers.at(i).second;
        }
        if (registers.at(i).first == reg2)
        {
            ++counter;
            storage_index = i;
        }
        if (counter == 2) break;
    }

    if (counter < 2) return false;

    registers[storage_index].second = value;
    emit paintRegister(qMakePair(storage_index, registers[storage_index].second));
    return true;
}

int CodeEditor::J(QVector<QPair<size_t, int>> &registers, size_t reg1, size_t reg2)
{
    int counter = 0;
    int val1 = 0, val2 = 0;
    foreach (auto reg, registers)
    {
        if (reg.first == reg1)
        {
            ++counter;
            val1 = reg.second;
        }
        if (reg.first == reg2)
        {
            ++counter;
            val2 = reg.second;
        }
        if (counter == 2) break;
    }

    if (counter < 2)
        return -1;
    else if(val1 != val2)
        return 0;

    return 1;
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}
