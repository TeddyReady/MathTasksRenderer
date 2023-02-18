#ifndef BASEMATH_H
#define BASEMATH_H
#include <QRandomGenerator>
#include <QString>
#include <algorithm>
#include <vector>
#include <cmath>

//Разложение целого числа на простые множители
std::vector<std::pair<int, int>> decompositionToSimple(int num);

//Проверка на простоту
bool isPrime(int num);

//НОД
int GCD(int a, int b);

//НОК
int LCM(int a, int b);

//Быстрое (бинарное) возведение в степень
long fastPower(long num, long deg);

#endif // BASEMATH_H
