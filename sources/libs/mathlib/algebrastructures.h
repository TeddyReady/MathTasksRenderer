#ifndef ALGEBRASTRUCTURES_H
#define ALGEBRASTRUCTURES_H
#include "common.h"

enum class Set {
    N,  No, Z,  Z0,
    Q,  Q0, R,  R0,
    C,  Sn, Dn, An,
    V4, O8, Zn, Mm_Z,
    Mm_R, Mn_Z, Mn_R,
    _2A, Func_C, Func_D,
    Pn_X, ABS_C_, V_2D,
    V_nD
};

enum class SetType {
    Set, Groupoid, Semigroup,
        Monoid, Group
};

enum class SetOptions {
      Check, Oper,
    Abel, Associate,
        Neutral
};

using set_type = std::vector<std::tuple<QString, QString, QString>>;


class AlgebraStructures {
public:
    explicit AlgebraStructures();

    set_type generateTasks(int, SetOptions);
    QString getCode(Set) const;
    QString getCode(SetType) const;
    QString getSetName(Set) const ;
    static int getSetsCount() { return numOfSets; }
private:
    static const int numOfSets;
    set_type baseData;
    QRandomGenerator *gen;
};

#endif // ALGEBRASTRUCTURES_H
