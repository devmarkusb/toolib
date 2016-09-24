#include "LibName/non-empty folder.h"
#include "gtest/gtest.h"


struct function_or_classTest : public ::testing::Test
{
    function_or_classTest() {}

    virtual void SetUp() override {}

    void f() { EXPECT_TRUE(true); }

    virtual void TearDown() override {}

    ~function_or_classTest() {}
};


TEST_F(function_or_classTest, test) { f(); }
