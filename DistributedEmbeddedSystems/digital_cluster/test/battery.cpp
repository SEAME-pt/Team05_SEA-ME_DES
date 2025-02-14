#include <gtest/gtest.h>
#include <QDebug>
#include <QTest>
#include "../include/mainwindow.h"

class BatteryT : public testing::Test
{
protected:
    MainWindow *mw;
    Battery* battery;
    MockPainter* painter;
    void SetUp() override
    {
        mw = new MainWindow();
        mw->show();
        painter = new MockPainter();
        battery = mw->get_battery();
    }
    void TearDown() override
    {
        painter->end();
        delete painter;
        battery->setTestPainter(nullptr);
        delete mw;
    }
};

TEST_F(BatteryT, TestBatteryState)
{
    EXPECT_CALL(*painter, begin(testing::_)).WillOnce(testing::Return(true));
    EXPECT_CALL(*painter, end()).WillOnce(testing::Return(true));
    EXPECT_CALL(*painter, drawText(testing::_, testing::_, testing::_)).Times(testing::AtLeast(2));
    EXPECT_CALL(*painter, drawArc(testing::_, testing::_, testing::_, testing::_, testing::_, testing::_)).Times(testing::AtLeast(2));
    EXPECT_CALL(*painter, drawPixmap(testing::_, testing::_)).Times(testing::AtLeast(1));
    EXPECT_CALL(*painter, setPen(testing::_)).Times(testing::AtLeast(4));
    EXPECT_CALL(*painter, setFont(testing::_)).Times(testing::AtLeast(2));
    EXPECT_CALL(*painter, boundingRect(testing::_,testing::_, testing::_)).Times(testing::AtLeast(1));
    EXPECT_CALL(*painter, setRenderHint(testing::_, testing::_)).Times(testing::AtLeast(1));
    
    battery->setTestPainter(painter);
    battery->set_current(50);
    EXPECT_EQ(battery->get_current(), 50); 
    QApplication::processEvents();
    QTest::qWait(100);
}

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
