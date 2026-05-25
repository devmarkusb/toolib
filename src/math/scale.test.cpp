#include "mb/toolib/math/scale.hpp"
#include "gtest/gtest.h"


namespace ul = mb::ul;

TEST(MathCalcNiceScaleTickTest, Basics) {
    EXPECT_DOUBLE_EQ(1.0, too::math::calc_nice_scale_tick<double>(10.0, 10));
    EXPECT_DOUBLE_EQ(2.0, too::math::calc_nice_scale_tick<double>(10.0, 5));
    EXPECT_DOUBLE_EQ(1.0, too::math::calc_nice_scale_tick<double>(49.1 - 39.9, 10));
    EXPECT_DOUBLE_EQ(2.0, too::math::calc_nice_scale_tick<double>(14.0, 10));
    EXPECT_DOUBLE_EQ(5.0, too::math::calc_nice_scale_tick<double>(50.0, 10));
    EXPECT_DOUBLE_EQ(10.0, too::math::calc_nice_scale_tick<double>(56.0, 10));
}

TEST(MathCalcNiceScaleTickTest, error) {
    EXPECT_THROW(too::math::calc_nice_scale_tick<double>(0.0, 10), ul::FailFast);
    EXPECT_THROW(too::math::calc_nice_scale_tick<int>(0, 10), ul::FailFast);
}

TEST(MathCalcScaleTickFromToTest, Basics) {
    double from = 0.0;
    double to = 0.0;
    std::tie(from, to) = too::math::calc_scale_tick_from_to<double>(39.9, 49.1, 1.0);
    EXPECT_DOUBLE_EQ(39.0, from);
    EXPECT_DOUBLE_EQ(50.0, to);
    std::tie(from, to) = too::math::calc_scale_tick_from_to<double>(1.0, 9.0, 1.0);
    EXPECT_DOUBLE_EQ(1.0, from);
    EXPECT_DOUBLE_EQ(9.0, to);
    std::tie(from, to) = too::math::calc_scale_tick_from_to<double>(1.0, 9.0, 0.5);
    EXPECT_DOUBLE_EQ(1.0, from);
    EXPECT_DOUBLE_EQ(9.0, to);
    std::tie(from, to) = too::math::calc_scale_tick_from_to<double>(1.0, 9.0, 2.0);
    EXPECT_GE(from, -1.0);
    EXPECT_LE(to, 11.0);
}
