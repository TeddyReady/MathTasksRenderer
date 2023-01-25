#ifndef DIALOGRESULTS_H
#define DIALOGRESULTS_H

#include <QStandardItemModel>
#include <QHeaderView>
#include <QDialog>
#include <QDebug>

#include "transpositiongroup.h"

enum class SupCommands {
    Number, Transposition, CycleType, Even,
       MultiTransposition, Name
};
QRegExp getInstructions(SupCommands command);
using tasks_type = QVector<std::tuple<QString, QString, SupCommands, int>>;

namespace Ui {
class DialogResults;
}

class DialogResults : public QDialog {
    Q_OBJECT
public:
    explicit DialogResults(const tasks_type &tasks, const QVector<QString> &results, QWidget *parent = nullptr);
    ~DialogResults();

private:
    int userCount, maxCount;
    Ui::DialogResults *ui;
};

#endif // DIALOGRESULTS_H
