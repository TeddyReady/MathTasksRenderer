#include "imebiusfunction.h"

MebiusFunctionInterface::MebiusFunctionInterface(int minNum, int maxNum, MebiusFunctionOptions option)
    : minNum(minNum), maxNum(maxNum), option(option), gen(QRandomGenerator::global()) {}
MebiusFunctionInterface::~MebiusFunctionInterface() { gen = nullptr; }

void MebiusFunctionInterface::create()
{
    std::vector<std::pair<int, int>> data;
    bool isCorrect = false;

    switch (option)
    {
    case MebiusFunctionOptions::Default:
        function.set(gen->bounded(minNum, maxNum));
        break;

    case MebiusFunctionOptions::Primes:
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

    case MebiusFunctionOptions::PrimeDegree:
        do {

            function.set(gen->bounded(minNum, maxNum));

            data = decompositionToSimple(function.get());
            if (data.size() == 1) continue;

            for (size_t i = 0; i < data.size(); ++i)
            {
                if (data[i].second > 1)
                {
                    isCorrect = true;
                    break;
                }
            }

        } while (not isCorrect);
        break;

    case MebiusFunctionOptions::NotEvenPrimes:
        do {
            function.set(gen->bounded(minNum, maxNum));

            data = decompositionToSimple(function.get());

            if (data.size() % 2 == 0) continue;
            isCorrect = true;

            for (size_t i = 0; i < data.size(); ++i)
            {
                if (data[i].second > 1)
                {
                    isCorrect = false;
                    break;
                }
            }

        } while (not isCorrect);
        break;

    case MebiusFunctionOptions::EvenPrimes:
        do {
            function.set(gen->bounded(minNum, maxNum));

            data = decompositionToSimple(function.get());

            if (data.size() % 2 != 0) continue;
            isCorrect = true;

            for (size_t i = 0; i < data.size(); ++i)
            {
                if (data[i].second > 1)
                {
                    isCorrect = false;
                    break;
                }
            }

        } while (not isCorrect);
        break;
    }
}

QString MebiusFunctionInterface::description()
{
    return QString("Вычислите функцию Мёбиуса");
}

QString MebiusFunctionInterface::task()
{
    return QString("\\mu(" + QString::number(function.get()) + ")=~?");
}

QString MebiusFunctionInterface::answer()
{
    return QString::number(function.solve());
}
