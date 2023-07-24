include(libs/qtmathjax/qtmathjax.pri)
include(libs/CppMathLib/sources/cppmathlib.pri)

QT       += core gui webkitwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = Algebra_Madness
TEMPLATE = app
CONFIG += c++17
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += \
    dialogmanual.cpp \
    dialogbase.cpp \
    dialoglatexprinter.cpp \
    dialogtesttimer.cpp \
    dialogresults.cpp \
#    libs/mathlib/eulerfunction.cpp \
#    libs/mathlib/groupproperties.cpp \
    libs/mathlib/ieulerfunction.cpp \
    libs/mathlib/imebiusfunction.cpp \
    libs/mathlib/isymbollegandre.cpp \
    main.cpp \
    generatorwindow.cpp \
#    libs/mathlib/mebiusfunction.cpp \
#    libs/mathlib/symbollegandre.cpp \
#    libs/mathlib/ringresidue.cpp \
    testmode.cpp \
#    libs/mathlib/transpositiongroup.cpp \
#    libs/mathlib/algebrastructures.cpp \
    dialogfontsize.cpp \
#    libs/mathlib/ringofmembers.cpp \
    theoryprinter.cpp \

HEADERS += \
    dialogmanual.h \
    dialogbase.h \
    dialoglatexprinter.h \
    dialogtesttimer.h\
    dialogresults.h \
    libs/mathlib/enum.h \
    libs/mathlib/environment.h \
#    libs/mathlib/eulerfunction.h \
    generatorwindow.h \
#    libs/mathlib/groupproperties.h \
    libs/mathlib/ieulerfunction.h \
    libs/mathlib/imatrix.tpp \
    libs/mathlib/imebiusfunction.h \
    libs/mathlib/include.h \
    libs/mathlib/interface.h \
#    libs/mathlib/mebiusfunction.h \
    libs/mathlib/isymbollegandre.h \
    libs/mathlib/metatype.h \
#    libs/mathlib/symbollegandre.h \
#    libs/mathlib/ringresidue.h \
    testmode.h \
#    libs/mathlib/transpositiongroup.h \
#    libs/mathlib/algebrastructures.h \
    dialogfontsize.h \
#    libs/mathlib/ringofmembers.h \

FORMS += \
    dialogbase.ui \
    dialoglatexprinter.ui \
    dialogmanual.ui \
    dialogtesttimer.ui \
    dialogresults.ui \
    generatorwindow.ui \
    testmode.ui \
    dialogfontsize.ui

LIBS += -L/usr/local/lib -lpoppler-qt5

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
RESOURCES += \
    general/resourses.qrc
