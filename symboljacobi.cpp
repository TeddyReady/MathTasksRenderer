#include "symboljacobi.h"

SymbolJacobi::SymbolJacobi() {}
SymbolJacobi::SymbolJacobi(int a, int p) : a(a), p(p) {}

int SymbolJacobi::solve(){
    int result = 1;
    auto data = decompositionToSimple(p);
    for (int i = 0; i < data.size(); i++) {
        if (data[i].second != 1) {
            for (int j = 0; j < data[i].second; j++) {
                SymbolLegandre task(a, data[i].first);
                result *= task.solve();
            }
        } else {
            SymbolLegandre task(a, data[i].first);
            result *= task.solve();
        }
    } return result;
}

void SymbolJacobi::setTask(int a, int p){
    this->a = a;
    this->p = p;
}
std::pair<int, int> SymbolJacobi::getTask(){
    return std::make_pair(a, p);
}
