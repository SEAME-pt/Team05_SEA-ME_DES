#include <gtest/gtest.h>
#include <QDebug>
#include "../include/mainwindow.h"

class AutonomyT : public testing::Test
{
protected:
    MainWindow* mw;
    Autonomy* autonomy;
    void SetUp() override
    {
        mw = new MainWindow();
        mw->show();
        autonomy = mw->get_autonomy();
    }
    void TearDown() override
    {
        delete mw;
    }
};

TEST_F(AutonomyT, TestVar)
{
    // autonomy->set_autonomy(7);
    EXPECT_EQ(autonomy->get_nbsections(), 8);
    EXPECT_EQ(autonomy->get_sections().size(), 8);
    EXPECT_TRUE(autonomy->get_layout() != nullptr);
    EXPECT_TRUE(autonomy->get_mainlayout() != nullptr);
    EXPECT_TRUE(autonomy->get_label() != nullptr);
}