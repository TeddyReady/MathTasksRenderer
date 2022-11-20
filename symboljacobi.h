#ifndef SYMBOLJACOBI_H
#define SYMBOLJACOBI_H
#include "symbollegandre.h"
#include "basemath.h"

class SymbolJacobi {
public:
    explicit SymbolJacobi();
    explicit SymbolJacobi(int a, int p);

    int solve();
    void setTask(int a, int p);
    std::pair<int, int> getTask();
private:
    int a, p;
};

#endif // SYMBOLJACOBI_H
