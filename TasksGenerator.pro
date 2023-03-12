include(libs/qtmathjax/qtmathjax.pri)
QT       += core gui webkitwidgets printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = Algebra_Madness
TEMPLATE = app
CONFIG += c++17
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += \
    sources/dialogmanual.cpp \
    libs/mathlib/basemath.cpp \
    sources/dialogbase.cpp \
    sources/dialoglatexprinter.cpp \
    sources/dialogtesttimer.cpp \
    sources/dialogresults.cpp \
    libs/mathlib/eulerfunction.cpp \
    libs/mathlib/groupproperties.cpp \
    sources/main.cpp \
    sources/generatorwindow.cpp \
    libs/mathlib/mebiusfunction.cpp \
    libs/mathlib/symbollegandre.cpp \
    libs/mathlib/ringresidue.cpp \
    sources/testmode.cpp \
    libs/mathlib/transpositiongroup.cpp \
    libs/mathlib/algebrastructures.cpp

HEADERS += \
    sources/dialogmanual.h \
    libs/mathlib/basemath.h \
    sources/dialogbase.h \
    sources/dialoglatexprinter.h \
    sources/dialogtesttimer.h\
    sources/dialogresults.h \
    libs/mathlib/eulerfunction.h \
    sources/generatorwindow.h \
    libs/mathlib/groupproperties.h \
    libs/mathlib/matrix.tpp \
    libs/mathlib/mebiusfunction.h \
    libs/mathlib/symbollegandre.h \
    libs/mathlib/ringresidue.h \
    sources/testmode.h \
    libs/mathlib/transpositiongroup.h \
    libs/mathlib/algebrastructures.h

FORMS += \
    sources/dialogbase.ui \
    sources/dialoglatexprinter.ui \
    sources/dialogmanual.ui \
    sources/dialogtesttimer.ui \
    sources/dialogresults.ui \
    sources/generatorwindow.ui \
    sources/testmode.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
RESOURCES += \
    resourses.qrc
