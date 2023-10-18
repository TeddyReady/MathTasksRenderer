#include "iringofmembers.tpp"

template <>
int RingOfMembersInterface<int>::generateValue()
{
    return static_cast<int>(gen->bounded(-4, 4));
}

template <>
double RingOfMembersInterface<double>::generateValue()
{
    return gen->bounded(-4, 4);
}

template <>
Zn RingOfMembersInterface<Zn>::generateValue()
{
    if (module == 0)
        module = static_cast<int>(gen->bounded(2, 60));

    return Zn(static_cast<int>(gen->bounded(0, module)), module);
}

template <>
Zp RingOfMembersInterface<Zp>::generateValue()
{
    if (module == 0)
    do {
        module = static_cast<int>(gen->bounded(2, 60));
    } while (not isPrime(module));

    return Zp(static_cast<int>(gen->bounded(0, module)), module);
}

template <>
Complex<int> RingOfMembersInterface<Complex<int>>::generateValue()
{
    return Complex<int>(static_cast<int>(gen->bounded(-4, 4)), static_cast<int>(gen->bounded(-4, 4)));
}

template <>
Complex<double> RingOfMembersInterface<Complex<double>>::generateValue()
{
    return Complex<double>(gen->bounded(-4, 4), gen->bounded(-4, 4));
}

template <>
QString RingOfMembersInterface<int>::getType()
{
    return QString("\\mathbb{Z}:");
}

template <>
QString RingOfMembersInterface<double>::getType()
{
    return QString("\\mathbb{R}:");
}

template <>
QString RingOfMembersInterface<Zn>::getType()
{
    return "\\mathbb{Z}_{" + QString::number(module) + "}:";
}

template <>
QString RingOfMembersInterface<Zp>::getType()
{
    return "\\mathbb{Z}_{" + QString::number(module) + "}:";
}

template <>
QString RingOfMembersInterface<Complex<int>>::getType()
{
    return QString("\\mathbb{Z}[i]:");
}

template <>
QString RingOfMembersInterface<Complex<double>>::getType()
{
    return QString("\\mathbb{C}:");
}
