#ifndef KELITABLEINTERFACE_H
#define KELITABLEINTERFACE_H
#include "interface.h"
#include "kelitable.h"
#include "basemath.h"

enum class KeliTableOptions
{
    Isomorph, Abel,
};

class KeliTableInterface : public TaskInterface {
private:
    int minNum, maxNum;
    KeliTableOptions option;
    QRandomGenerator *gen;
    KeliTable table;
    QVector<std::string> realVars, masqVars;
    bool _answer;

    void generateVars(int volume);
    std::string getCodeByStaticIndex(int index, int tableNumber) const;
    QString masqueTable();
    void randomSortTable();

public:
    explicit KeliTableInterface(int minNum, int maxNum, KeliTableOptions option);
    ~KeliTableInterface();

    void create();
    QString description();
    QString task();
    QString answer();
};

#endif // KELITABLEINTERFACE_H
