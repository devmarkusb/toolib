#include "gtest/gtest.h"
#include "Toolib/math/floating_point.h"


TEST(Math_almost_equal_Test, Basics) { EXPECT_TRUE(too::math::almost_equal(1.0, 1.0)); }
