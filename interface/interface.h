#ifndef INTERFACE_H
#define INTERFACE_H
#include "metatype.h"

/* Iterface for math classes */
class TaskInterface {
public:
    virtual ~TaskInterface() = default;

    virtual void create() = 0;
    virtual QString description() = 0;
    virtual QString task() = 0;
    virtual QString answer() = 0;
};

#endif // INTERFACE_H
