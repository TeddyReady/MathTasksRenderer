#include "groupproperties.h"

GroupProperties::GroupProperties()
{
    QFile dataBase(":/general/data/groups.csv");
    if (!dataBase.open(QFile::ReadOnly)) {
        qDebug() << "Внимание! Файл не существует!";
        return;
    } else {
        QTextStream in(&dataBase);
        std::vector<QString> operators;
        QStringList operLine = in.readLine().split(",");
        operLine.removeAt(0);
    }
}

int GroupProperties::Exp() const
{

    switch (group) {
    case Set::Sn:
    {
        int result = 1;
        for (int i = 2; i <= n; ++i)
            result = LCM(result, i);
        return result;
    }
    case Set::Dn:
        return 0;
    case Set::An:
    {
        int result = 1;
        for (int i = 1; i <= n; i += 2)
            result = LCM(result, i);
        return result;
    }
    case Set::V4:
        return 2;
    case Set::O8:
        return 6;
    case Set::Zn:
        return 0;
    }
}

QString GroupProperties::getCode() const
{
    switch (group) {
    case Set::Sn:
        return "\\mathbb{S}_{" + QString::number(n) + "}";
    case Set::Dn:
        return "\\mathbb{D}_{" + QString::number(n) + "}";
    case Set::An:
        return "\\mathbb{A}_{" + QString::number(n) + "}";
    case Set::V4:
        return "\\mathbb{V}_4";
    case Set::O8:
        return "\\mathbb{Q}_8";
    case Set::Zn:
        return "\\mathbb{Z}_{" + QString::number(n) + "}";
    }
}
