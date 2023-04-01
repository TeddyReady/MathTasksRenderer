#include "ringofmembers.h"

RingOfMembers::RingOfMembers(bool isModule, int module)
{
    this->isModule = isModule;
    this->module = module;
}

RingOfMembers::RingOfMembers(const RingOfMembers& other)
{
    members = other.members;
    isModule = other.isModule;
    module = other.module;
}

void RingOfMembers::addElement(int number)
{
    if (isModule)
        number %= module;

    if (members.isEmpty() && number == 0)
        members.push_back(1);
    else members.push_back(number);
}

void RingOfMembers::setModule(int module)
{
    isModule = true;
    this->module = module;
}

int RingOfMembers::getDeg() const { return members.size(); }

int RingOfMembers::getModule() const { return module; }

QString RingOfMembers::getMembers() const
{
    QString result;
    for (int i = 0; i < members.size(); ++i)
    {
        if (members[i] == 0) continue;
        if (result.isEmpty()) {
            if (members[i] == 1)
                result.append("x^{" + QString::number(members.size() - i) + "}");
            else if (members[i] == -1)
                result.append("-x^{" + QString::number(members.size() - i) + "}");
            else result.append(QString::number(members[i]) + "x^{" + QString::number(members.size() - i) + "}");
        } else if (i == members.size() - 1) {
            if (members[i] > 0)
                result.append("+" + QString::number(members[i]));
            else result.append(QString::number(members[i]));
        }
        else {
            if (members[i] == 1)
                result.append("+x^{" + QString::number(members.size() - i) + "}");
            else if (members[i] > 0)
                result.append("+" + QString::number(members[i]) + "x^{" + QString::number(members.size() - i) + "}");
            else {
                if (members[i] == -1)
                    result.append("-x^{" + QString::number(members.size() - i) + "}");
                else result.append(QString::number(members[i]) + "x^{" + QString::number(members.size() - i) + "}");
            }
        }
    } return result;
}

QString RingOfMembers::getBaseSet() const
{
    if (isModule) return QString("Z_{" + QString::number(module) + "}[x]");
    else return QString("Z[x]");
}

void RingOfMembers::clear()
{
    members.clear();
    module = -1;
    isModule = false;
}

RingOfMembers RingOfMembers::operator +(const RingOfMembers& member) const
{
    RingOfMembers mini, maxi;
    if (this->getDeg() > member.getDeg()) {
        mini = member;
        maxi = *this;
    } else {
        mini = *this;
        maxi = member;
    }

    for (int i = mini.getDeg(); i < max(mini.getDeg(), maxi.getDeg()); ++i)
        mini.members.push_front(0);

    RingOfMembers result(isModule, module);
    int maxDeg = max(this->getDeg(), member.getDeg());
    for (int i = 0; i < maxDeg; ++i)
        result.addElement(mini.members[i] + maxi.members[i]);

    return result;
}

RingOfMembers RingOfMembers::operator *(const RingOfMembers& member) const{}

RingOfMembers RingOfMembers::operator %(const RingOfMembers& member) const{}

RingOfMembers GCD(const RingOfMembers& member_1, const RingOfMembers& member_2)
{

}
