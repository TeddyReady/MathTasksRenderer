#ifndef ALGEBRASTRUCTURES_H
#define ALGEBRASTRUCTURES_H
#include "basemath.h"

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
