#ifndef DIALOGRESULTS_H
#define DIALOGRESULTS_H
#include <QStandardItemModel>
#include <QHeaderView>
#include <QDialog>
#include <QDebug>

#include "transpositiongroup.h"

enum class SupCommands {
    Number, Transposition, CycleType, Even
};
QRegExp getInstructions(SupCommands command);
using tasks_type = QVector<std::pair<std::pair<QString, QString>, std::pair<SupCommands, int>>>;

namespace Ui {
class DialogResults;
}

class DialogResults : public QDialog {
    Q_OBJECT
public:
    explicit DialogResults(QWidget *parent = nullptr, tasks_type *tasks = nullptr, QVector<QString> *results = nullptr);
    ~DialogResults();

private:
    int userCount, maxCount;
    Ui::DialogResults *ui;
};

#endif // DIALOGRESULTS_H
