#ifndef ENUM_H
#define ENUM_H

enum ToolBarOptions {
    Check, Clear, Sep_1,
    Print, Sep_2, Manual,
        Font, Exit
};

enum class Set {
    N,  No, Z,  Z0,
    Q,  Q0, R,  R0,
    C,  Sn, Dn, An,
    V4, O8, Zn, Mm_Z,
    Mm_R, Mn_Z, Mn_R,
    _2A, Func_C, Func_D,
    Pn_X, ABS_C_, V_2D,
    V_nD, Zp, Z_i
};

enum class RingOfMembersOptions {
    Summary, Multiply, Divide, GCD
};

#endif // ENUM_H
