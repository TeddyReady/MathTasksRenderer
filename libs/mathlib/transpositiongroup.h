#ifndef TRANSPOSITIONGROUP_H
#define TRANSPOSITIONGROUP_H
#include "basemath.h"

enum class TranspositionGroupOptions {
       Write, Multiply,
    Inverse,  Cycle, Mod2,
        Count, Order,
        Decomposition,
          Neighbor
};

enum class ViewMode {
    Standart, Cycle,
     Decomposition,
    Neighbors, None
};

class TranspositionGroup {
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
private:
    QVector<QVector<int>> tp;
    ViewMode mode;
};

#endif // TRANSPOSITIONGROUP_H
