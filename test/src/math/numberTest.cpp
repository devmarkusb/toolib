#include "gtest/gtest.h"
#include "Toolib/math/number.h"


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
