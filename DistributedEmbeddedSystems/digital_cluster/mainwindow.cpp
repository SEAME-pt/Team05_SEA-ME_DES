#include "mainwindow.h"
#include <iostream>
#include <QDir>
#include <QCoreApplication>
#include <QFontDatabase>
#include <unistd.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
     , mqttClient(new QMqttClient(this))
{
    setStyleSheet("background-color: rgb(2, 1, 30);");
    left_dial = new CustomDial(this);
    right_dial = new BatteryDial(this);
    QHBoxLayout* layout = new QHBoxLayout(); //horizontal layout
    layout->addWidget(left_dial, 1, Qt::AlignCenter);
    layout->addWidget(right_dial, 1, Qt::AlignCenter);
    QVBoxLayout* mainlayout = new QVBoxLayout();
    mainlayout->addLayout(layout, 4);

    temp = new TempBar(this);
    autonomy = new AutonomyBar(this);
    QHBoxLayout* layoutbar = new QHBoxLayout();
    layoutbar->setSpacing(80);
    layoutbar->addWidget(temp, 1, Qt::AlignRight);
    layoutbar->addWidget(autonomy, 1, Qt::AlignLeft);
    mainlayout->addLayout(layoutbar, 1);
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(mainlayout);
    setCentralWidget(centralWidget);

    init_mqtt();
}

MainWindow::~MainWindow()
{
    std::cout << "Removing mainwindow" << std::endl;
}

void MainWindow::init_mqtt() {
    // mqttClient->setHostname("972e24210b544ba49bfb9c1d3164d02b.s1.eu.hivemq.cloud");
    // mqttClient->setPort(8883);
    // mqttClient->setUsername("jetracer"); 
    // mqttClient->setPassword("Ft_seame5"); 
    // mqttClient->setHostname("10.21.221.67");
    mqttClient->setPort(1883);
    mqttClient->setHostname("127.0.0.1");

    connect(mqttClient, &QMqttClient::connected, this, &MainWindow::onMqttConnected);
    connect(mqttClient, &QMqttClient::messageReceived, this, &MainWindow::onMessageReceived);
    connect(mqttClient, &QMqttClient::errorChanged, this, [](QMqttClient::ClientError error) {
    qDebug() << "MQTT Client error:" << error;
    });
    mqttClient->connectToHost();
}

void MainWindow::onMqttConnected() {
    qDebug() << "Connected to MQTT broker";
    QMqttTopicFilter topic("jetracer/speed");
    auto subscription = mqttClient->subscribe(topic);
    QMqttTopicFilter battery("jetracer/battery");
    auto subscribe = mqttClient->subscribe(battery);
    QMqttTopicFilter temp("jetracer/temperature");
    auto sub = mqttClient->subscribe(temp);
    if (!subscription || !sub | !subscribe) {
        qDebug() << "Failed to subscribe to topic";
    } else {
        qDebug() << "Successfully subscribed to topic";
    }
}

void MainWindow::onMessageReceived(const QByteArray &message, const QMqttTopicName &topic)
{
    qDebug() << "Message received on topic" << topic.name() << ":" << message;
    bool ok;
    double msg = message.toDouble(&ok); 
    if (ok) {
        if (topic.name() == "jetracer/speed") {
            QMetaObject::invokeMethod(this, [this, msg]() {
                qDebug() << "Updating speed: " << msg;
                left_dial->set_current(msg);
            }, Qt::QueuedConnection);
        }
        else if (topic.name() == "jetracer/battery") {
            QMetaObject::invokeMethod(this, [this, msg]() {
                qDebug() << "Updating battery: " << msg;
                right_dial->set_current(msg);
            }, Qt::QueuedConnection);
        }
        else if (topic.name() == "jetracer/temperature") {
            QMetaObject::invokeMethod(this, [this, msg]() {
                qDebug() << "Updating temperature: " << msg;
                temp->set_temperature(msg);
            }, Qt::QueuedConnection);
        }
    } else {
        qDebug() << "Invalid data received";
    }
}
