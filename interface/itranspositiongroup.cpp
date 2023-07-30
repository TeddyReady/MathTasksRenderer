#include "itranspositiongroup.h"

TranspositionGroupInterface::TranspositionGroupInterface(int minN, int maxN, TranspositionGroupOptions option, ViewMode mode)
    : minNum(minN), maxNum(maxN), option(option), gen(QRandomGenerator::global()), viewMode(mode) {}
TranspositionGroupInterface::~TranspositionGroupInterface() { gen = nullptr; }

void TranspositionGroupInterface::create()
{
    switch (option)
    {
    case TranspositionGroupOptions::Multiply:
        createTransposition(tp_1);
        createTransposition(tp_2, tp_1.getTask());
        break;

    default:
        createTransposition(tp_1);
        break;
    }
}

QString TranspositionGroupInterface::description()
{
    switch (option)
    {
    case TranspositionGroupOptions::Write:

        if (viewMode == ViewMode::Standart)
            return QString("Запишите подстановку в виде произведения независимых циклов");
        else
            return QString("Запишите подстановку в табличном виде");


    case TranspositionGroupOptions::Multiply:

        if (viewMode == ViewMode::Standart)
            return QString("Найдите произведение подстановок, записанных в табличном виде");
        else
            return QString("Найдите произведение подстановок, записанных в циклическом виде");

    case TranspositionGroupOptions::Inverse:

        if (viewMode == ViewMode::Standart)
            return QString("Найдите подстановку, обратную данной, записанной в табличном виде");
        else
            return QString("Найдите подстановку, обратную данной, записанной в циклическом виде");

    case TranspositionGroupOptions::Cycle:

        if (viewMode == ViewMode::Standart)
            return QString("Найдите цикловой тип подстановки, записанной в табличном виде");
        else
            return QString("Найдите цикловой тип подстановки, записанной в циклическом виде");

    case TranspositionGroupOptions::Mod2:

        if (viewMode == ViewMode::Standart)
            return QString("Определите четность подстановки, записанной в табличном виде");
        else
            return QString("Определите четность подстановки, записанной в циклическом виде");

    case TranspositionGroupOptions::Count:

        if (viewMode == ViewMode::Standart)
            return QString("Вычислите количество беспорядков подстановки, записанной в табличном виде");
        else
            return QString("Вычислите количество беспорядков подстановки, записанной в циклическом виде");

    case TranspositionGroupOptions::Order:

        if (viewMode == ViewMode::Standart)
            return QString("Определите порядок подстановки, записанной в табличном виде");
        else
            return QString("Определите порядок подстановки, записанной в циклическом виде");

    case TranspositionGroupOptions::Decomposition:
            return QString("Запишите подстановку в виде произведения транспозиций");

    case TranspositionGroupOptions::Neighbor:
            return QString("Запишите подстановку в виде произведения транспозиций соседних элементов");

    default:
        return QString("");
    }
}

QString TranspositionGroupInterface::task()
{
    QString taskText("S_{" + QString::number(tp_1.getTask()) + "}:");

    switch (option)
    {
    case TranspositionGroupOptions::Write:
    case TranspositionGroupOptions::Cycle:
    case TranspositionGroupOptions::Decomposition:
    case TranspositionGroupOptions::Neighbor:
        taskText += QString::fromStdString(tp_1.writeToMode(viewMode));
        break;

    case TranspositionGroupOptions::Multiply:
        taskText += QString::fromStdString(tp_1.writeToMode(viewMode)) + "\\cdot" +
                    QString::fromStdString(tp_2.writeToMode(viewMode));
        break;

    case TranspositionGroupOptions::Inverse:
        taskText += QString("{") + QString::fromStdString(tp_1.writeToMode(viewMode)) + QString("}^{-1}");
        break;

    case TranspositionGroupOptions::Mod2:
        taskText += QString("\\delta") + QString::fromStdString(tp_1.writeToMode(viewMode));
        break;

    case TranspositionGroupOptions::Count:
        taskText += QString("\\Delta") + QString::fromStdString(tp_1.writeToMode(viewMode));
        break;

    case TranspositionGroupOptions::Order:
        taskText += QString("~ord") + QString::fromStdString(tp_1.writeToMode(viewMode));
        break;

    default:
        taskText += QString("");
        break;
    }
    return taskText + QString("=~?");
}

QString TranspositionGroupInterface::answer()
{
    switch (option)
    {
    case TranspositionGroupOptions::Write:
        return QString::fromStdString(tp_1.writeToMode(static_cast<ViewMode>((static_cast<int>(viewMode) + 1) % 2)));

    case TranspositionGroupOptions::Multiply:
        return QString::fromStdString((tp_1 * tp_2).writeToMode(viewMode));

    case TranspositionGroupOptions::Inverse:
        return QString::fromStdString((~tp_1).writeToMode(viewMode));

    case TranspositionGroupOptions::Cycle:
        return QString::fromStdString(tp_1.cycleType());

    case TranspositionGroupOptions::Mod2:
        return QString::fromStdString(tp_1.getEven());

    case TranspositionGroupOptions::Count:
        return QString::number(tp_1.getHaos());

    case TranspositionGroupOptions::Order:
        return QString::number(tp_1.getOrder());

    case TranspositionGroupOptions::Decomposition:
        return QString::fromStdString(tp_1.writeToMode(ViewMode::Decomposition));

    case TranspositionGroupOptions::Neighbor:
        return QString::fromStdString(tp_1.writeToMode(ViewMode::Neighbors));

    default:
        return QString("");
    }
}

void TranspositionGroupInterface::createTransposition(TranspositionGroup &tp, int order)
{
    if (order == 0)
        order = static_cast<int>(gen->bounded(minNum, maxNum));

    int curValue;
    QVector<int> sizes, id, tmp;

    /* Generating random weights */
    do {
        int cnt = 0; sizes.clear();

        while (cnt != order)
        {
            curValue = static_cast<int>(gen->bounded(1, (order + 1) - cnt));
            sizes.push_back(curValue);
            cnt += curValue;
        }

    } while (*std::max_element(sizes.begin(), sizes.end()) == 1);

    /* Identity transposition */
    for (int i = 1; i <= order; ++i)
        id.push_back(i);

    QString transposition("");
    tp.clear();
    for (int i = 0; i < sizes.size(); ++i)
    {
        /* Create a cycle */
        for (int j = 0; j < sizes[i]; ++j)
        {
            curValue = static_cast<int>(gen->bounded(0, id.size()));
            tmp.push_back(id[curValue]);
            id.removeAt(curValue);
        }

        /* QString transposition */
        transposition += QString("(");
        for (int k = 0; k < tmp.size(); ++k)
        {
            transposition += QString::number(tmp.at(k));
            if (k + 1 != tmp.size())
                transposition += QString(",");
        }
        transposition += QString(")");

        tmp.clear();
    }

    tp = TranspositionGroup(transposition.toStdString(), order);
}
