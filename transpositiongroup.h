#ifndef TRANSPOSITIONGROUP_H
#define TRANSPOSITIONGROUP_H

#include <QRandomGenerator>
#include <QString>
#include <QVector>
#include <QDebug>
#include <QMap>

#include "basemath.h"

enum class ViewMode {
    Standart, Cycle,
     Decomposition,
       Neighbors
};

class TranspositionGroup {
    QVector<QVector<int>> tp;
    ViewMode mode;
public:
    explicit TranspositionGroup();
    explicit TranspositionGroup(QVector<std::pair<int, int>>);
    explicit TranspositionGroup(const QString &str, int order);
    explicit TranspositionGroup(QVector<int>, int);
    TranspositionGroup operator *(TranspositionGroup &trans);
    TranspositionGroup operator ~();
    bool operator ==(const TranspositionGroup& trans);

    void setTask(int, ViewMode mode, bool identityForbidden = false);
    int getTask();
    QVector<QVector<int>>& getTransposition();
    ViewMode getViewMode();
    QString writeToMode(ViewMode, bool forTest = false);
    QString cycleType();
    int getHaos();
    QString getEven(bool forTest = false);
    int getOrder();
    TranspositionGroup simplify(int);
};

#endif // TRANSPOSITIONGROUP_H
