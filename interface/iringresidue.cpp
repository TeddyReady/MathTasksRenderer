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

    default:
        break;
    }

    switch (option)
    {
    case RingResidueOptions::GenCount:
        optional = static_cast<int>(gen->bounded(2, 15));
        switch (type) {
        case ResidueType::Zn:
            operation = "+";
            module = static_cast<int>(gen->bounded(minNum, maxNum));
            break;

        case ResidueType::MultiGroup_Zn:
            operation = "*";
            do {
                module = static_cast<int>(gen->bounded(minNum, maxNum));
            } while(not isPrime(module));
            break;
        }
        break;

    case RingResidueOptions::A_in_M:
        optional = static_cast<int>(gen->bounded(2, 15));
        module = static_cast<int>(gen->bounded(minNum, maxNum));
        n = static_cast<int>(gen->bounded(2, module));
        operation = "+";
        type = ResidueType::Zn;
        break;

    case RingResidueOptions::A_X_equal_B:
    case RingResidueOptions::Count_A_X_equal_B:
        module = static_cast<int>(gen->bounded(minNum, maxNum));
        n = static_cast<int>(gen->bounded(2, module));
        optional = static_cast<int>(gen->bounded(1, module));
        break;

    case RingResidueOptions::XX_equal_p:
    case RingResidueOptions::Count_XX_equal_p:
        do {
            module = static_cast<int>(gen->bounded(minNum, maxNum));
        } while (not isPrime(module));
        n = static_cast<int>(gen->bounded(1, module));
        break;

    case RingResidueOptions::XX_equal_pq:
    case RingResidueOptions::Count_XX_equal_pq:
        do {
            module = static_cast<int>(gen->bounded(minNum, maxNum));
        } while (isPrime(module));
        n = static_cast<int>(gen->bounded(1, module));
        break;

    default:
        break;
    }
}

QString RingResidueInterface::description()
{
    switch (option)
    {
    case RingResidueOptions::GenCount:
        return QString("Вычислите количество образующих элементов группы вычетов");

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

    case RingResidueOptions::Count_A_X_equal_B:
        return QString("Найдите число решений линейного сравнения");

    case RingResidueOptions::Count_XX_equal_p:
    case RingResidueOptions::Count_XX_equal_pq:
        return QString("Найдите число решений квадратичного сравнения");

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
        return printResidue(operation) + ":" + QString::number(n) + "^{" + QString::number(optional) + "}=~?";

    case RingResidueOptions::A_X_equal_B:
    case RingResidueOptions::Count_A_X_equal_B:
        return QString("%1x\\equiv %2~mod(%3)\\Rightarrow?").arg(QString::number(n)).arg(QString::number(optional)).arg(QString::number(module));

    case RingResidueOptions::Order:
        return QString("%1:ord_{%2}(%3)=?").arg(printResidue(operation)).arg(operation).arg(n);

    case RingResidueOptions::XX_equal_p:
    case RingResidueOptions::XX_equal_pq:
    case RingResidueOptions::Count_XX_equal_p:
    case RingResidueOptions::Count_XX_equal_pq:
        return QString("x^2\\equiv %1~mod(%2)\\Rightarrow?").arg(QString::number(n)).arg(QString::number(module));

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
        return QString::number(RingResidue(n, module, type).pow(optional));

    case RingResidueOptions::Order:
        return QString::number(RingResidue(n, module, type).order(operation));

    case RingResidueOptions::Count_A_X_equal_B:
        return QString::number(RingResidue::countOfAnswersLinear(n, optional, module));

    case RingResidueOptions::A_X_equal_B:
        return printAnswers(RingResidue::solveLinear(n, optional, module));

    case RingResidueOptions::Count_XX_equal_p:
        return QString::number(RingResidue::countOfAnswersPrimeQuadro(n, module));

    case RingResidueOptions::XX_equal_p:
        return printAnswers(RingResidue::solvePrimeQuadro(n, module));

    case RingResidueOptions::Count_XX_equal_pq:
        return QString::number(RingResidue::countOfAnswersCompositeQuadro(n, module));

    case RingResidueOptions::XX_equal_pq:
        return printAnswers(RingResidue::solveCompositeQuadro(n, module));

    default:
        return QString("");
    }
}

QString RingResidueInterface::printResidue(const char *operation) const
{
    switch (type) {
        case ResidueType::Zn:
        case ResidueType::Zp:
            return QString("\\mathbb{Z}_{" + QString::number(module) + "}");

        case ResidueType::MultiGroup_Zn:
            return QString("\\mathbb{Z}_{" + QString::number(module) + "}^{*}");

        default:
            return QString("");
    }
}

QString RingResidueInterface::printAnswers(const std::vector<int> &answers) const
{
    if (answers.at(0) == -1)
        return QString("-");

    QString result;
    for(size_t i = 0; i < answers.size(); ++i)
    {
        result += QString::number(answers[i]);
        if (i + 1 != answers.size())
            result += ",";
    }
    return result;
}
