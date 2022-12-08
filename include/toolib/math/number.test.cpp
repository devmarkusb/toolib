#include "toolib/math/number.h"
#include "gtest/gtest.h"

namespace too = mb::too;

TEST(MathNumberGetDecDigitCountTest, Dec)
{
    EXPECT_EQ(1, too::math::getDigitCount(0));
    EXPECT_EQ(1, too::math::getDigitCount(1));
    EXPECT_EQ(2, too::math::getDigitCount(12));
    EXPECT_EQ(3, too::math::getDigitCount(123));
}

TEST(MathNumberGetDecDigitCountTest, DecNeg)
{
    EXPECT_EQ(1, too::math::getDigitCount(-1));
    EXPECT_EQ(2, too::math::getDigitCount(-12));
}

TEST(MathNumberGetDecDigitCountTest, Bin)
{
    EXPECT_EQ(1, too::math::getDigitCount(0, too::math::ENumSys::BIN));
    EXPECT_EQ(2, too::math::getDigitCount(2, too::math::ENumSys::BIN));
    EXPECT_EQ(3, too::math::getDigitCount(4, too::math::ENumSys::BIN));
}

TEST(MathNumberGetDecDigitCountTest, Oct)
{
    EXPECT_EQ(1, too::math::getDigitCount(0, too::math::ENumSys::OCT));
    EXPECT_EQ(1, too::math::getDigitCount(7, too::math::ENumSys::OCT));
    EXPECT_EQ(2, too::math::getDigitCount(8, too::math::ENumSys::OCT));
}

TEST(MathNumberGetDecDigitCountTest, Hex)
{
    EXPECT_EQ(1, too::math::getDigitCount(0, too::math::ENumSys::HEX));
    EXPECT_EQ(1, too::math::getDigitCount(15, too::math::ENumSys::HEX));
    EXPECT_EQ(2, too::math::getDigitCount(16, too::math::ENumSys::HEX));
}

TEST(math_sgnTest, Hex)
{
    EXPECT_EQ(0, too::math::sgn(0));
    EXPECT_EQ(+1, too::math::sgn(1));
    EXPECT_EQ(-1, too::math::sgn(-1));
    EXPECT_EQ(+1, too::math::sgn(5));
    EXPECT_EQ(-1, too::math::sgn(-5));

    EXPECT_EQ(0, too::math::sgn(0.0));
    EXPECT_EQ(+1, too::math::sgn(1.0));
    EXPECT_EQ(-1, too::math::sgn(-1.0));

    EXPECT_EQ(0, too::math::sgn(0u));
    EXPECT_EQ(+1, too::math::sgn(1u));

    EXPECT_EQ(-1, too::math::sgn(-1L));
}

TEST(math_is_power_ofTest, simple)
{
    EXPECT_EQ(2, *too::math::is_power_of(100, 10));
    if (too::math::is_power_of(10, 10))
        EXPECT_TRUE(true);
    else
        EXPECT_TRUE(false);
    EXPECT_FALSE(too::math::is_power_of(101, 10));
}

TEST(math_is_power_ofTest, floatingpoint)
{
    EXPECT_DOUBLE_EQ(2.0, *too::math::is_power_of(100.0, 10.0));
    EXPECT_DOUBLE_EQ(-5.0, *too::math::is_power_of(0.00001, 10.0));
    EXPECT_FALSE(too::math::is_power_of(100.1, 10.0));
}

TEST(math_is_power_ofTest, corners)
{
    EXPECT_DOUBLE_EQ(0, *too::math::is_power_of(1, 2));

    EXPECT_DOUBLE_EQ(0.0, *too::math::is_power_of(1.0, 0.5));
    EXPECT_DOUBLE_EQ(0.0, *too::math::is_power_of(1.0, 2.0));

    EXPECT_DOUBLE_EQ(-1.0, *too::math::is_power_of(2.0, 0.5));
    EXPECT_DOUBLE_EQ(1.0, *too::math::is_power_of(2.0, 2.0));
    EXPECT_FALSE(too::math::is_power_of(2.0, 4.0));

    EXPECT_DOUBLE_EQ(-1.0, *too::math::is_power_of(0.25, 4.0));
}
