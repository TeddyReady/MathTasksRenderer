#include "ieulerfunction.h"

EulerFunctionInterface::EulerFunctionInterface(int minNum, int maxNum, EulerFunctionOptions option)
    : minNum(minNum), maxNum(maxNum), option(option), gen(QRandomGenerator::global()) {}
EulerFunctionInterface::~EulerFunctionInterface() { gen = nullptr; }

void EulerFunctionInterface::create()
{
    std::vector<std::pair<int, int>> data;
    bool isCorrect = false;

    switch (option)
    {
    case EulerFunctionOptions::Default:
        function.set(gen->bounded(minNum, maxNum));
        break;

    case EulerFunctionOptions::Primes:
        do {
            function.set(gen->bounded(minNum, maxNum));
        } while (!isPrime(function.get()));
        break;

    case EulerFunctionOptions::PrimesDegrees:
        do {
            function.set(gen->bounded(minNum, maxNum));
            data = decompositionToSimple(function.get());
        } while (data.size() != 1 || data[0].second == 1);
        break;

    case EulerFunctionOptions::MultiplyPrimes:
        do {
            function.set(gen->bounded(minNum, maxNum));

            data = decompositionToSimple(function.get());
            if (data.size() == 1) continue;

            isCorrect = true;
            for (size_t i = 0; i < data.size(); ++i)
            {
                if (data[i].second != 1 || !isPrime(data[i].first))
                {
                    isCorrect = false;
                    break;
                }
            }

        } while (not isCorrect);
        break;

    case EulerFunctionOptions::MultiplyPrimesDegrees:
        do {
            function.set(gen->bounded(minNum, maxNum));

            data = decompositionToSimple(function.get());
            if (data.size() == 1) continue;

            isCorrect = true;
            for (size_t i = 0; i < data.size(); ++i)
            {
                if (data[i].second == 1)
                {
                    isCorrect = false;
                    break;
                }
            }

        } while (not isCorrect);
        break;
    }
}

QString EulerFunctionInterface::description()
{
    return QString("Вычислите функцию Эйлера");
}

QString EulerFunctionInterface::task()
{
    return QString("\\varphi(" + QString::number(function.get()) + ")=~?");
}

QString EulerFunctionInterface::answer()
{
    return QString::number(function.solve());
}
