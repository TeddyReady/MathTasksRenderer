#include "icomplex.tpp"

template <>
Complex<int> ComplexInterface<Complex<int>>::generateValue()
{
    return Complex<int>(static_cast<int>(gen->bounded(minNum, maxNum)), static_cast<int>(gen->bounded(minNum, maxNum)));
}

template <>
Complex<double> ComplexInterface<Complex<double>>::generateValue()
{
    return Complex<double>(gen->bounded(minNum, maxNum), gen->bounded(minNum, maxNum));
}

template <>
QString ComplexInterface<Complex<int>>::getType()
{
    return QString("\\mathbb{Z}[i]:");
}

template <>
QString ComplexInterface<Complex<double>>::getType()
{
    return QString("\\mathbb{C}:");
}
