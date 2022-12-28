#include <QSplashScreen>
#include <QApplication>
#include <QThread>
#include "generatorwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap("://other/splash.png"));
    splash->show();

    Qt::Alignment location = Qt::AlignBottom | Qt::AlignCenter;
    splash->showMessage("Устанавливаем таблицу CSS...", location, Qt::black);
    QFile styleFile(":/style.css");
    styleFile.open(QFile::ReadOnly);
    app.setStyleSheet(styleFile.readAll());
    QThread::msleep(1500);
    splash->showMessage("Загружаем приложение...", location, Qt::black);
    GeneratorWindow *mainWindow = new GeneratorWindow;
    mainWindow->setWindowTitle("Algebra Madness");
    mainWindow->setWindowIcon(QIcon("://other/appIcon.png"));
    QThread::msleep(1500);
    mainWindow->show();
    splash->finish(mainWindow);

    delete splash;
    return app.exec();
}
