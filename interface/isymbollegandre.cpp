#include "isymbollegandre.h"

SymbolLegandreInterface::SymbolLegandreInterface(std::pair<int, int> rangeA, std::pair<int, int> rangeP, SymbolLegandreOptions option)
    : rangeA(rangeA), rangeP(rangeP), option(option), gen(QRandomGenerator::global()) {}
SymbolLegandreInterface::~SymbolLegandreInterface() { gen = nullptr; }

void SymbolLegandreInterface::create()
{
    symbol = std::make_unique<SymbolLegandre>();
    std::vector<std::pair<int, int>> data;

    switch (option)
    {
    case SymbolLegandreOptions::Default:
        do {
            symbol->set(gen->bounded(rangeA.first, rangeA.second), gen->bounded(rangeP.first, rangeP.second));
        } while(not isPrime(symbol->get().second));
        break;

    case SymbolLegandreOptions::Primes:
        do {
            symbol->set(gen->bounded(rangeA.first, rangeA.second), gen->bounded(rangeP.first, rangeP.second));
        } while (not isPrime(symbol->get().first) || not isPrime(symbol->get().second));
        break;

    case SymbolLegandreOptions::aEqual_1:
        do {
            symbol->set(-1, gen->bounded(rangeP.first, rangeP.second));
        } while(not isPrime(symbol->get().second));
        break;

    case SymbolLegandreOptions::NotEvenPrimes:
        do {
            symbol->set(gen->bounded(rangeA.first, rangeA.second), gen->bounded(rangeP.first, rangeP.second));

        } while (GCD(symbol->get().first, symbol->get().second) != 1 || (symbol->get().second + symbol->get().first) % 2 != 0
                 || not isPrime(symbol->get().second));
        break;

    case SymbolLegandreOptions::aEqual2:
        do {
            symbol->set(2, gen->bounded(rangeP.first, rangeP.second));
        } while(not isPrime(symbol->get().second));
        break;
    }
}

QString SymbolLegandreInterface::description()
{
    return QString("Вычислите символ Лежандра");
}

QString SymbolLegandreInterface::task()
{
    return QString(print() + "=~?");
}

QString SymbolLegandreInterface::answer()
{
    return QString::number(symbol->solve());
}

QString SymbolLegandreInterface::print()
{
    return QString("\\left(\\frac{" + QString::number(symbol->get().first) + "}{"
                                    + QString::number(symbol->get().second) + "}\\right)");
}

SymbolJacobiInterface::SymbolJacobiInterface(std::pair<int, int> rangeA, std::pair<int, int> rangeP, SymbolLegandreOptions option)
    : SymbolLegandreInterface(rangeA, rangeP, option) {}

void SymbolJacobiInterface::create()
{
    symbol = std::make_unique<SymbolJacobi>();
    std::vector<std::pair<int, int>> data;

    switch (option)
    {
    case SymbolLegandreOptions::Default:
        do {
            symbol->set(gen->bounded(rangeA.first, rangeA.second), gen->bounded(rangeP.first, rangeP.second));
        } while(isPrime(symbol->get().second));
        break;

    case SymbolLegandreOptions::Primes:
        do {
            symbol->set(gen->bounded(rangeA.first, rangeA.second), gen->bounded(rangeP.first, rangeP.second));
        } while (not isPrime(symbol->get().first) || isPrime(symbol->get().second));
        break;

    case SymbolLegandreOptions::aEqual_1:
        do {
            symbol->set(-1, gen->bounded(rangeP.first, rangeP.second));
        } while(isPrime(symbol->get().second));
        break;

    case SymbolLegandreOptions::NotEvenPrimes:
        do {
            symbol->set(gen->bounded(rangeA.first, rangeA.second), gen->bounded(rangeP.first, rangeP.second));

        } while (GCD(symbol->get().first, symbol->get().second) != 1 || (symbol->get().second + symbol->get().first) % 2 != 0
                 || isPrime(symbol->get().second));
        break;

    case SymbolLegandreOptions::aEqual2:
        do {
            symbol->set(2, gen->bounded(rangeP.first, rangeP.second));
        } while(isPrime(symbol->get().second));
        break;
    }
}

QString SymbolJacobiInterface::description()
{
    return QString("Вычислите символ Якоби");
}
