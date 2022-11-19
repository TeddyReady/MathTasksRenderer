#include "mebiusfunction.h"

MebiusFunction::MebiusFunction() {}
MebiusFunction::MebiusFunction(int number) : number(number) {}

int MebiusFunction::solve(){
    if (number == 1) return 1;
    else {
        auto data = decompositionToSimple(number);
        for (int i = 0; i < data.size(); i++) {
            if (data[i].second > 1)
                return 0;
        }
        return pow(-1, data.size());
    }
}

void MebiusFunction::setTask(int num){
    number = num;
}
int MebiusFunction::getTask(){
    return number;
}
