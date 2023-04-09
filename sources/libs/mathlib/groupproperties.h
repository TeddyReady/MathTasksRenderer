#ifndef GROUPPROPERTIES_H
#define GROUPPROPERTIES_H
#include "basemath.h"

#define GROUPS_COUNT 6

enum class GroupPropertiesOptions {
    SubGroups, NormalSubGroups,
    Center, Order, Exponent
};

class GroupProperties {
public:
    explicit GroupProperties();

    void setGroup(Set gp, int n) {group = gp; this->n = n;}
    int Exp() const;
    QString getCode() const;
    int getGroups() const { return GROUPS_COUNT;}
private:
    Set group;
    int n;
};

#endif // GROUPPROPERTIES_H
