#ifndef TRANSPOSITIONGROUP_H
#define TRANSPOSITIONGROUP_H
#include <QRandomGenerator>
#include <QString>
#include <QVector>
#include <QDebug>
#include <QMap>
#include "basemath.h"
#include <string>

enum class ViewMode {
    Standart, Cycle
};

class TranspositionGroup {
    QVector<QVector<int>> tp;
    ViewMode mode;
public:
    explicit TranspositionGroup();
    explicit TranspositionGroup(QVector<std::pair<int, int>>);
    explicit TranspositionGroup(const QString &str, int order);
    TranspositionGroup operator *(TranspositionGroup &trans);
    TranspositionGroup operator ~();
    bool operator ==(const TranspositionGroup& trans);

    void setTask(int, ViewMode mode);
    int getTask();
    QVector<QVector<int>>& getTransposition();
    ViewMode getViewMode();
    QString writeToMode(ViewMode, bool forTest = false);
    QString cycleType();
    int getHaos();
    QString getEven(bool forTest = false);
    int getOrder();
};

#endif // TRANSPOSITIONGROUP_H
