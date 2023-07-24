#ifndef MEBIUSFUNCTIONINTERFACE_H
#define MEBIUSFUNCTIONINTERFACE_H
#include "interface.h"
#include "mebiusfunction.h"
#include "basemath.h"

enum class MebiusFunctionOptions {
    Default, Primes,
    EvenPrimes, NotEvenPrimes,
    PrimeDegree
};

class MebiusFunctionInterface : public TaskInterface {
private:
    int minNum, maxNum;
    MebiusFunctionOptions option;
    QRandomGenerator *gen;
    MebiusFunction function;

public:
    explicit MebiusFunctionInterface(int minNum, int maxNum, MebiusFunctionOptions option);
    ~MebiusFunctionInterface();

    void create();
    QString description();
    QString task();
    QString answer();
};

#endif // MEBIUSFUNCTIONINTERFACE_H
