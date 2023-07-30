#include "imatrix.tpp"

template <>
QString MatrixInterface<int>::getType()
{
    return QString("\\mathbb{Z}:");
}

template <>
QString MatrixInterface<double>::getType()
{
    return QString("\\mathbb{R}:");
}

template <>
QString MatrixInterface<Zn>::getType()
{
    return "\\mathbb{Z}_{" + QString::number(module) + "}:";
}

template <>
QString MatrixInterface<Zp>::getType()
{
    return "\\mathbb{Z}_{" + QString::number(module) + "}:";
}

template <>
QString MatrixInterface<Complex<int>>::getType()
{
    return QString("\\mathbb{Z}[i]:");
}

template <>
QString MatrixInterface<Complex<double>>::getType()
{
    return QString("\\mathbb{C}:");
}
