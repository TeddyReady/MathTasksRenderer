#include <QApplication>
#include "generatorwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QFile styleFile(":/style.css");
    styleFile.open(QFile::ReadOnly);
    app.setStyleSheet(styleFile.readAll());
    GeneratorWindow w;
    w.show();

    return app.exec();
}
