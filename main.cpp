#include <QApplication>
#include "generatorwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    GeneratorWindow w;
    w.show();

    return app.exec();
}
