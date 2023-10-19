#include "icomplex.tpp"

template <>
Complex<int> ComplexInterface<int>::generateValue()
{
    return Complex<int>(static_cast<int>(gen->bounded(minNum, maxNum)), static_cast<int>(gen->bounded(minNum, maxNum)));
}

template <>
Complex<double> ComplexInterface<double>::generateValue()
{
    return Complex<double>(gen->bounded(minNum, maxNum), gen->bounded(minNum, maxNum));
}

template <>
QString ComplexInterface<int>::getType()
{
    return QString("\\mathbb{Z}[i]:");
}

template <>
QString ComplexInterface<double>::getType()
{
    return QString("\\mathbb{C}:");
}
