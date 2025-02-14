#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <cstdio>
#include <cstdlib>
#include "speed.h"
#include "battery.h"
#include <QVBoxLayout>
#include <QtMqtt/QtMqtt>
#include <QtMqtt/QMqttClient>
#include <QtMqtt/QMqttTopicFilter>
#include <QtMqtt/QMqttMessage>
#include <QtMqtt>
#include <QApplication>
#include "temperature.h"
#include "autonomy.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QMqttClient* get_client();
    Battery *get_battery();
    Autonomy *get_autonomy();
    Temperature *get_temperature();

private slots:
    void connected();
    void message_received(const QByteArray &message, const QMqttTopicName &topic); // Declaração para receber mensagens
    void init_mqtt();

private:
    Speed *left_dial = nullptr;
    Battery *right_dial = nullptr;
    QMqttClient *client;
    Temperature *temp;
    Autonomy *autonomy;

};
#endif // MAINWINDOW_H
