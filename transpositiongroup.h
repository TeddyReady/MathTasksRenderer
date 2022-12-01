#ifndef TRANSPOSITIONGROUP_H
#define TRANSPOSITIONGROUP_H
#include <QRandomGenerator>
#include <QVector>
#include <QDebug>

enum class ViewMode {
    Standart, Cycle
};

class TranspositionGroup {
    QVector<QVector<int>> tp;
    ViewMode mode;
public:
    explicit TranspositionGroup();

    QString writeToMode(ViewMode);

    void setTask(int, ViewMode mode);
    int getTask();
    ViewMode getViewMode();
};

#endif // TRANSPOSITIONGROUP_H
