#ifndef RINGOFMEMBERSINTERFACE_H
#define RINGOFMEMBERSINTERFACE_H
#include "interface.h"
#include "ringofmembers.tpp"
#include "basemath.h"

#define GCD_MAX_CHAIN 3

enum class RingOfMembersOptions {
    Summary ,
    Diff    ,
    Multiply,
    Divide  ,
    Ostat   ,
    GCD     ,
};

template <class T>
class RingOfMembersInterface : public TaskInterface {
private:
    int minDeg, maxDeg;
    RingOfMembersOptions option;
    QRandomGenerator *gen;
    RingOfMembers<T> member_1, member_2;
    QVector<RingOfMembers<T>> members;
    int module;

public:
    explicit RingOfMembersInterface(int minDeg, int maxDeg, RingOfMembersOptions option)
            : minDeg(minDeg), maxDeg(maxDeg), option(option), gen(QRandomGenerator::global()) {}
    ~RingOfMembersInterface() { gen = nullptr; }

    void create()
    {
        module = 0;
        members.clear();

        switch (option)
        {
        case RingOfMembersOptions::Summary:
        case RingOfMembersOptions::Diff:
        case RingOfMembersOptions::Multiply:

            do
                createMembers(member_1, static_cast<int>(gen->bounded(minDeg, maxDeg)));
            while(member_1.isZero());

            do
                createMembers(member_2, static_cast<int>(gen->bounded(minDeg, maxDeg)));
            while(member_2.isZero());
            break;

        case RingOfMembersOptions::Divide:
        case RingOfMembersOptions::Ostat:
        {
            int result_deg = gen->bounded(minDeg, maxDeg);
            int members_count = gen->bounded(1, result_deg);

            for (int i = 0; i < members_count; ++i)
            {
                RingOfMembers<T> tmp;
                do createMembers(tmp, 2); while (tmp.isZero());
                members.push_back(tmp);
            }

            do createMembers(member_2, gen->bounded(2, result_deg - members_count + 2));
            while (member_2.isZero());


            do createMembers(member_1, gen->bounded(1, result_deg - members_count - static_cast<int>(member_2.getDeg()) + 2));
            while (member_1.isZero());
            break;
        }
        case RingOfMembersOptions::GCD:

            int result_deg = gen->bounded(minDeg, maxDeg);
            int members_count = gen->bounded(1, result_deg);

            for (int i = 0; i < members_count; ++i)
            {
                RingOfMembers<T> tmp;
                do createMembers(tmp, 2); while (tmp.isZero());
                members.push_back(tmp);
            }

            do createMembers(member_2, gen->bounded(2, result_deg - members_count + 2));
            while (member_2.isZero());


            do createMembers(member_1, gen->bounded(2, result_deg - members_count + 2));
            while (member_1.isZero());

            break;
        }
    }

    QString description()
    {
        switch (option)
        {
        case RingOfMembersOptions::Summary:
            return QString("Найдите сумму многочленов");
        case RingOfMembersOptions::Diff:
            return QString("Найдите разность многочленов");
        case RingOfMembersOptions::Multiply:
            return QString("Найдите произведение многочленов");
        case RingOfMembersOptions::Divide:
            return QString("Вычислите целую часть от деления многочленлов");
        case RingOfMembersOptions::Ostat:
            return QString("Разделите многочлен с остатком");
        case RingOfMembersOptions::GCD:
            return QString("Найдите НОД многочленов");
        default:
            break;
        }

        return QString("");
    }

    QString task()
    {
        switch (option)
        {
        case RingOfMembersOptions::Summary:
            return QString("%1(%2)+(%3)=~?").arg(getType()).arg(printMembers(member_1)).arg(printMembers(member_2));
        case RingOfMembersOptions::Diff:
            return QString("%1(%2)-(%3)=~?").arg(getType()).arg(printMembers(member_1)).arg(printMembers(member_2));
        case RingOfMembersOptions::Multiply:
            return QString("%1(%2)\\cdot(%3)=~?").arg(getType()).arg(printMembers(member_1)).arg(printMembers(member_2));
        case RingOfMembersOptions::Divide:
            return QString("%1(%2)/(%3)=~?").arg(getType()).arg(printMembers(calculateMembers()*member_2 + member_1)).arg(printMembers(member_2));
        case RingOfMembersOptions::Ostat:
            return QString("%1(%2)~mod~(%3)=~?").arg(getType()).arg(printMembers(calculateMembers()*member_2 + member_1)).arg(printMembers(member_2));
        case RingOfMembersOptions::GCD:
            return QString("%1НОД(%2,%3)=~?").arg(getType()).arg(printMembers(member_1 * calculateMembers())).arg(printMembers(member_2 * calculateMembers()));
        default:
            break;
        }

        return QString("");
    }

    QString answer()
    {
        switch (option)
        {
        case RingOfMembersOptions::Summary:
            return printMembers(member_1 + member_2);
        case RingOfMembersOptions::Diff:
            return printMembers(member_1 - member_2);
        case RingOfMembersOptions::Multiply:
            return printMembers(member_1 * member_2);
        case RingOfMembersOptions::Divide:
            return printMembers(calculateMembers());
        case RingOfMembersOptions::Ostat:
            return printMembers(member_1);
        case RingOfMembersOptions::GCD:
            return printMembers(calculateMembers());
        default:
            break;
        }
    }

    QString printMembers(const RingOfMembers<T> &other) const
    {
        QString members = QString::fromStdString(std::string(other));
        if (members.at(members.size() - 1) == '+')
            members.chop(1);
        return members;
    }

    void createMembers(RingOfMembers<T> &members, int deg)
    {
        std::vector<T> data;

        for(unsigned int i = 0; i < deg; ++i)
            data.push_back(generateValue());

        members = RingOfMembers<T>(data);
    }

    RingOfMembers<T> calculateMembers()
    {
        RingOfMembers<T> tmp = members.first();

        for (int i = 1; i < members.size(); ++i)
            tmp *= members.at(i);

        return tmp;
    }

    QString getType();
    T generateValue();
};

#endif // RINGOFMEMBERSINTERFACE_H
