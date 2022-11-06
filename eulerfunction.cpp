#include "eulerfunction.h"
EulerFunction::EulerFunction() : taskName("Функция Эйлера") {}
EulerFunction::EulerFunction(int number) : taskName("Функция Эйлера"), number(number) {}

int EulerFunction::solve(){
    auto data = decompositionToSimple(number);
    int result = 1;
    for (int i = 0; i < data.size(); i++){
        result *= pow(data[i].first, data[i].second) - pow(data[i].first, data[i].second - 1);
    }
    return result;
}
