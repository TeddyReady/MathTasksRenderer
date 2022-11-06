#ifndef EULERFUNCTION_H
#define EULERFUNCTION_H

//#include "generatorwindow.h"
#include "basemath.h"

class EulerFunction
{
    friend class GeneratorWindow;
public:
    explicit EulerFunction();
    explicit EulerFunction(int number);

    int solve();
    void setTask(int num);
private:
    QString taskName;
    int number;
};

#endif // EULERFUNCTION_H
