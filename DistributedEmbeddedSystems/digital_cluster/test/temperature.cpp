#include <gtest/gtest.h>
#include <QDebug>
#include "../include/mainwindow.h"

class TemperatureT : public testing::Test
{
protected:
    MainWindow* mw;
    Temperature* temperature;
    void SetUp() override
    {
        mw = new MainWindow();
        mw->show();
        temperature = mw->get_temperature();
    }
    void TearDown() override
    {
        delete mw;
    }
};

TEST_F(TemperatureT, TestVar)
{
    // temperature->set_temperature(70);
    EXPECT_EQ(temperature->get_nbsections(), 8);
    EXPECT_EQ(temperature->get_sections().size(), 8);
    EXPECT_TRUE(temperature->get_layout() != nullptr);
    EXPECT_TRUE(temperature->get_mainlayout() != nullptr);
    EXPECT_TRUE(temperature->get_label() != nullptr);
}