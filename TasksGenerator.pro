QT       += core gui webkitwidgets printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = Algebra_Madness
TEMPLATE = app
CONFIG += c++17
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += \
    dialogmanual.cpp \
    libs/mathlib/basemath.cpp \
    dialogbase.cpp \
    dialoglatexprinter.cpp \
    dialogtesttimer.cpp \
    dialogresults.cpp \
    libs/mathlib/eulerfunction.cpp \
    libs/mathlib/groupproperties.cpp \
    main.cpp \
    generatorwindow.cpp \
    libs/mathlib/mebiusfunction.cpp \
    libs/mathlib/symbollegandre.cpp \
    libs/mathlib/ringresidue.cpp \
    testmode.cpp \
    libs/mathlib/transpositiongroup.cpp \
    libs/mathlib/algebrastructures.cpp

HEADERS += \
    dialogmanual.h \
    libs/mathlib/basemath.h \
    dialogbase.h \
    dialoglatexprinter.h \
    dialogtesttimer.h\
    dialogresults.h \
    libs/mathlib/eulerfunction.h \
    generatorwindow.h \
    libs/mathlib/groupproperties.h \
    libs/mathlib/matrix.tpp \
    libs/mathlib/mebiusfunction.h \
    libs/mathlib/symbollegandre.h \
    libs/mathlib/ringresidue.h \
    testmode.h \
    libs/mathlib/transpositiongroup.h \
    libs/mathlib/algebrastructures.h

FORMS += \
    dialogbase.ui \
    dialoglatexprinter.ui \
    dialogmanual.ui \
    dialogtesttimer.ui \
    dialogresults.ui \
    generatorwindow.ui \
    testmode.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
RESOURCES += \
    resourses.qrc
include(libs/qtmathjax/qtmathjax.pri)
