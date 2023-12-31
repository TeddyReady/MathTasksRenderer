#ifndef ComplexInterface_H
#define ComplexInterface_H
#include "interface.h"
#include "ringofmembers.tpp"
#include "complex.tpp"
#include "basemath.h"

enum class ComplexOptions {
    Summary ,
    Diff    ,
    Multiply,
    Divide  ,
    Ostat   ,
    GCD     ,
};

template <class T>
class ComplexInterface : public TaskInterface {
private:
    int minNum, maxNum;
    ComplexOptions option;
    QRandomGenerator *gen;
    Complex<T> num_1, num_2, extra_num;

public:
    explicit ComplexInterface(int minNum, int maxNum, ComplexOptions option)
            : minNum(minNum), maxNum(maxNum), option(option), gen(QRandomGenerator::global()) {}
    ~ComplexInterface() { gen = nullptr; }

    void create()
    {
        switch (option) {
        case ComplexOptions::Ostat:
        case ComplexOptions::GCD:
            num_1 = generateValue();
            num_2 = generateValue();
            do {extra_num = generateValue();} while(extra_num.norm() > num_1.norm());
            break;
        default:
            num_1 = generateValue();
            num_2 = generateValue();
            break;
        }
    }

    QString description()
    {
        switch (option)
        {
        case ComplexOptions::Summary:
        case ComplexOptions::Diff:
        case ComplexOptions::Multiply:
            return QString("Найдите значение выражения");
        case ComplexOptions::Divide:
            return QString("Вычислите целую часть от деления");
        case ComplexOptions::Ostat:
            return QString("Вычислите остаток от деления чисел");
        case ComplexOptions::GCD:
            return QString("Найдите НОД чисел");
        default:
            break;
        }

        return QString("");
    }

    QString task()
    {
        switch (option)
        {
        case ComplexOptions::Summary:
            return QString("%1(%2)+(%3)=~?").arg(getType()).arg(printNum(num_1)).arg(printNum(num_2));
        case ComplexOptions::Diff:
            return QString("%1(%2)-(%3)=~?").arg(getType()).arg(printNum(num_1)).arg(printNum(num_2));
        case ComplexOptions::Multiply:
            return QString("%1(%2)\\cdot(%3)=~?").arg(getType()).arg(printNum(num_1)).arg(printNum(num_2));
        case ComplexOptions::Divide:
            return QString("%1(%2)/(%3)=~?").arg(getType()).arg(printNum(num_1)).arg(printNum(num_2));
        case ComplexOptions::Ostat:
            return QString("%1(%2)~mod~(%3)=~?").arg(getType()).arg(printNum(num_1*num_2 + extra_num)).arg(printNum(num_1));
        case ComplexOptions::GCD:
            return QString("%1НОД(%2,%3)=~?").arg(getType()).arg(printNum(num_1*extra_num)).arg(printNum(num_2*extra_num));
        default:
            break;
        }

        return QString("");
    }

    QString answer()
    {
        switch (option)
        {
        case ComplexOptions::Summary:
            return printNum(num_1 + num_2);
        case ComplexOptions::Diff:
            return printNum(num_1 - num_2);
        case ComplexOptions::Multiply:
            return printNum(num_1 * num_2);
        case ComplexOptions::Divide:
            return printNum(num_1 / num_2);
        case ComplexOptions::Ostat:
            return printNum(extra_num);
        case ComplexOptions::GCD:
            return printNum(extra_num);
        default:
            return QString("");
        }
    }

    QString printNum(const Complex<T> &other) const
    {
        return "["+ QString::fromStdString(std::string(other)) + "]";
    }

    QString getType();
    Complex<T> generateValue();
};

#endif // ComplexInterface_H
