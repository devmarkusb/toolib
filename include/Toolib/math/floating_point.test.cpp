#include "Toolib/math/floating_point.h"
#include "gtest/gtest.h"


TEST(Math_almost_equal_Test, test)
{
    EXPECT_TRUE(too::math::almost_equal(1.0, 1.0));
    EXPECT_FALSE(too::math::almost_equal(1.0, 1.0001));
}

TEST(Math_almost_equal_alltypes_Test, test)
{
    EXPECT_TRUE(too::math::almost_equal_alltypes(1, 1)); 
    EXPECT_FALSE(too::math::almost_equal_alltypes(1, 2));
}
