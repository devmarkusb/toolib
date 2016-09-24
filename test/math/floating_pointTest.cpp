#include "Toolib/math/floating_point.h"
#include "gtest/gtest.h"


TEST(Math_almost_equal_Test, Basics) { EXPECT_TRUE(too::math::almost_equal(1.0, 1.0)); }
