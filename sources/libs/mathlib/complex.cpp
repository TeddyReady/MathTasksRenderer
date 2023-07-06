#include "complex.h"

Complex::Complex() : a(0), b(0) {}
Complex::Complex(int a, int b) : a(a), b(b) {}
Complex::Complex(const Complex &other) : a(other.a), b(other.b) {}

Complex Complex::operator-() const
{
    return Complex(-a, -b);
}

Complex Complex::operator+(const Complex& other) const
{
    return Complex(a + other.a, b + other.b);
}

Complex Complex::operator+=(const Complex& other)
{
    a += other.a;
    b += other.b;
    return *this;
}

Complex Complex::operator-(const Complex& other) const
{
    return Complex(a - other.a, b - other.b);
}

Complex Complex::operator-=(const Complex& other)
{
    a -= other.a;
    b -= other.b;
    return *this;
}

Complex Complex::operator*(const Complex& other) const
{
    return Complex(a * other.a - b * other.b, a * other.b + other.a * b);
}

Complex Complex::operator*=(const Complex& other)
{
    a = a * other.a - b * other.b;
    b = a * other.b + other.a * b;
    return *this;
}
