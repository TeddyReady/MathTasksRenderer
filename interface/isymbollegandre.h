#ifndef SYMBOLLEGANDREINTERFACE_H
#define SYMBOLLEGANDREINTERFACE_H
#include "interface.h"
#include "symbollegandre.h"
#include "basemath.h"
#include "include.h"

enum class SymbolLegandreOptions {
    Default, Primes,
    aEqual_1, aEqual2,
    NotEvenPrimes
};

class SymbolLegandreInterface: public TaskInterface {
protected:
    std::pair<int, int> rangeA, rangeP;
    SymbolLegandreOptions option;
    QRandomGenerator *gen;
    std::unique_ptr<SymbolLegandre> symbol;

public:
    explicit SymbolLegandreInterface(std::pair<int, int> rangeA, std::pair<int, int> rangeP, SymbolLegandreOptions option);
    ~SymbolLegandreInterface();

    virtual void create();
    virtual QString description();
    QString task();
    QString answer();

    QString print();
};

class SymbolJacobiInterface: public SymbolLegandreInterface {
public:
    explicit SymbolJacobiInterface(std::pair<int, int> rangeA, std::pair<int, int> rangeP, SymbolLegandreOptions option);

    void create();
    QString description();
};

#endif // SYMBOLLEGANDREINTERFACE_H
