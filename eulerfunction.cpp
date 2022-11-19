#include "eulerfunction.h"
EulerFunction::EulerFunction() {}
EulerFunction::EulerFunction(int number) : number(number) {}

int EulerFunction::solve(){
    auto data = decompositionToSimple(number);
    int result = 1;
    for (int i = 0; i < data.size(); i++){
        result *= pow(data[i].first, data[i].second) - pow(data[i].first, data[i].second - 1);
    }
    return result;
}

void EulerFunction::setTask(int num){
    number = num;
}
int EulerFunction::getTask(){
    return number;
}
