#ifndef RINGRESIDUEINTERFACE_H
#define RINGRESIDUEINTERFACE_H
#include "interface.h"
#include "ringresidue.h"
#include "basemath.h"

enum class RingResidueOptions {
    GenCount            ,
    A_in_M              ,
    Order               ,
    Count_A_X_equal_B   ,
    A_X_equal_B         ,
    Count_XX_equal_p   ,
    XX_equal_p          ,
    Count_XX_equal_pq   ,
    XX_equal_pq         ,
};

class RingResidueInterface : public TaskInterface {
private:
    int minNum, maxNum;
    RingResidueOptions option;
    QRandomGenerator *gen;
    ResidueType type;
    int n, module, optional;
    const char *operation;
    bool isGroup;

public:
    explicit RingResidueInterface(int minNum, int maxNum, RingResidueOptions option, ResidueType type);
    ~RingResidueInterface();

    QString printResidue(const char *operation) const;
    QString printAnswers(const std::vector<int> &answers) const;

    void create();
    QString description();
    QString task();
    QString answer();
};

#endif // RINGRESIDUEINTERFACE_H
