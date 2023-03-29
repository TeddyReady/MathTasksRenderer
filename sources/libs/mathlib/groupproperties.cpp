#include "groupproperties.h"

GroupProperties::GroupProperties()
{
    QFile dataBase(":/general/data/group.csv");
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
    case Groups::Sn:
    {
        int result = 1;
        for (int i = 2; i <= n; ++i)
            result = LCM(result, i);
        return result;
    }
    case Groups::Dn:
        return 0;
    case Groups::An:
    {
        int result = 1;
        for (int i = 1; i <= n; i += 2)
            result = LCM(result, i);
        return result;
    }
    case Groups::V4:
        return 2;
    case Groups::Q8:
        return 6;
    case Groups::Zn:
        return 0;
    }
}

QString GroupProperties::getCode() const
{
    switch (group) {
    case Groups::Sn:
        return "\\mathbb{S}_{" + QString::number(n) + "}";
    case Groups::Dn:
        return "\\mathbb{D}_{" + QString::number(n) + "}";
    case Groups::An:
        return "\\mathbb{A}_{" + QString::number(n) + "}";
    case Groups::V4:
        return "\\mathbb{V}_4";
    case Groups::Q8:
        return "\\mathbb{Q}_8";
    case Groups::Zn:
        return "\\mathbb{Z}_{" + QString::number(n) + "}";
    }
}
