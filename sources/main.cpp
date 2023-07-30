#include "generatorwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setOrganizationName(App::OrgName);
    QApplication::setApplicationName(App::AppName);

    QApplication::setOverrideCursor(Qt::WaitCursor);

    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(RSC::pics::splash));
    splash->show();

    Qt::Alignment location = Qt::AlignBottom | Qt::AlignCenter;
    splash->showMessage("Устанавливаем таблицу CSS...", location, Qt::black);
    QFile styleFile(RSC::general::style);
    styleFile.open(QFile::ReadOnly);
    app.setStyleSheet(styleFile.readAll());
    styleFile.close();
    //QThread::msleep(1500);
    splash->showMessage("Загружаем приложение...", location, Qt::black);
    GeneratorWindow *mainWindow = new GeneratorWindow;
    //QThread::msleep(500);
    splash->finish(mainWindow);
    delete splash;

    QApplication::restoreOverrideCursor();
    return app.exec();
}
