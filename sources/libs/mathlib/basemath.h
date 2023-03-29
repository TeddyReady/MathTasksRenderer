#ifndef BASEMATH_H
#define BASEMATH_H
#include "common.h"

//Number decomposition to simple
std::vector<std::pair<int, int>> decompositionToSimple(int num);

//Prime check
bool isPrime(int num);

//Great Common Divisor
int GCD(int a, int b);

//Little Common Multiply
int LCM(int a, int b);

//Faster binary power
long fastPower(long num, long deg);

//Faster binary module power
long modulePower(long num, long deg, int module);

#endif // BASEMATH_H
