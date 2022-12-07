#include "basemath.h"

BaseMath::BaseMath() {}

QVector<std::pair<int, int>> decompositionToSimple(int num){
    QVector<std::pair<int, int>> deviders;
    if (isPrime(num)){
         deviders.push_back(std::make_pair(num, 1));
    } else {
        int count = 0;
        int localNum = num;
        for (int i = 2; i <= num / 2 + 1;){
            if (localNum % i == 0){
                localNum /= i;
                count++;
            } else if (count != 0){
                deviders.push_back(std::make_pair(i, count));
                count = 0;
            } else {
                i++;
            }
        }
    }
    return deviders;
}

bool isPrime(int num) {
    if (num % 2 == 0 || num == 1) {
        return false;
    }
    for (int i = 3; i <= sqrt(num); i+= 2){
          if (num % i == 0) {
              return false;
          }
    }
    return true;
}

int GCD(int a, int b){
    return b ? GCD(b, a % b) : a;
}

int LCM(int a, int b){
    return a / GCD(a, b) * b;
}
