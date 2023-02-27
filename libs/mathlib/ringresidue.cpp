#include "ringresidue.h"

RingResidue::RingResidue() : n(1), type(RingResidueType::Undefined) {}

QString RingResidue::getCode() const
{
    switch (type) {
    case RingResidueType::Summary:
        return QString("\\left(\\mathbb{Z}_{" + QString::number(n) + "}, +\\right)");
    case RingResidueType::Multiply:
        return QString("\\left(\\mathbb{Z}^*_{" + QString::number(n) + "}, \\cdot\\right)");
    default:
        return QString("-1");
    }
}

int RingResidue::countOfGenerators() const
{
    return EulerFunction(n).solve();
}

int RingResidue::solve(int base, int degree) const
{
    switch (type) {
    case RingResidueType::Summary:
        return (base * degree) % n;
    case RingResidueType::Multiply:
        return modulePower(base, degree, n);
    default:
        return -1;
    }
}

int RingResidue::getOrd(int num)
{
    num %= n;
    switch (type) {
    case RingResidueType::Summary:
        return LCM(num, n) / num;
    case RingResidueType::Multiply:
        for (int i = 1; i <= EulerFunction(n).solve(); ++i)
        {
            if (EulerFunction(n).solve() % i != 0) continue;
            if (solve(num, i) == 1)
                return i;
        } break;
    default:
        return -1;
    }
}
