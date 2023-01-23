QT       += core gui webkitwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = Algebra_Madness
TEMPLATE = app
CONFIG += c++20
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += \
    basemath.cpp \
    dialogeulerfunction.cpp \
    dialogmebiusfunction.cpp \
    dialogset.cpp \
    dialogtesttimer.cpp \
    dialogresults.cpp \
    dialogsymboljacobi.cpp \
    dialogsymbollegandre.cpp \
    dialogtranspositiongroup.cpp \
    eulerfunction.cpp \
    main.cpp \
    generatorwindow.cpp \
    mebiusfunction.cpp \
    symboljacobi.cpp \
    symbollegandre.cpp \
    testmode.cpp \
    transpositiongroup.cpp

HEADERS += \
    basemath.h \
    dialogeulerfunction.h \
    dialogmebiusfunction.h \
    dialogset.h \
    dialogtesttimer.h\
    dialogresults.h \
    dialogsymboljacobi.h \
    dialogsymbollegandre.h \
    dialogtranspositiongroup.h \
    eulerfunction.h \
    generatorwindow.h \
    mebiusfunction.h \
    symboljacobi.h \
    symbollegandre.h \
    testmode.h \
    transpositiongroup.h

FORMS += \
    dialogeulerfunction.ui \
    dialogmebiusfunction.ui \
    dialogset.ui \
    dialogtesttimer.ui \
    dialogresults.ui \
    dialogsymboljacobi.ui \
    dialogsymbollegandre.ui \
    dialogtranspositiongroup.ui \
    generatorwindow.ui \
    testmode.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
RESOURCES += \
    resourses.qrc
include(libs/qtmathjax/qtmathjax.pri)
