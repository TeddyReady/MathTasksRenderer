#ifndef DIALOGRESULTS_H
#define DIALOGRESULTS_H
#include <QDialog>
#include <QDebug>
#include "transpositiongroup.h"

enum class SupCommands {
    None, Transposition
};

using tasks_type = QVector<std::pair<std::pair<QString, QString>, SupCommands>>;

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
