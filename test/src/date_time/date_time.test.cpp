#include "gtest/gtest.h"
#include <sstream>
#include "Toolib/date_time/date_time.h"

using namespace too::date_time;

TEST(date_time__normalize_do_itTest, already_done)
{
    std::pair<Years, Months> ym;

    ym = {2016, 6};
    normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(2016, 6), ym);

    ym = {0, 0};
    normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, 0), ym);

    ym = {0, 1};
    normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, 1), ym);

    ym = {1, 1};
    normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(1, 1), ym);

    ym = {0, -1};
    normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, -1), ym);

    ym = {-1, -1};
    normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(-1, -1), ym);

    ym = {0, 12};
    normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, 12), ym);

    ym = {0, -12};
    normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, -12), ym);
}

TEST(date_time__normalize_do_itTest, month_0_year_not)
{
    std::pair<Years, Months> ym;

    ym = {-1, 0};
    normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, -12), ym);

    ym = {1, 0};
    normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, 12), ym);
}

TEST(date_time__normalize_do_itTest, equal_signs)
{
    std::pair<Years, Months> ym;

    ym = {1, 13};
    normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(2, 1), ym);

    ym = {0, 24};
    normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(1, 12), ym);

    ym = {0, 13};
    normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(1, 1), ym);

    ym = {0, 14};
    normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(1, 2), ym);

    ym = {0, 25};
    normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(2, 1), ym);

    ym = {-1, -13};
    normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(-2, -1), ym);

    ym = {0, -24};
    normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(-1, -12), ym);
}

TEST(date_time__normalize_do_itTest, different_signs)
{
    std::pair<Years, Months> ym;

    ym = {-1, 1};
    normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, -11), ym);

    ym = {-1, 11};
    normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, -1), ym);

    ym = {-1, 12};
    normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, 0), ym);

    ym = {-1, 24};
    normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, 12), ym);

    ym = {-2, 24};
    normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, 0), ym);

    ym = {-1, 13};
    normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, 1), ym);

    ym = {1, -11};
    normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, 1), ym);

    ym = {1, -12};
    normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, 0), ym);

    ym = {2, -24};
    normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, 0), ym);

    ym = {1, -13};
    normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, -1), ym);

    ym = {1, -24};
    normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, -12), ym);
}

TEST(date_time__MonthYear_baseTest, dont_normalize)
{
    MonthYear_base<dont_normalize> my;
    std::string s;
    std::string tmp;

    s = "0";
    s+= MonthYear_decl::string_delim;
    s+= "0";
    std::stringstream ss;
    ss << my;
    EXPECT_EQ(s, ss.str());

    my.set(13, 1);
    s = "13";
    s+= MonthYear_decl::string_delim;
    s+= "1";
    std::stringstream ss2;
    ss2 << my;
    EXPECT_EQ(s, ss2.str());

    tmp = "13";
    tmp+= MonthYear_decl::string_delim + "1";
    my = tmp;
    s = "13";
    s+= MonthYear_decl::string_delim;
    s+= "1";
    std::stringstream ss3;
    ss3 << my;
    EXPECT_EQ(s, ss3.str());

    EXPECT_EQ(13 + 12, my.asMonths());
}

TEST(date_time__MonthYear_baseTest, dont_normalize_ops)
{
    MonthYear_base<dont_normalize> my(-1, 1);
    std::string tmp = "13";
    tmp+= MonthYear_decl::string_delim + "0";
    MonthYear_base<dont_normalize> my_other(tmp);

    my+= my_other;
    EXPECT_EQ(24, my.asMonths());

    my-= my_other;
    EXPECT_EQ(11, my.asMonths());

    //todo
//    MonthYear_base<dont_normalize>  plus = my + my_other;
//    EXPECT_EQ(24, plus.asMonths());
}
