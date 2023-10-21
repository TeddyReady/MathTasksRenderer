#include "ikelitable.h"
#define MAX_TABLE_SIZE 6

KeliTableInterface::KeliTableInterface(int minNum, int maxNum, KeliTableOptions option)
    : minNum(minNum), maxNum(maxNum), option(option), gen(QRandomGenerator::global()) {}
KeliTableInterface::~KeliTableInterface() { gen = nullptr; }

void KeliTableInterface::create()
{
    std::vector<std::vector<std::string>> data;
    std::vector<std::string> header;

    int volume = static_cast<int>(gen->bounded(minNum, maxNum));
    generateVars(volume);

    _answer = gen->bounded(2);
    switch (option)
    {
    case KeliTableOptions::Isomorph:
        for (int i = 0; i < volume; ++i)
        {
            header.push_back(realVars[i]);
            std::vector<std::string> row;
            for (int j = 0; j < volume; ++j)
                row.push_back(realVars[gen->bounded(volume)]);

            data.push_back(row);
        }
        break;
    case KeliTableOptions::Abel:
        if (not _answer)
        {
            do {
                data.clear(); header.clear();
                for (int i = 0; i < volume; ++i)
                {
                    header.push_back(realVars[i]);
                    std::vector<std::string> row;
                    for (int j = 0; j < volume; ++j)
                        row.push_back(realVars[gen->bounded(volume)]);

                    data.push_back(row);
                }
            } while (KeliTable(data, header, header).isAbel());
        }
        else
        {
            for (int i = 0; i < volume; ++i)
            {
                header.push_back(realVars[i]);
                std::vector<std::string> row;
                row.resize(volume);
                data.push_back(row);
            }

            for (int i = 0; i < volume; ++i)
                for (int j = i; j < volume; ++j)
                {
                    data[i][j] = realVars[gen->bounded(volume)];

                    if (i != j)
                        data[j][i] = data[i][j];
                }
        }
        break;
    }

    table = KeliTable(data, header, header);

    randomSortTable();
}

QString KeliTableInterface::description()
{
    switch (option)
    {
    case KeliTableOptions::Isomorph:
        return QString("Являются ли данные структуры изоморфными");
    case KeliTableOptions::Abel:
        return QString("Является ли данная структура абелевой");
    }
}

QString KeliTableInterface::task()
{
    switch (option)
    {
    case KeliTableOptions::Isomorph:
        return QString("%1~?~%2")
                .arg(QString::fromStdString(std::string(table)))
                .arg(masqueTable());

    case KeliTableOptions::Abel:
        return QString("%1\\Rightarrow~?")
                .arg(QString::fromStdString(std::string(table)));
    }
}

QString KeliTableInterface::answer()
{
    switch (option)
    {
    case KeliTableOptions::Isomorph:
        return (_answer) ? "\\cong" : "\\ncong";

    case KeliTableOptions::Abel:
        return (_answer) ? "Да" : "Нет" ;
    }
}

QString KeliTableInterface::masqueTable()
{
    randomSortTable();

    QString masqTable = QString::fromStdString(std::string(table));

    if (_answer)
    {
        for (int i = 0; i < table.volume(); ++i)
        {
            masqTable = masqTable.replace(
                QString::fromStdString(realVars[i]),
                QString::fromStdString(masqVars[i])
            );
        }
    }
    else
    {
        for (int i = 0; i < table.volume(); ++i)
        {
            masqTable = masqTable.replace(
                QString::fromStdString(realVars[i]),
                QString::fromStdString(masqVars[(table.volume()-1) - i])
            );
        }
    }

    return masqTable;
}

std::string KeliTableInterface::getCodeByStaticIndex(int index, int tableNumber) const
{
    if (tableNumber == 1)
    {
        switch (index)
        {
        case 0:
            return std::string("\\alpha");
        case 1:
            return std::string("\\beta");
        case 2:
            return std::string("\\gamma");
        case 3:
            return std::string("\\delta");
        case 4:
            return std::string("\\xi");
        case 5:
            return std::string("\\zeta");
        default:
            return std::string("");
        }
    }
    else
    {
        switch (index)
        {
        case 0:
            return std::string("x");
        case 1:
            return std::string("y");
        case 2:
            return std::string("z");
        case 3:
            return std::string("w");
        case 4:
            return std::string("v");
        case 5:
            return std::string("n");
        default:
            return std::string("");
        }
    }
}

void KeliTableInterface::generateVars(int volume)
{
    QVector<int> index;
    for (int i = 0; i < MAX_TABLE_SIZE; ++i)
        index.push_back(i);

    for (int i = 0; i < volume; ++i)
    {
        int pos = gen->bounded(index.size());
        realVars.push_back(getCodeByStaticIndex(index[pos], 1));
        index.removeAt(pos);
    }
    index.clear();

    for (int i = 0; i < MAX_TABLE_SIZE; ++i)
        index.push_back(i);

    for (int i = 0; i < volume; ++i)
    {
        int pos = gen->bounded(index.size());
        masqVars.push_back(getCodeByStaticIndex(index[pos], 2));
        index.removeAt(pos);
    }
}

void KeliTableInterface::randomSortTable()
{
    int swap_count = gen->bounded(2, table.volume());
    for (int i = 0; i < swap_count; ++i)
    {
        if (static_cast<bool>(gen->bounded(2)))
            table.swapRows(gen->bounded(table.volume()), gen->bounded(table.volume()));

        else
            table.swapCols(gen->bounded(table.volume()), gen->bounded(table.volume()));
    }
}
