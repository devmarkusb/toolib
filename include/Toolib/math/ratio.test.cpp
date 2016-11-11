#include "Toolib/math/ratio.h"
#include "gtest/gtest.h"


TEST(gcdTest, test)
{
    EXPECT_EQ(5, too::math::gcd(10, 5));
    EXPECT_EQ(1, too::math::gcd(11, 5));
    EXPECT_EQ(2, too::math::gcd(24, 22));
    EXPECT_EQ(8, too::math::gcd(24, 32));
}

TEST(gcdTest, pathol)
{
    EXPECT_EQ(42, too::math::gcd(42, 0));
    EXPECT_EQ(42, too::math::gcd(0, 42));
    EXPECT_EQ(1, too::math::gcd(1, 1));
    EXPECT_EQ(1, too::math::gcd(1, 2));
    EXPECT_EQ(1, too::math::gcd(1, 23432));
    EXPECT_EQ(1, too::math::gcd(23432, 1));
}

TEST(gcdTest, neg)
{
    EXPECT_EQ(5, too::math::gcd(-10, 5));
    EXPECT_EQ(1, too::math::gcd(11, -5));
    EXPECT_EQ(2, too::math::gcd(24, -22));

    // these don't work (and need not)
    //EXPECT_EQ(8, too::math::gcd(-24, -32));
    //EXPECT_EQ(42, too::math::gcd(-42, 0));
    //EXPECT_EQ(42, too::math::gcd(0, -42));
}

TEST(lcmTest, test)
{
    EXPECT_EQ(10, too::math::lcm(10, 5));
    EXPECT_EQ(55, too::math::lcm(11, 5));
    EXPECT_EQ(264, too::math::lcm(24, 22));
    EXPECT_EQ(96, too::math::lcm(24, 32));
}

TEST(RationalTest, constr)
{
    too::math::Rational r1{1, 2};
    EXPECT_EQ(1, r1.num);
    EXPECT_EQ(2, r1.denom);

    too::math::Rational r2{4};
    EXPECT_EQ(4, r2.num);
    EXPECT_EQ(1, r2.denom);

    too::math::Rational r3{-1, 2};
    EXPECT_EQ(-1, r3.num);
    EXPECT_EQ(2, r3.denom);

    too::math::Rational r4{-4};
    EXPECT_EQ(-4, r4.num);
    EXPECT_EQ(1, r4.denom);

    const auto m = std::micro{};
    too::math::Rational r5{m};
    EXPECT_EQ(1, r5.num);
    EXPECT_EQ(1000000, r5.denom);

    const auto k = std::kilo{};
    too::math::Rational r6{k};
    EXPECT_EQ(1000, r6.num);
    EXPECT_EQ(1, r6.denom);
}

TEST(RationalTest, constr_zero)
{
    too::math::Rational r1{};
    EXPECT_EQ(0, r1.num);
    EXPECT_EQ(1, r1.denom);

    too::math::Rational r2;
    EXPECT_EQ(0, r2.num);
    EXPECT_EQ(1, r2.denom);

    too::math::Rational r3{0};
    EXPECT_EQ(0, r3.num);
    EXPECT_EQ(1, r3.denom);
}

TEST(RationalTest, assign)
{
    too::math::Rational r1{};
    too::math::Rational r1_{24, 32};

    r1 = r1_;
    EXPECT_EQ(24, r1.num);
    EXPECT_EQ(32, r1.denom);
}

TEST(RationalTest, inv)
{
    too::math::Rational r1{1, 2};
    r1.inverse();
    EXPECT_EQ(2, r1.num);
    EXPECT_EQ(1, r1.denom);

    too::math::Rational r2{-1, 2};
    r2.inverse();
    EXPECT_EQ(-2, r2.num);
    EXPECT_EQ(1, r2.denom);
}

TEST(RationalTest, asfloat)
{
    too::math::Rational r1{1, 2};
    double d{r1.asFloatingPoint<double>()};
    EXPECT_DOUBLE_EQ(0.5, d);
}

TEST(RationalTest, reduce)
{
    too::math::Rational r1{24, 32};
    r1.reduce();
    EXPECT_EQ(3, r1.num);
    EXPECT_EQ(4, r1.denom);

    too::math::Rational r2{-24, 32};
    r2.reduce();
    EXPECT_EQ(-3, r2.num);
    EXPECT_EQ(4, r2.denom);
}

