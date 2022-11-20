#ifndef SYMBOLLEGANDRE_H
#define SYMBOLLEGANDRE_H
#include "basemath.h"

class SymbolLegandre {
public:
    explicit SymbolLegandre();
    explicit SymbolLegandre(int a, int p);

    int solve();
    void setTask(int a, int p);
    std::pair<int, int> getTask();
private:
    int a, p;
};

#endif // SYMBOLLEGANDRE_H
