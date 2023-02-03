#ifndef MEBIUSFUNCTION_H
#define MEBIUSFUNCTION_H
#include "basemath.h"

enum class MebiusFunctionOptions {
    Default, Primes,
    EvenPrimes, NotEvenPrimes,
    PrimeDegree
};

class MebiusFunction {
public:
    explicit MebiusFunction();
    explicit MebiusFunction(int number);

    int solve() const;
    void setTask(int num) { number = num; }
    int getTask() const { return number; }
private:
    int number;
};

#endif // MEBIUSFUNCTION_H
