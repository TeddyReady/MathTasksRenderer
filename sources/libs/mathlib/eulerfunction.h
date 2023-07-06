#ifndef EULERFUNCTION_H
#define EULERFUNCTION_H
#include "basemath.h"

enum class EulerFunctionOptions {
    Default, Primes,
    PrimesDegrees, MultiplyPrimes,
    MultiplyPrimesDegrees
};

class EulerFunction : protected BaseMath {
public:
    explicit EulerFunction();
    explicit EulerFunction(int number);

    void create(const QVector<QVariant> &options);
    QString description();
    QString task(const QString &optional);
    QString answer(const QVariant &other);

    void setTaskOption(EulerFunctionOptions option);
    int solve() const;
    void setTask(int num) { number = num; }
    int getTask() const { return number; }
private:
    EulerFunctionOptions taskType;
    int number;
};

#endif // EULERFUNCTION_H
