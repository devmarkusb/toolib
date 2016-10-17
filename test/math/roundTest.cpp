#include "Toolib/math/round.h"
#include <cstdint>
#include "gtest/gtest.h"


TEST(MathRoundTest, Dbl)
{
    EXPECT_DOUBLE_EQ(1.0, too::math::round(0.5, 0));
    EXPECT_DOUBLE_EQ(0.0, too::math::round(0.4, 0));
    EXPECT_DOUBLE_EQ(0.5, too::math::round(0.54, 1));
    EXPECT_DOUBLE_EQ(0.6, too::math::round(0.55, 1));
    EXPECT_DOUBLE_EQ(0.54, too::math::round(0.54, 2));
    EXPECT_DOUBLE_EQ(0.54, too::math::round(0.54, 4));
}

TEST(MathRoundTest, DblNeg)
{
    EXPECT_DOUBLE_EQ(-1.0, too::math::round(-0.5, 0));
    EXPECT_DOUBLE_EQ(0.0, too::math::round(-0.4, 0));
    EXPECT_DOUBLE_EQ(-0.5, too::math::round(-0.54, 1));
    EXPECT_DOUBLE_EQ(-0.6, too::math::round(-0.55, 1));
}

TEST(MathRoundToTest, ToIntegralType)
{
    EXPECT_DOUBLE_EQ(1, too::math::round_to<short>(0.5));
    EXPECT_DOUBLE_EQ(0, too::math::round_to<short>(0.4));
}

TEST(MathRoundToTest, ToIntegralTypeInf)
{
    EXPECT_DOUBLE_EQ(127, too::math::round_to<int8_t>(127.9));
    EXPECT_DOUBLE_EQ(-128, too::math::round_to<int8_t>(-128.9));
}

TEST(MathRoundToTest, ToIntegralTypeIgnoreDecimalPlaces)
{
    EXPECT_DOUBLE_EQ(3, too::math::round_to<int8_t>(2.9, 1));
    EXPECT_DOUBLE_EQ(2, too::math::round_to<int8_t>(2.1, 1));
}

TEST(MathRoundToTest, ToFloat)
{
    EXPECT_DOUBLE_EQ(2.9f, too::math::round_to<float>(2.9, 1));
    EXPECT_DOUBLE_EQ(2.0f, too::math::round_to<float>(2.1, 0));
}
