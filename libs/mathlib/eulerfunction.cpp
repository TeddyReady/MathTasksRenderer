#include "eulerfunction.h"
EulerFunction::EulerFunction() : number(1) {}
EulerFunction::EulerFunction(int number) : number(number) {}

int EulerFunction::solve() const {
    auto data = decompositionToSimple(number);
    int result = 1;
    for (size_t i = 0; i < data.size(); ++i){
        result *= pow(data[i].first, data[i].second) - pow(data[i].first, data[i].second - 1);
    }
    return result;
}
