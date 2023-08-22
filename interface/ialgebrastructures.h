#ifndef ALGEBRASTRUCTURES_H
#define ALGEBRASTRUCTURES_H
#include "environment.h"
#include "interface.h"
#include "basemath.h"

enum class AlgebraStructuresOptions {
    CheckStructure  ,
    CheckOperation  ,
    Commutativity   ,
    Associativity   ,
};

class AlgebraStructuresInterface : public TaskInterface {
private:
    QVector<QPair<QString, QString>> structures, commutativity, associativity;
    AlgebraStructuresOptions option;
    QRandomGenerator *gen;
    int taskLocation;

public:
    explicit AlgebraStructuresInterface(AlgebraStructuresOptions option);
    ~AlgebraStructuresInterface();

    void create();
    QString description();
    QString task();
    QString answer();

    void parseTasks(const QString &xml_file);
};

#endif // ALGEBRASTRUCTURES_H
