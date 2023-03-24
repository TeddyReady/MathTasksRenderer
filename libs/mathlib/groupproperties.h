#ifndef GROUPPROPERTIES_H
#define GROUPPROPERTIES_H
#include <QRandomGenerator>
#include <QPushButton>
#include <QTextStream>
#include <QMessageBox>
#include <QString>
#include <QDialog>
#include <QDebug>
#include <QFile>
#include <vector>
#include "basemath.h"

#define GROUPS_COUNT 6

enum class GroupPropertiesOptions {
    SubGroups, NormalSubGroups,
    Center, Order, Exponent
};

enum class Groups {
    Sn, Dn, An, V4, Q8, Zn
};

class GroupProperties {
public:
    explicit GroupProperties();

    void setGroup(Groups gp, int n) {group = gp; this->n = n;}
    int Exp() const;
    QString getCode() const;
    int getGroups() const { return GROUPS_COUNT;}
private:
    Groups group;
    int n;
};

#endif // GROUPPROPERTIES_H
