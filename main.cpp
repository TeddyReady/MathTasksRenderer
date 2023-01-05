#include <QSplashScreen>
#include <QApplication>
#include <QThread>
#include "generatorwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/img/splash.png"));
    splash->show();

    Qt::Alignment location = Qt::AlignBottom | Qt::AlignCenter;
    splash->showMessage("Устанавливаем таблицу CSS...", location, Qt::black);
    QFile styleFile(":/general/style.css");
    styleFile.open(QFile::ReadOnly);
    app.setStyleSheet(styleFile.readAll());
    styleFile.close();
    QThread::msleep(1500);
    splash->showMessage("Загружаем приложение...", location, Qt::black);
    GeneratorWindow *mainWindow = new GeneratorWindow;
    mainWindow->setWindowTitle("Algebra Madness");
    mainWindow->setWindowIcon(QIcon(":/img/appIcon.png"));
    QThread::msleep(500);
    mainWindow->show();
    splash->finish(mainWindow);
    delete splash;

    QApplication::restoreOverrideCursor();
    return app.exec();
}
