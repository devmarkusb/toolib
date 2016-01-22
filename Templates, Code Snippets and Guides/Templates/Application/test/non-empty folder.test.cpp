#include "gtest/gtest.h"

struct function_or_classTest : public ::testing::Test
{
    function_or_classTest() {}

    void SetUp() {}

    void f() { EXPECT_TRUE(true); }

    void TearDown() {}

    ~function_or_classTest() {}
};


TEST_F(function_or_classTest, test) { f(); }
