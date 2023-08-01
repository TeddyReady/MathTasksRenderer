#include "iringresidue.h"

RingResidueInterface::RingResidueInterface(int minNum, int maxNum, RingResidueOptions option, ResidueType type)
    : minNum(minNum), maxNum(maxNum), option(option), gen(QRandomGenerator::global()), type(type) {}
RingResidueInterface::~RingResidueInterface() { gen = nullptr; }

void RingResidueInterface::create()
{
    if (static_cast<bool>(gen->bounded(0, 2)))
        operation = "+";
    else
        operation = "*";

    switch (type) {
    case ResidueType::Zn:
        if (not strcmp(operation, "*"))
        {
            do {
                module = static_cast<int>(gen->bounded(minNum, maxNum));
            } while (not isPrime(module));
        }
        else
            module = static_cast<int>(gen->bounded(minNum, maxNum));

        n = static_cast<int>(gen->bounded(2, module));
        break;

    case ResidueType::Zp:
        do {
            module = static_cast<int>(gen->bounded(minNum, maxNum));
        } while (not isPrime(module));

        n = static_cast<int>(gen->bounded(2, module));
        break;

    case ResidueType::MultiGroup_Zn:
        module = static_cast<int>(gen->bounded(minNum, maxNum));
        do {
            n = static_cast<int>(gen->bounded(2, module));
        } while(GCD(n, module) != 1);

        operation = "*";
        break;
    }

    switch (option)
    {
    case RingResidueOptions::A_in_M:
        degree = static_cast<int>(gen->bounded(2, 15));
        break;

//    case RingResidueOptions::A_X_equal_B:
//        return;

//    case RingResidueOptions::XX_equal_p:
//        return;

//    case RingResidueOptions::XX_equal_pq:
//        return;


    default:
        break;
    }
}

QString RingResidueInterface::description()
{
    switch (option)
    {
    case RingResidueOptions::GenCount:
        return QString("Вычислите количество образующих элементов группы Вычетов");

    case RingResidueOptions::A_in_M:
        return QString("Найдите значение выражения");

    case RingResidueOptions::A_X_equal_B:
        return QString("Решите линейное сравнение");

    case RingResidueOptions::Order:
        return QString("Вычислите порядок элемента");

    case RingResidueOptions::XX_equal_p:
        return QString("Решите квадратичное сравнение по простому модулю");

    case RingResidueOptions::XX_equal_pq:
        return QString("Решите квадратичное сравнение по составному модулю");


    default:
        return QString("");
    }
}

QString RingResidueInterface::task()
{
    switch (option)
    {
    case RingResidueOptions::GenCount:
        return printResidue(operation) + "=~?";

    case RingResidueOptions::A_in_M:
        return printResidue(operation) + ":" + QString::number(n) + "^{" + QString::number(degree) + "}=~?";

//    case RingResidueOptions::A_X_equal_B:
//            return QString("Решите линейное сравнение");

    case RingResidueOptions::Order:
        return printResidue(operation) + ":" + QString::number(n) + "=~?";

//    case RingResidueOptions::XX_equal_p:
//            return QString("Решите квадратичное сравнение по простому модулю");

//    case RingResidueOptions::XX_equal_pq:
//            return QString("Решите квадратичное сравнение по составному модулю");


    default:
        return QString("");
    }
}

QString RingResidueInterface::answer()
{
    switch (option)
    {
    case RingResidueOptions::GenCount:
        return QString::number(RingResidue(n, module, type).countOfGenerators(operation));

    case RingResidueOptions::A_in_M:
        return QString::number(RingResidue(n, module, type).pow(degree, operation));

//    case RingResidueOptions::A_X_equal_B:
//           return QString::number(RingResidue(n, module, type).pow(degree, operation));

    case RingResidueOptions::Order:
        return QString::number(RingResidue(n, module, type).order(operation));

//    case RingResidueOptions::XX_equal_p:
//            return QString("Решите квадратичное сравнение по простому модулю");

//    case RingResidueOptions::XX_equal_pq:
//            return QString("Решите eквадратичное сравнение по составному модулю");

    default:
        return QString("");
    }
}

QString RingResidueInterface::printResidue(const char *operation) const
{
    switch (type) {
    case ResidueType::Zn:
    case ResidueType::Zp:
        if (not strcmp(operation, "+"))
            return QString("\\left(\\mathbb{Z}_{" + QString::number(module) + "}, +\\right)");
        else
            return QString("\\left(\\mathbb{Z}_{" + QString::number(module) + "}, \\cdot\\right)");

    case ResidueType::MultiGroup_Zn:
        return QString("\\left(\\mathbb{Z}_{" + QString::number(module) + "}^{*}, \\cdot\\right)");

     default:
        return QString("");
    }
}
