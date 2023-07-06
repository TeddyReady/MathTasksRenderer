#include "ringresidue.h"

RingResidue::RingResidue() : n(1), type(RingResidueType::Undefined) {}

QString RingResidue::getCode() const
{
    switch (type) {
    case RingResidueType::Summary:
        return QString("\\mathbb{Z}_{" + QString::number(n) + "}");
    case RingResidueType::Multiply:
        return QString("\\mathbb{Z}_{" + QString::number(n) + "}^*");
    default:
        return QString("\\mathbb{Z}_{" + QString::number(n) + "}");
    }
}

int RingResidue::countOfGenerators() const
{
    if (type == RingResidueType::Summary) {
        return EulerFunction(n).solve();
    } else if (type == RingResidueType::Multiply) {
        return EulerFunction(EulerFunction(n).solve()).solve();
    } else return -1;
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

QString RingResidue::solveLinear(int a, int b, int mod) const
{
    QString result;
    int d = GCD(a, mod);
    if (b % d != 0)
        result = "Нет корней";
    else if (d == 1)
        result = QString::number((b * modulePower(a, EulerFunction(mod).solve() - 1, mod)) % mod);
    else {
        int curAnswer = solveLinear(a / d, b / d, mod / d).toInt();
        for (int i = 0; i < d; ++i)
        {
            result += QString::number(curAnswer + (mod / d) * i);
            if (i + 1 != d)
                result += ",~";
        }

    } return result;
}

QString RingResidue::solveSimpleQuadro(int a, int mod) const
{
    QString result; int n;
    // Ищем квадратичный невычет
    for (int i = 2; i < mod; ++i)
    {
        if (SymbolLegandre(i, mod).solve() == -1)
        {
            n = i;
            break;
        }
    }
    if (SymbolLegandre(a, mod).solve() == -1) result = "Нет корней";
    else if (SymbolLegandre(a, mod).solve() == 0) result = "0";
    else if ((a - 1) % mod == 0) result = "\\pm 1";
    else if ((mod - 3) % 4 == 0) result = "\\pm " + QString::number(modulePower(a, (mod + 1) / 4, mod));
    else {
        int inv_a = modulePower(a, EulerFunction(mod).solve() - 1, mod);
        int cntOf_2 = 0; int s = mod - 1;
        while (s % 2 == 0)
        {
            s /= 2;
            ++cntOf_2;
        }
        int b = modulePower(n, s, mod);
        int r = modulePower(a, (s + 1)/2, mod);
        //for (int k = 0; k <= cntOf_2 - 2; ++k)
        result = QString::number(inv_a);
    } return result;
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
        } return -100;
    default:
        return -1;
    }
}
