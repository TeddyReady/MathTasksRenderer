#include "mebiusfunction.h"

MebiusFunction::MebiusFunction() : number(1) {}
MebiusFunction::MebiusFunction(int number) : number(number) {}

int MebiusFunction::solve() const {
    if (number == 1) return 1;
    else {
        auto data = decompositionToSimple(number);
        for (size_t i = 0; i < data.size(); ++i) {
            if (data[i].second > 1)
                return 0;
        }
        return pow(-1, data.size());
    }
}
