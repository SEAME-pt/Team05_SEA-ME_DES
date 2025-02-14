#include "./include/mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTimer>
#include <unistd.h>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    MainWindow w;
    w.setWindowState(Qt::WindowFullScreen);
    w.show();
    return app.exec();
}
