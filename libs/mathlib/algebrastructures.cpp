#include "algebrastructures.h"

const int AlgebraStructures::numOfSets = 18;

AlgebraStructures::AlgebraStructures() :
    count(0), gen(QRandomGenerator::global())
{
    QFile dataBase(":/general/data/set.csv");
    if (!dataBase.open(QFile::ReadOnly)) {
        qDebug() << "Внимание! Файл не существует!";
        return;
    } else {
        QTextStream in(&dataBase);
        std::vector<QString> operators;
        QStringList operLine = in.readLine().split(",");
        operLine.removeAt(0);
        for (QString member: operLine) {
            member.replace(" ", "");
            operators.emplace_back(member);
        }
        for (int i = 0; i < numOfSets; i++) {
            QStringList tmp = in.readLine().split(",");
            for (int j = 0; j < operators.size(); j++)
                if (QString(tmp.at(j + 1)).toInt() / 1000 >= 0)
                    baseData.emplace_back(std::make_tuple(tmp.at(0), operators[j], tmp.at(j + 1)));

        } dataBase.close();
    }
}

set_type AlgebraStructures::generateTasks(int countOfTasks, SetOptions option)
{
    set_type data;
    if (option == SetOptions::Check) {
        for(int i = 0; i < countOfTasks;) {
            size_t index = static_cast<size_t>(gen->bounded(0, baseData.size()));
            int answer = QString(std::get<2>(baseData[index])).toInt() / 1000;
            if (answer) {

                QString curSet = std::get<0>(baseData[index]);
                QString curOper = std::get<1>(baseData[index]);

                data.emplace_back(std::make_tuple(getCode(static_cast<Set>(curSet.toInt())),
                                                  curOper,
                                                  getCode(static_cast<SetType>(answer))));
                ++i;
            }
        }
    } else {
        for(int i = 0; i < countOfTasks;) {
            size_t index = static_cast<size_t>(gen->bounded(0, baseData.size()));
            if (option == SetOptions::Oper || QString(std::get<2>(baseData[index])).toInt() / 1000) {
                QString curSet = std::get<0>(baseData[index]);
                QString curOper = std::get<1>(baseData[index]);
                data.emplace_back(std::make_tuple(getCode(static_cast<Set>(curSet.toInt())),
                                        curOper, std::get<2>(baseData[index])));
                ++i;
            }
        }
    } return data;
}

QString AlgebraStructures::getCode(Set set) const
{
    switch (set) {
    case Set::N:
        return "\\mathbb{N}";
    case Set::No:
        return "\\mathbb{N}_0";
    case Set::Z:
        return "\\mathbb{Z}";
    case Set::Z0:
        return "\\mathbb{Z}\\backslash\\{0\\}";
    case Set::Q:
        return "\\mathbb{Q}";
    case Set::Q0:
        return "\\mathbb{Q}\\backslash\\{0\\}";
    case Set::R:
        return "\\mathbb{R}";
    case Set::R0:
        return "\\mathbb{R}\\backslash\\{0\\}";
    case Set::C:
        return "\\mathbb{C}";
    case Set::Sn:
        return "\\mathbb{S}_n";
    case Set::Dn:
        return "\\mathbb{D}_n";
    case Set::An:
        return "\\mathbb{A}_n";
    case Set::V4:
        return "\\mathbb{V}_4";
    case Set::O8:
        return "\\mathbb{Q}_8";
    case Set::Zn:
        return "\\mathbb{Z}_n";
    case Set::Mm_Z:
        return "\\mathbb{M}_{n\\times n}\\left(\\mathbb{Z}\\right)";
    case Set::Mm_R:
        return "\\mathbb{M}_{n\\times n}\\left(\\mathbb{R}\\right)";
    case Set::Mn_Z:
        return "\\mathbb{M}_{n\\times m}\\left(\\mathbb{Z}\\right)";
    case Set::Mn_R:
        return "\\mathbb{M}_{n\\times m}\\left(\\mathbb{R}\\right)";
    }
}

QString AlgebraStructures::getCode(SetType type) const
{
    switch (type) {
    case SetType::Set:
        return "Множество";

    case SetType::Groupoid:
        return "Группоид";

    case SetType::Semigroup:
        return "Полугруппа";

    case SetType::Monoid:
        return "Моноид";

    case SetType::Group:
        return "Группа";
    } return "";
}
