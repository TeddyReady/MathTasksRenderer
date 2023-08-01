#include "imatrix.tpp"

template <>
QString MatrixInterface<int>::stringFromT(const int &param) const
{
    return QString::number(param);
}

template <>
QString MatrixInterface<double>::stringFromT(const double &param) const
{
    return QString::number(param);
}

template <>
QString MatrixInterface<Zn>::stringFromT(const Zn &param) const
{
    return QString::fromStdString(std::string(param));
}

template <>
QString MatrixInterface<Zp>::stringFromT(const Zp &param) const
{
    return QString::fromStdString(std::string(param));
}

template <>
QString MatrixInterface<Complex<int>>::stringFromT(const Complex<int> &param) const
{
    return QString::fromStdString(std::string(param));
}

template <>
QString MatrixInterface<Complex<double>>::stringFromT(const Complex<double> &param) const
{
    return QString::fromStdString(std::string(param));
}

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
