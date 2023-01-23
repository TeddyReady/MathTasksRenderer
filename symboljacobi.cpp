#include "symboljacobi.h"

SymbolJacobi::SymbolJacobi() : SymbolLegandre() {}
SymbolJacobi::SymbolJacobi(int a, int p) : SymbolLegandre(a, p) {}

int SymbolJacobi::solve() const {
    int result = 1;
    auto data = decompositionToSimple(p);
    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i].second != 1) {
            for (size_t j = 0; j < data[i].second; ++j) {
                SymbolLegandre task(a, data[i].first);
                result *= task.solve();
            }
        } else {
            SymbolLegandre task(a, data[i].first);
            result *= task.solve();
        }
    } return result;
}
