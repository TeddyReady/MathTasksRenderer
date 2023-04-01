#ifndef RING_OF_MEMBERS
#define RING_OF_MEMBERS
#include "basemath.h"

enum class RingOfMembersOptions {
    Summary, Multiply, Divide, GCD
};

class RingOfMembers {
public:
    explicit RingOfMembers(bool isModule = false, int module = 0);
    RingOfMembers(const RingOfMembers& other);

    void addElement(int number);
    void setModule(int module);
    int getDeg() const;
    int getModule() const;
    QString getMembers() const;
    QString getBaseSet() const;
    void clear();
    RingOfMembers operator *(const RingOfMembers& member) const;
    RingOfMembers operator +(const RingOfMembers& member) const;
    RingOfMembers operator %(const RingOfMembers& member) const;
    friend RingOfMembers GCD(const RingOfMembers& member_1, const RingOfMembers& member_2);
private:
    QVector<int> members;
    bool isModule;
    int module;
};


#endif //RING_OF_MEMBERS
