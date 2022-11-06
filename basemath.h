#ifndef BASEMATH_H
#define BASEMATH_H
#include <iostream>
#include <cmath>
#include <QDebug>
#include <QVector>

class BaseMath
{
public:
    BaseMath();
};

//Разложение на простые
QVector<std::pair<int, int>> decompositionToSimple(int num);
//Проверка на простоту
bool isPrime(int num);

#endif // BASEMATH_H
