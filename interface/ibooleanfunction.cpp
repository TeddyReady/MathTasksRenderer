#include "ibooleanfunction.h"

BooleanFunctionInterface::BooleanFunctionInterface(int minNum, int maxNum, BooleanFunctionOptions option)
    : minNum(minNum), maxNum(maxNum), option(option), gen(QRandomGenerator::global()) {}
BooleanFunctionInterface::~BooleanFunctionInterface() { gen = nullptr; }

void BooleanFunctionInterface::create()
{
    std::vector<bool> data;
    int dimension = pow(2, gen->bounded(minNum, maxNum));

    for (int i = 0; i < dimension; ++i)
        data.push_back(static_cast<bool>(gen->bounded(0, 2)));

    function = BooleanFunction(data);
}

QString BooleanFunctionInterface::description()
{
    switch (option)
    {
    case BooleanFunctionOptions::PolinomJegalkina:
        return QString("Представьте булевую функцию в виде многочлена Жегалкина");
    }
}

QString BooleanFunctionInterface::task()
{
    switch (option)
    {
    case BooleanFunctionOptions::PolinomJegalkina:
        return QString("%1 = ?").arg(QString::fromStdString(std::string(function)));
    }
}

QString BooleanFunctionInterface::answer()
{
    switch (option)
    {
    case BooleanFunctionOptions::PolinomJegalkina:
        return QString::fromStdString(std::string(function.printPolinomial()));
    }
}
