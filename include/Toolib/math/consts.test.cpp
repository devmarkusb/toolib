#include "Toolib\math\consts.h"
#include "Toolib\math\floating_point.h"
#include "gtest/gtest.h"

#define _USE_MATH_DEFINES
#include <math.h>


TEST(math_constsTest, test)
{
    double pi = static_cast<double>(M_PI);
    EXPECT_DOUBLE_EQ(pi, too::math::consts::pi<double>());
    EXPECT_TRUE(too::math::almost_equal(pi, too::math::consts::pi<double>()));

    float pi_f = static_cast<float>(M_PI);
    EXPECT_DOUBLE_EQ(pi_f, too::math::consts::pi<float>());
    EXPECT_TRUE(too::math::almost_equal(pi_f, too::math::consts::pi<float>()));

    long double pi_ld = static_cast<long double>(M_PI);
    EXPECT_TRUE(too::math::almost_equal(pi_ld, too::math::consts::pi<long double>()));
}

#undef _USE_MATH_DEFINES
