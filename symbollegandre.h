#ifndef SYMBOLLEGANDRE_H
#define SYMBOLLEGANDRE_H
#include "basemath.h"

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

#endif // SYMBOLLEGANDRE_H
