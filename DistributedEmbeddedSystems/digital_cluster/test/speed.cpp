#include <gtest/gtest.h>
#include <QApplication>
#include <QDebug>
#include "../include/speed.h"
#include <QTimer>
#include <QEventLoop>

class SpeedT : public testing::Test {
protected:
    Speed* speed;
    void SetUp() override {  // new before each test
        speed = new Speed();
    }
    void TearDown() override { //delete after each test
        delete speed;
    }
};

TEST_F(SpeedT, Current_target) {    
    speed->set_current(3);
    EXPECT_FLOAT_EQ(speed->get_current(), static_cast<int>(3 * 3.6));
    float expected_target_angle = (speed->get_current() * 270.0f) / speed->get_max();
    expected_target_angle = std::min(expected_target_angle, 270.0f);
    EXPECT_FLOAT_EQ(speed->get_target_angle(), expected_target_angle); //test target angle
}

TEST_F(SpeedT, Animation) {
    speed->set_current(3);
    EXPECT_TRUE(speed->get_is_animating());
    QTimer timer;
    QEventLoop loop;
    QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.start(200);
    loop.exec();
    EXPECT_TRUE(speed->get_current_angle() > 0);  //test current angle
}
