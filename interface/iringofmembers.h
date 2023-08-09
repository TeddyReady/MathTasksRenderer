#ifndef RINGOFMEMBERSINTERFACE_H
#define RINGOFMEMBERSINTERFACE_H
#include "interface.h"
#include "ringofmembers.tpp"
#include "basemath.h"

enum class RingOfMembersOptions {
    Summary ,
    Diff    ,
    Multiply,
    Divide  ,
    GCD     ,
};

class RingOfMembersInterface : public TaskInterface {
private:
    int minNum, maxNum;
    RingOfMembersOptions option;
    QRandomGenerator *gen;
    Set type;
    int n, module, optional;
    const char *operation;

public:
    explicit RingOfMembersInterface(int minNum, int maxNum, RingOfMembersOptions option, Set type);
    ~RingOfMembersInterface();

    QString printResidue(const char *operation) const;
    QString printAnswers(const std::vector<int> &answers) const;

    void create();
    QString description();
    QString task();
    QString answer();
};

#endif // RINGOFMEMBERSINTERFACE_H
