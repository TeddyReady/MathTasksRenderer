#ifndef GROUPPROPERTIES_H
#define GROUPPROPERTIES_H

#include <QRandomGenerator>
#include <QString>
#include <vector>

using gp_type = std::vector<std::tuple<QString, QString, QString>>;

enum class GroupPropertiesOptions {
    Sn, Dn, An, V4, O8, Zn
};

class GroupProperties {
public:
    explicit GroupProperties();

    gp_type generateTasks(int, GroupPropertiesOptions);
private:
    static const int numOfSets;
    gp_type baseData;
    QRandomGenerator *gen;
};

#endif // GROUPPROPERTIES_H
