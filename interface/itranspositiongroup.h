#ifndef TRANSPOSITIONGROUPINTERFACE_H
#define TRANSPOSITIONGROUPINTERFACE_H
#include "interface.h"
#include "transpositiongroup.h"
#include "basemath.h"

enum class TranspositionGroupOptions {
       Write, Multiply,
    Inverse, Cycle, Mod2,
        Count, Order,
        Decomposition,
          Neighbor
};

class TranspositionGroupInterface : public TaskInterface {
private:
    int minNum, maxNum;
    TranspositionGroupOptions option;
    QRandomGenerator *gen;
    TranspositionGroup tp_1, tp_2;
    ViewMode viewMode;

public:
    explicit TranspositionGroupInterface(int minN, int maxN, TranspositionGroupOptions option, ViewMode mode);
    ~TranspositionGroupInterface();

    void create();
    QString description();
    QString task();
    QString answer();

    void createTransposition(TranspositionGroup &tp, int order = 0);
};

#endif // TRANSPOSITIONGROUPINTERFACE_H
