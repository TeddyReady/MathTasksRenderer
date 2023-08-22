#include "ialgebrastructures.h"

AlgebraStructuresInterface::AlgebraStructuresInterface(AlgebraStructuresOptions option)
    : option(option), gen(QRandomGenerator::global()) { parseTasks(RSC::data::algebra_structures); }
AlgebraStructuresInterface::~AlgebraStructuresInterface() { gen = nullptr; }

void AlgebraStructuresInterface::create()
{

}

QString AlgebraStructuresInterface::description()
{

}

QString AlgebraStructuresInterface::task()
{

}

QString AlgebraStructuresInterface::answer()
{

}

void AlgebraStructuresInterface::parseTasks(const QString &xml_file)
{

}
