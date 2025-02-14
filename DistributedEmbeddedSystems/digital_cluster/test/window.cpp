#include <gtest/gtest.h>
#include <QApplication>
#include <QMetaObject>
#include <QDebug>
#include "../include/mainwindow.h"

class Window : public testing::Test {
protected:
    MainWindow* window;
    void SetUp() override {  // new before each test
        window = new MainWindow();
    }
    void TearDown() override { //delete after each test
        delete window;
    }
};

TEST_F(Window, Mqtt) {
    EXPECT_EQ(window->get_client()->state(), QMqttClient::Connected);
    //check signal slot connection 
    const QMetaObject *meta = window->get_client()->metaObject();
    EXPECT_TRUE(meta->indexOfSlot("connected()") >= 0);
    EXPECT_TRUE(meta->indexOfSlot("message_received(const QByteArray&, const QMqttTopicName&)") >= 0);
}