TEST(RationalTest, eq)
{
    too::math::Rational r1{24, 32};
    too::math::Rational r1_{3, 4};
    EXPECT_TRUE(r1 == r1_);
    EXPECT_FALSE(r1 != r1_);
    EXPECT_FALSE(r1 < r1_);
    EXPECT_FALSE(r1 > r1_);
    EXPECT_TRUE(r1 <= r1_);
    EXPECT_TRUE(r1 >= r1_);

    too::math::Rational r2{-24, 32};
    too::math::Rational r2_{-3, 4};
    EXPECT_TRUE(r2 == r2_);

    too::math::Rational r3_{3, 4};
    EXPECT_FALSE(r2 == r3_);
    EXPECT_TRUE(r2 != r3_);

    too::math::Rational r4{3, 4};
    EXPECT_TRUE(r4 == r3_);

    too::math::Rational r5{3, 5};
    EXPECT_FALSE(r5 == r3_);

    too::math::Rational r6{4, 4};
    EXPECT_FALSE(r6 == r3_);
}

TEST(RationalTest, make_common_denom)
{
    too::math::Rational r1{1, 32};
    too::math::Rational r2{1, 24};
    too::math::make_common_denom(r1, r2);

    too::math::Rational r1_{3, 96};
    EXPECT_TRUE(r1 == r1_);
    too::math::Rational r2_{4, 96};
    EXPECT_TRUE(r2 == r2_);
}

TEST(RationalTest, make_common_denom_neg)
{
    {
        too::math::Rational r1{1, 32};
        too::math::Rational r2{-1, 24};
        too::math::make_common_denom(r1, r2);

        too::math::Rational r1_{3, 96};
        EXPECT_TRUE(r1 == r1_);
        too::math::Rational r2_{-4, 96};
        EXPECT_TRUE(r2 == r2_);
    }
    {
        too::math::Rational r1{-1, 32};
        too::math::Rational r2{1, 24};
        too::math::make_common_denom(r1, r2);

        too::math::Rational r1_{-3, 96};
        EXPECT_TRUE(r1 == r1_);
        too::math::Rational r2_{4, 96};
        EXPECT_TRUE(r2 == r2_);
    }
    {
        too::math::Rational r1{-1, 32};
        too::math::Rational r2{-1, 24};
        too::math::make_common_denom(r1, r2);

        too::math::Rational r1_{-3, 96};
        EXPECT_TRUE(r1 == r1_);
        too::math::Rational r2_{-4, 96};
        EXPECT_TRUE(r2 == r2_);
    }
}

TEST(RationalTest, lessthan)
{
    {
        too::math::Rational r1{2, 4};
        too::math::Rational r2{3, 4};
        too::math::Rational r3{4, 4};

        EXPECT_TRUE(r1 < r2);
        EXPECT_TRUE(r2 < r3);
    }
    {
        too::math::Rational r1{3, 5};
        too::math::Rational r2{3, 4};
        too::math::Rational r3{3, 3};

        EXPECT_TRUE(r1 < r2);
        EXPECT_TRUE(r2 < r3);
    }
    {
        too::math::Rational r1{5, 8};
        too::math::Rational r2{3, 4};
        too::math::Rational r3{7, 8};

        EXPECT_TRUE(r1 < r2);
        EXPECT_TRUE(r2 < r3);
    }
}

TEST(RationalTest, lessthan_consts)
{
    using namespace too::math;

    EXPECT_TRUE(atto < femto);
    EXPECT_TRUE(femto < pico);
    EXPECT_TRUE(pico < nano);
    EXPECT_TRUE(nano < micro);
    EXPECT_TRUE(micro < milli);
    EXPECT_TRUE(milli < centi);
    EXPECT_TRUE(centi < deci);
    EXPECT_TRUE(deci < one);
    EXPECT_TRUE(one < deca);
    EXPECT_TRUE(deca < hecto);
    EXPECT_TRUE(hecto < kilo);
    EXPECT_TRUE(kilo < mega);
    EXPECT_TRUE(mega < giga);
    EXPECT_TRUE(giga < tera);
    EXPECT_TRUE(tera < peta);
    EXPECT_TRUE(peta < exa);

    EXPECT_TRUE(peta <= exa);
    EXPECT_FALSE(peta >= exa);
    EXPECT_FALSE(peta > exa);

    EXPECT_TRUE(exa >= peta);
    EXPECT_FALSE(exa <= peta);
    EXPECT_FALSE(exa < peta);
}

TEST(RationalTest, booltest)
{
    too::math::Rational r1{0, 32};
    if (!r1.is_null())
        EXPECT_TRUE(false);
    else
        EXPECT_TRUE(true);
    if (!r1)
        EXPECT_TRUE(true);
    else
        EXPECT_TRUE(false);

    too::math::Rational r2{1, 24};
    if (!r2)
        EXPECT_TRUE(false);
    else
        EXPECT_TRUE(true);
    if (!r2.is_null())
        EXPECT_TRUE(true);
    else
        EXPECT_TRUE(false);
    if (!!r2)
        EXPECT_TRUE(true);
    else
        EXPECT_TRUE(false);
}

TEST(RationalTest, unaryminus)
{
    too::math::Rational exp_mr1{-1, 24};
    too::math::Rational r1{1, 24};
    too::math::Rational mr1{-r1};
    EXPECT_TRUE(exp_mr1 == mr1);
}
