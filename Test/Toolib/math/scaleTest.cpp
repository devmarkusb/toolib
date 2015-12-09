#include "gtest/gtest.h"
#include "Toolib/math/scale.h"


TEST(MathCalcNiceScaleTickTest, Basics)
{
    EXPECT_DOUBLE_EQ(1.0, *too::math::calcNiceScaleTick<double>(10.0, 10));
}
