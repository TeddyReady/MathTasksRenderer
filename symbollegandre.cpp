#include "symbollegandre.h"

SymbolLegandre::SymbolLegandre() {}
SymbolLegandre::SymbolLegandre(int a, int p) : a(a), p(p) {}

int SymbolLegandre::solve(){
    if (a % p == 0) return 0;
    else if (a > p) {
        SymbolLegandre task(a % p, p);
        return task.solve();
    }
    else if (a == 1) return 1;
    else if (a == 2) return static_cast<int>(pow(-1, (p*p-1)/8));
    else if (a == -1) return static_cast<int>(pow(-1, (p-1)/2));
    else if (a < 0) {
        SymbolLegandre task1(-a, p), task2(-1, p);
        return (task1.solve() * task2.solve());
    } else if (!isPrime(a)) {
        int result = 1;
        auto data = decompositionToSimple(a);
        for (int i = 0; i < data.size(); i++) {
            if (data[i].second != 1) {
                for (int j = 0; j < data[i].second; j++) {
                    SymbolLegandre task(data[i].first, p);
                    result *= task.solve();
                }
            } else {
                SymbolLegandre task(data[i].first, p);
                result *= task.solve();
            }
        } return result;
    } else {
        SymbolLegandre task(p, a);
        return static_cast<int>(pow(-1, ((p-1)*(a-1))/4)) * task.solve();
    }
}

void SymbolLegandre::setTask(int a, int p){
    this->a = a;
    this->p = p;
}
std::pair<int, int> SymbolLegandre::getTask(){
    return std::make_pair(a, p);
}
