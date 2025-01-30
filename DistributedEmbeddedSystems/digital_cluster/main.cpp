#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTimer>
#include <unistd.h>
#include <iostream>

int main(int argc, char *argv[])
{
    //QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
    //QCoreApplication a(argc, argv);
    QApplication app(argc, argv);
    MainWindow w;
    w.setWindowState(Qt::WindowFullScreen);
    w.show();
    return app.exec();
}
