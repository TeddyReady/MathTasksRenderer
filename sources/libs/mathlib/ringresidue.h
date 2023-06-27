#ifndef RINGRESIDUE_H
#define RINGRESIDUE_H

#include "symbollegandre.h"
#include "eulerfunction.h"
#include "basemath.h"

enum class RingResidueOptions {
          GenCount, A_in_M,
    Order, A_X_equal_B, XX_equal_p,
            XX_equal_pq
};

enum class RingResidueType {
    Summary, Multiply, Undefined
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
    QString solveLinear(int a, int b, int mod) const;
    QString solveSimpleQuadro(int a, int mod) const;
    int getOrd(int num);
private:
    int n;
    RingResidueType type;
};

#endif // RINGRESIDUE_H
