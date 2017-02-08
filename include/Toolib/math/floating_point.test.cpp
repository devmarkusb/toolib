#include "Toolib/math/floating_point.h"
#include "gtest/gtest.h"


TEST(math_almost_equal_Test, test)
{
    EXPECT_TRUE(too::math::almost_equal(1.0, 1.0));
    EXPECT_FALSE(too::math::almost_equal(1.0, 1.0001));
}

TEST(math_almost_equal_alltypes_Test, test)
{
    EXPECT_TRUE(too::math::almost_equal_alltypes(1, 1));
    EXPECT_FALSE(too::math::almost_equal_alltypes(1, 2));
}

TEST(math_to_string_Test, test)
{
    EXPECT_STREQ("4.556000", too::math::to_string(4.556).c_str());
    EXPECT_STREQ("4.6", too::math::to_string(4.556, 2).c_str());

    EXPECT_STREQ("4.556000", too::math::to_string<too::math::FloatFormat::default_>(4.556).c_str());
    EXPECT_STREQ("4.6", too::math::to_string<too::math::FloatFormat::default_>(4.556, 2).c_str());

    EXPECT_STREQ("4.556000", too::math::to_string<too::math::FloatFormat::fixed>(4.556).c_str());
    EXPECT_STREQ("4.56", too::math::to_string<too::math::FloatFormat::fixed>(4.556, 2).c_str());

    const auto s1 = too::math::to_string<too::math::FloatFormat::scientific>(4.556);
    EXPECT_EQ(0u, s1.find("4.556000e+00"));
    const auto s2 = too::math::to_string<too::math::FloatFormat::scientific>(4.556, 2);
    EXPECT_EQ(0u, s2.find("4.56e+00"));
}
