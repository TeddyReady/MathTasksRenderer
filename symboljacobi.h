#ifndef SYMBOLJACOBI_H
#define SYMBOLJACOBI_H
#include "symbollegandre.h"

class SymbolJacobi : public SymbolLegandre {
public:
    explicit SymbolJacobi();
    explicit SymbolJacobi(int a, int p);

    int solve() const override;
};

#endif // SYMBOLJACOBI_H
