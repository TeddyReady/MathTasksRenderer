#ifndef MEBIUSFUNCTION_H
#define MEBIUSFUNCTION_H
#include "basemath.h"

class MebiusFunction {
public:
    explicit MebiusFunction();
    explicit MebiusFunction(int number);

    int solve();
    void setTask(int num);
    int getTask();
private:
    int number;
};

#endif // MEBIUSFUNCTION_H
