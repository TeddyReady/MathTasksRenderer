#include "basemath.h"

std::vector<std::pair<int, int>> decompositionToSimple(int num)
{
    std::vector<std::pair<int, int>> deviders;
    if (isPrime(num))
         deviders.emplace_back(std::make_pair(num, 1));
    else {
        int count = 0;
        int localNum = num;
        for (size_t i = 2; i <= static_cast<size_t>(num / 2 + 1);) {
            if (localNum % i == 0){
                localNum /= i;
                count++;
            } else if (count != 0){
                deviders.emplace_back(std::make_pair(i, count));
                count = 0;
            } else i++;
        }
    }
    return deviders;
}

bool isPrime(int num)
{
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

int GCD(int a, int b)
{
    return b ? GCD(b, a % b) : a;
}

int LCM(int a, int b)
{
    return a / GCD(a, b) * b;
}

long fastPower(long num, long deg) {
    long result = 1;
    while(deg) {
        if (deg % 2 == 0) {
            deg /= 2;
            num *= num;
        } else {
            deg--;
            result *= num;
        }
    } return result;
}

long modulePower(long num, long deg, int module) {
  if (deg == 0) return 1;
  int subNum = modulePower(num, deg / 2, module);
  if (deg % 2 == 0)
    return (subNum * subNum) % module;
  else
    return (num * subNum * subNum) % module;
}

int max(int a, int b)
{
    return a > b ? a : b;
}
