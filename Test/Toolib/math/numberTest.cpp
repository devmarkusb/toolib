#include "gtest/gtest.h"
#include "Toolib/math/number.h"


TEST(MathNumberGetDecDigitCountTest, Dec)
{
    EXPECT_EQ(1, too::math::getDecDigitCount(0));
    EXPECT_EQ(1, too::math::getDecDigitCount(1));
    EXPECT_EQ(2, too::math::getDecDigitCount(12));
    EXPECT_EQ(3, too::math::getDecDigitCount(123));
}

TEST(MathNumberGetDecDigitCountTest, DecNeg)
{
    EXPECT_EQ(1, too::math::getDecDigitCount(-1));
    EXPECT_EQ(2, too::math::getDecDigitCount(-12));
}

TEST(MathNumberGetDecDigitCountTest, Bin)
{
    EXPECT_EQ(1, too::math::getDecDigitCount(0, too::math::ENumSys::BIN));
    EXPECT_EQ(2, too::math::getDecDigitCount(2, too::math::ENumSys::BIN));
    EXPECT_EQ(3, too::math::getDecDigitCount(4, too::math::ENumSys::BIN));
}

TEST(MathNumberGetDecDigitCountTest, Oct)
{
    EXPECT_EQ(1, too::math::getDecDigitCount(0, too::math::ENumSys::OCT));
    EXPECT_EQ(1, too::math::getDecDigitCount(7, too::math::ENumSys::OCT));
    EXPECT_EQ(2, too::math::getDecDigitCount(8, too::math::ENumSys::OCT));
}

TEST(MathNumberGetDecDigitCountTest, Hex)
{
    EXPECT_EQ(1, too::math::getDecDigitCount(0, too::math::ENumSys::HEX));
    EXPECT_EQ(1, too::math::getDecDigitCount(15, too::math::ENumSys::HEX));
    EXPECT_EQ(2, too::math::getDecDigitCount(16, too::math::ENumSys::HEX));
}

