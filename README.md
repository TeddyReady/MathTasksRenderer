# MathTasksRenderer

## About project
MathTasksRenderer - application with user interface, written on C++/Qt with MathJax render support. This application was created for learning Higher Algebra. 

In this project released next themes:
1. Algebra Structures (groups, rings, fields and other structures);
2. Symmetric group (transposition group);
3. Residue-class;
4. Polynomial ring;
5. Complex numbers;
6. Matrix ring;
7. Jacobi and Legandre symbols;
8. Euler and Mebius functions;

At this moment, project have only **russian** localization.

[Actually releases](https://drive.google.com/drive/folders/1ZljdK5I1yh3rMvvRP4rtfFE1fB51Hb61?usp=share_link)

## Install and build from sources
All operations were performed on Ubuntu 22.04 and 23.04

This application using as submodule self-written mathematical module [CppMathLib](https://github.com/TeddyReady/CppMathLib.git);

Firstly, install necessary packages:

    # apt update && sudo apt upgrade -y
    # apt install -y qtbase5-dev qtbase5-doc qtbase5-examples libqt5core5a qt5-doc qt5-assistant libqt5xml5 libqt5webview5 libqt5webview5-dev libqt5websockets5-dev libqt5webkit5-dev libqt5webenginewidgets5 libqt5webview5-dev libqt5svg5-dev libqt5script5 libqt5qml5 libqt5pdfwidgets5 libqt5dbus5 libqt53dcore5 libqt53danimation5 libpoppler-qt5-dev libphonon4qt5-dev qttools5-dev qtmultimedia5-dev qtbase5-examples qtbase5-doc-html
    # apt install build-essential make cmake git

Next, cloning this repo:

    $ https://github.com/TeddyReady/MathTasksRenderer.git && cd MathTasksRenderer

Let's build:

    $ mkdir build && cd build
    $ qmake .. && make -j`nproc`

Now, we can starting application:

    $ ./Algebra_Madness
