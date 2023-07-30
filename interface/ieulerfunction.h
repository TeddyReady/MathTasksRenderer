#ifndef EULERFUNCTIONINTERFACE_H
#define EULERFUNCTIONINTERFACE_H
#include "interface.h"
#include "eulerfunction.h"
#include "basemath.h"

enum class EulerFunctionOptions {
    Default, Primes,
    PrimesDegrees, MultiplyPrimes,
    MultiplyPrimesDegrees
};

class EulerFunctionInterface : public TaskInterface {
private:
    int minNum, maxNum;
    EulerFunctionOptions option;
    QRandomGenerator *gen;
    EulerFunction function;

public:
    explicit EulerFunctionInterface(int minNum, int maxNum, EulerFunctionOptions option);
    ~EulerFunctionInterface();

    void create();
    QString description();
    QString task();
    QString answer();
};

#endif // EULERFUNCTIONINTERFACE_H
