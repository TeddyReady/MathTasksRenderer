#ifndef EULERFUNCTION_H
#define EULERFUNCTION_H
#include "basemath.h"

class EulerFunction {
public:
    explicit EulerFunction();
    explicit EulerFunction(int number);

    int solve();
    void setTask(int num);
    int getTask();
private:
    int number;
};

#endif // EULERFUNCTION_H
