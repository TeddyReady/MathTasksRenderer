#ifndef EULERFUNCTION_H
#define EULERFUNCTION_H
#include "basemath.h"

enum class EulerFunctionOptions {
    Default, Primes,
    PrimesDegrees, MultiplyPrimes,
    MultiplyPrimesDegrees
};

class EulerFunction {
public:
    explicit EulerFunction();
    explicit EulerFunction(int number);

    int solve() const;
    void setTask(int num) { number = num; }
    int getTask() const { return number; }
private:
    int number;
};

#endif // EULERFUNCTION_H
