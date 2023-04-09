#ifndef BASEMATH_H
#define BASEMATH_H
#include "metatype.h"

class BaseMath {
protected:
    virtual void create(const QVector<QVariant> &options) = 0;
    virtual QString description() = 0;
    virtual QString task(const QString &optional = "") = 0;
    virtual QString answer(const QVariant &other) = 0;
};

//Number decomposition to simple
std::vector<std::pair<int, int>> decompositionToSimple(int num);

//Prime check
bool isPrime(int num);

//Great Common Divisor
int GCD(int a, int b);

//Little Common Multiply
int LCM(int a, int b);

//Faster binary power
long fastPower(long num, long deg);

//Faster binary module power
long modulePower(long num, long deg, int module);

int max(int a, int b);

#endif // BASEMATH_H
