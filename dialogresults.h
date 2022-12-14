#ifndef DIALOGRESULTS_H
#define DIALOGRESULTS_H
#include <QDialog>
#include <QDebug>

namespace Ui {
class DialogResults;
}

using tasks_type = QVector<std::pair<QString, QString>>;

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
