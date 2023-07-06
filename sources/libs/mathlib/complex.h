#ifndef COMPLEX_H
#define COMPLEX_H
#include "basemath.h"

class Complex {
public:
    Complex();
    Complex(int a, int b);
    Complex(const Complex &other);

    Complex operator-()                     const;
    Complex operator+(const Complex& other) const;
    Complex operator+=(const Complex& other);
    Complex operator-(const Complex& other) const;
    Complex operator-=(const Complex& other);
    Complex operator*(const Complex& other) const;
    Complex operator*=(const Complex& other);

private:
    int a, b;
};

#endif // COMPLEX_H
