#ifndef BOOLEANFUNCTIONINTERFACE_H
#define BOOLEANFUNCTIONINTERFACE_H
#include "interface.h"
#include "booleanfunction.h"

enum class BooleanFunctionOptions {
    PolinomJegalkina,
};

class BooleanFunctionInterface : public TaskInterface {
private:
    int minNum, maxNum;
    BooleanFunctionOptions option;
    QRandomGenerator *gen;
    BooleanFunction function;

public:
    explicit BooleanFunctionInterface(int minNum, int maxNum, BooleanFunctionOptions option);
    ~BooleanFunctionInterface();

    void create();
    QString description();
    QString task();
    QString answer();
};

#endif // BOOLEANFUNCTIONINTERFACE_H
