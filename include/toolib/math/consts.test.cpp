#define _USE_MATH_DEFINES
#include <cmath> // needs to be the first include for the upper macro to work
#undef _USE_MATH_DEFINES
#include "toolib/almost_equal.h"
#include "toolib/math/consts.h"
#include "toolib/math/floating_point.h"
#include "gtest/gtest.h"

#ifndef M_PI // mingw53 doesn't have it
#define M_PI 3.141592653589793
#endif

TEST(math_constsTest, test)
{
    double pi = static_cast<double>(M_PI);
    EXPECT_DOUBLE_EQ(pi, too::math::consts::pi<double>());
    EXPECT_TRUE(too::almost_equal(pi, too::math::consts::pi<double>()));

    float pi_f = static_cast<float>(M_PI);
    EXPECT_DOUBLE_EQ(pi_f, too::math::consts::pi<float>());
    EXPECT_TRUE(too::almost_equal(pi_f, too::math::consts::pi<float>()));

    long double pi_ld = static_cast<long double>(M_PI);
    EXPECT_TRUE(too::math::approx_equal(pi_ld, too::math::consts::pi<long double>(), 1e-15L));
}

#undef M_PI // anyway
