#ifndef RINGRESIDUE_H
#define RINGRESIDUE_H

#include "libs/mathlib/eulerfunction.h"

enum class RingResidueOptions {
    GenCount, A_in_M,
    Order, A_X_equal_B,
};

class RingResidue {
public:
    explicit RingResidue();

    void setOrder(int n) { this->n = n; }
    int getOrder() const { return n; }

    int countOfGenerators() const;
    int solve(int base, int degree) const;
private:
    int n;
};

#endif // RINGRESIDUE_H
