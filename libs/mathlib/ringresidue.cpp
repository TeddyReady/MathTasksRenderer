#include "ringresidue.h"

RingResidue::RingResidue() : n(1) {}

int RingResidue::countOfGenerators() const
{
    return EulerFunction(n).solve();
}

int RingResidue::solve(int base, int degree) const
{
    return fastPower(base, degree) % n;
}
