#ifndef RINGRESIDUE_H
#define RINGRESIDUE_H

#include "libs/mathlib/eulerfunction.h"

enum class RingResidueOptions {
    GenCount, A_in_M,
    Order, A_X_equal_B,
};

enum class RingResidueType {
    Multiply, Summary, Undefined
};

class RingResidue {
public:
    explicit RingResidue();

    void setOrder(int n) { this->n = n; }
    int getOrder() const { return n; }
    void setType(RingResidueType type) { this->type = type; }
    QString getCode() const ;

    int countOfGenerators() const;
    int solve(int base, int degree) const;
    int getOrd(int num);
private:
    int n;
    RingResidueType type;
};

#endif // RINGRESIDUE_H
