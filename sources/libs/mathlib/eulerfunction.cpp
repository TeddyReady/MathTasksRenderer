#include "eulerfunction.h"
EulerFunction::EulerFunction() : number(1) {}
EulerFunction::EulerFunction(int number) : number(number) {}

void EulerFunction::create(const QVector<QVariant> &options)
{
    QPair<int, int> range = options.at(0).value<QPair<int, int>>();

    QRandomGenerator *random = QRandomGenerator::global();
    std::vector<std::pair<int, int>> data;

    switch (taskType) {
    case EulerFunctionOptions::Default:
        number = (random->bounded(range.first, range.second));
        break;
    case EulerFunctionOptions::Primes:
        do {
            number = (random->bounded(range.first, range.second));
        } while (!isPrime(getTask()));
        break;
    case EulerFunctionOptions::PrimesDegrees:
        do {
            number = (random->bounded(range.first, range.second));
            data = decompositionToSimple(getTask());
        } while (data.size() != 1 || data[0].second <= 1);
        break;
    case EulerFunctionOptions::MultiplyPrimes:
        do {
            bool access = false;

            number = (random->bounded(range.first, range.second));

            data = decompositionToSimple(getTask());

            for (size_t i = 0; i < data.size(); ++i) {
                if (data[i].second != 1) {
                    access = true;
                    break;
                }
            } if (access) continue;
        } while (data.size() == 1);
        break;
    case EulerFunctionOptions::MultiplyPrimesDegrees:
        do {
            bool access = false;

            number = (random->bounded(range.first, range.second));

            data = decompositionToSimple(getTask());

            for (size_t i = 0; i < data.size(); ++i){
                if (data[i].second == 1) {
                    access = true;
                    break;
                }
            } if (access) continue;
        } while (data.size() == 1);
        break;
    }
    random = nullptr;
}

QString EulerFunction::description()
{
    return QString("Вычислите функцию Эйлера");
}

QString EulerFunction::task(const QString &optional)
{
    Q_UNUSED(optional);
    return QString("\\varphi(" + QString::number(getTask()) + ")=~?");
}

QString EulerFunction::answer(const QVariant &other)
{
    Q_UNUSED(other);
    return QString::number(solve());
}

void EulerFunction::setTaskOption(EulerFunctionOptions option)
{
    taskType = option;
}

int EulerFunction::solve() const {
    auto data = decompositionToSimple(number);
    int result = 1;
    for (size_t i = 0; i < data.size(); ++i){
        result *= fastPower(data[i].first, data[i].second) - fastPower(data[i].first, data[i].second - 1);
    }
    return result;
}
