QT       += core gui webkitwidgets printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = Algebra_Madness
TEMPLATE = app
CONFIG += c++20
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += \
    basemath.cpp \
    dialogbase.cpp \
    dialoglatexprinter.cpp \
    dialogset.cpp \
    dialogtesttimer.cpp \
    dialogresults.cpp \
    eulerfunction.cpp \
    main.cpp \
    generatorwindow.cpp \
    mebiusfunction.cpp \
    symbollegandre.cpp \
    testmode.cpp \
    transpositiongroup.cpp

HEADERS += \
    basemath.h \
    dialogbase.h \
    dialoglatexprinter.h \
    dialogset.h \
    dialogtesttimer.h\
    dialogresults.h \
    eulerfunction.h \
    generatorwindow.h \
    mebiusfunction.h \
    symbollegandre.h \
    testmode.h \
    transpositiongroup.h

FORMS += \
    dialogbase.ui \
    dialoglatexprinter.ui \
    dialogset.ui \
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
