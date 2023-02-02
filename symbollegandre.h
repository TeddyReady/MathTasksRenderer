#ifndef SYMBOLLEGANDRE_H
#define SYMBOLLEGANDRE_H
#include "basemath.h"

enum class SymbolLegandreOptions {
    Default, Primes,
    aEqual_1, aEqual2,
    NotEvenPrimes
};

class SymbolLegandre {
public:
    explicit SymbolLegandre();
    explicit SymbolLegandre(int a, int p);

    void setTask(int a, int p) { this->a = a; this->p = p; }
    std::pair<int, int> getTask() const { return std::make_pair(a, p); }
    virtual int solve() const;
protected:
    int a, p;
};

enum class SymbolJacobiOptions {
    Default, Primes,
    aEqual_1, aEqual2,
    NotEvenPrimes
};

class SymbolJacobi : public SymbolLegandre {
public:
    explicit SymbolJacobi();
    explicit SymbolJacobi(int a, int p);

    int solve() const override;
};

#endif // SYMBOLLEGANDRE_H
