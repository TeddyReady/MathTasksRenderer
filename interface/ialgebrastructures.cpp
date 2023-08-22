#include "ialgebrastructures.h"

AlgebraStructuresInterface::AlgebraStructuresInterface(AlgebraStructuresOptions option)
    : option(option), gen(QRandomGenerator::global()) { parseTasks(RSC::data::algebra_structures); }
AlgebraStructuresInterface::~AlgebraStructuresInterface() { gen = nullptr; }

void AlgebraStructuresInterface::create()
{
    switch (option)
    {
    case AlgebraStructuresOptions::CheckStructure:
    case AlgebraStructuresOptions::CheckOperation:
        taskLocation = static_cast<int>(gen->bounded(0, structures.size()));
        break;

    case AlgebraStructuresOptions::Associativity:
        taskLocation = static_cast<int>(gen->bounded(0, associativity.size()));
        break;

    case AlgebraStructuresOptions::Commutativity:
        taskLocation = static_cast<int>(gen->bounded(0, commutativity.size()));
        break;
    default:
        break;
    }
}

QString AlgebraStructuresInterface::description()
{
    switch (option)
    {
    case AlgebraStructuresOptions::CheckStructure:
        return QString("Опишите, чем является данная алгебраическая структура");

    case AlgebraStructuresOptions::CheckOperation:
        return QString("Является ли данная алгебраическая структура группоидом");

    case AlgebraStructuresOptions::Associativity:
        return QString("Является ли данная операция ассоциативной на заданном множестве");

    case AlgebraStructuresOptions::Commutativity:
        return QString("Является ли данная операция коммутативной на заданном множестве");

    default:
        return QString("");
    }
}

QString AlgebraStructuresInterface::task()
{
    switch (option)
    {
    case AlgebraStructuresOptions::CheckStructure:
    case AlgebraStructuresOptions::CheckOperation:
        return QString("%1\\Rightarrow?").arg(structures.at(taskLocation).first);

    case AlgebraStructuresOptions::Associativity:
        return QString("%1\\Rightarrow?").arg(associativity.at(taskLocation).first);

    case AlgebraStructuresOptions::Commutativity:
        return QString("%1\\Rightarrow?").arg(commutativity.at(taskLocation).first);

    default:
        return QString("");
    }
}

QString AlgebraStructuresInterface::answer()
{
    switch (option)
    {
    case AlgebraStructuresOptions::CheckStructure:
        if (structures.at(taskLocation).second == "None")
            return QString("Операцию невозможно задать на данном множестве").replace(" ", "~");
        else
            return structures.at(taskLocation).second;

    case AlgebraStructuresOptions::CheckOperation:
        if (structures.at(taskLocation).second == "None")
            return QString("Нет");
        else
            return QString("Да");

    case AlgebraStructuresOptions::Associativity:
        return associativity.at(taskLocation).second;

    case AlgebraStructuresOptions::Commutativity:
        return commutativity.at(taskLocation).second;

    default:
        return QString("");
    }
}

void AlgebraStructuresInterface::parseTasks(const QString &xml_file)
{
    QDomDocument document;
    QFile xmlFile(xml_file);
    if (!xmlFile.open(QIODevice::ReadOnly ))
    {
       qDebug() << "Cannot open xml file:" << xml_file;
       return;
    }
    document.setContent(&xmlFile);
    xmlFile.close();

    QDomNodeList dataSet = document.elementsByTagName(QString("set"));

    for (int setCounter = 0; setCounter < dataSet.count(); ++setCounter)
    {
        if (dataSet.item(setCounter).isElement())
        {
            QDomElement description = dataSet.item(setCounter).firstChildElement();

            QDomNodeList operations = dataSet.item(setCounter).toElement().elementsByTagName(QString("operation"));

            for (int operCounter = 0; operCounter < operations.count(); ++operCounter)
            {
                QDomElement curOperation = operations.at(operCounter).toElement();
                if (curOperation.isElement())
                {
                    QString key = QString("(%1,%2)").arg(dataSet.item(setCounter).toElement().attribute("name")).arg(curOperation.attribute("type"));
                    QDomNodeList options = curOperation.childNodes();

                    for (int optionsCounter = 0; optionsCounter < options.size(); ++optionsCounter)
                    {
                        QDomElement curElement = options.at(optionsCounter).toElement();
                        if (curElement.tagName() == "structure")
                            structures.push_back(qMakePair(key, curElement.text()));
                        else if (curElement.tagName() == "commutativity")
                            commutativity.push_back(qMakePair(key, curElement.text()));
                        else if (curElement.tagName() == "associativity")
                            associativity.push_back(qMakePair(key, curElement.text()));
                    }

                }
            }

        }
    }
}
