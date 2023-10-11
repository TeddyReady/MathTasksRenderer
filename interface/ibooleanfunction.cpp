#include "ibooleanfunction.h"
#define FUNC_SYSTEM_COUNT 3

BooleanFunctionInterface::BooleanFunctionInterface(int minNum, int maxNum, BooleanFunctionOptions option)
    : minNum(minNum), maxNum(maxNum), option(option), gen(QRandomGenerator::global()) {}
BooleanFunctionInterface::~BooleanFunctionInterface() { gen = nullptr; }

void BooleanFunctionInterface::create()
{
    switch (option)
    {
    case BooleanFunctionOptions::FullSystem:
        {
            system.clear();
            for (int i = 0; i < FUNC_SYSTEM_COUNT; ++i)
            {
                std::vector<bool> data;
                int dimension = pow(2, gen->bounded(minNum, maxNum));

                for (int i = 0; i < dimension; ++i)
                    data.push_back(static_cast<bool>(gen->bounded(0, 2)));

                system.push_back(BooleanFunction(data));
            }
            break;
        }
    default:
        {
            std::vector<bool> data;
            int dimension = pow(2, gen->bounded(minNum, maxNum));

            for (int i = 0; i < dimension; ++i)
                data.push_back(static_cast<bool>(gen->bounded(0, 2)));

            function = BooleanFunction(data);
            break;
        }
    }
}

QString BooleanFunctionInterface::description()
{
    switch (option)
    {
    case BooleanFunctionOptions::PolinomJegalkina:
        return QString("Представьте булевую функцию в виде многочлена Жегалкина");
    case BooleanFunctionOptions::FunctionFromJegalkin:
        return QString("Выразите булевую функцию, используя многочлен Жегалкина");
    case BooleanFunctionOptions::S_functions:
        return QString("Принадлежит ли функция классу самодвойственных");
    case BooleanFunctionOptions::L_functions:
        return QString("Принадлежит ли функция классу линейных");
    case BooleanFunctionOptions::M_functions:
        return QString("Принадлежит ли функция классу монотонных");
    case BooleanFunctionOptions::T1_functions:
        return QString("Принадлежит ли функция классу, сохраняющих константу 1");
    case BooleanFunctionOptions::T0_functions:
        return QString("Принадлежит ли функция классу, сохраняющих константу 0");
    case BooleanFunctionOptions::FullSystem:
        return QString("Является ли система булевых функций - полной");
    }
}

QString BooleanFunctionInterface::task()
{
    switch (option)
    {
    case BooleanFunctionOptions::PolinomJegalkina:
        return QString("%1 = ?").arg(QString::fromStdString(std::string(function)));
    case BooleanFunctionOptions::FunctionFromJegalkin:
        return QString("%1 = ?").arg(QString::fromStdString(std::string(function.printPolinomial())));
    case BooleanFunctionOptions::FullSystem:
        return QString("\\begin{cases} %1 \\\\ %2 \\\\ %3 \\end{cases} \\Rightarrow ?")
                    .arg(QString::fromStdString(std::string(system.at(0))))
                    .arg(QString::fromStdString(std::string(system.at(1))))
                    .arg(QString::fromStdString(std::string(system.at(2))));
    default:
        return QString("%1 \\Rightarrow ?").arg(QString::fromStdString(std::string(function)));
    }
}

QString BooleanFunctionInterface::answer()
{
    switch (option)
    {
    case BooleanFunctionOptions::PolinomJegalkina:
        return QString::fromStdString(std::string(function.printPolinomial()));
    case BooleanFunctionOptions::FunctionFromJegalkin:
        return QString::fromStdString(std::string(function));
    case BooleanFunctionOptions::S_functions:
        return function.isBelongsToClass_S() ? "Да" : "Нет";
    case BooleanFunctionOptions::L_functions:
        return function.isBelongsToClass_L() ? "Да" : "Нет";
    case BooleanFunctionOptions::M_functions:
        return function.isBelongsToClass_M() ? "Да" : "Нет";
    case BooleanFunctionOptions::T1_functions:
        return function.isBelongsToClass_T1() ? "Да" : "Нет";
    case BooleanFunctionOptions::T0_functions:
        return function.isBelongsToClass_T0() ? "Да" : "Нет";
    case BooleanFunctionOptions::FullSystem:
        return BooleanFunction::isSystemFull(system) ? "Да" : "Нет";
    }
}
