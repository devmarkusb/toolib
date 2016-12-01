#include "Toolib/date_time/date_time.h"
#include <sstream>
#include <type_traits>
#include "gtest/gtest.h"

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

    ym = {2, 0};
    normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(1, 12), ym);
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
    s += MonthYear_decl::string_delim;
    s += "0";
    std::stringstream ss;
    ss << my;
    EXPECT_EQ(s, ss.str());

    my.set(13, 1);
    s = "13";
    s += MonthYear_decl::string_delim;
    s += "1";
    std::stringstream ss2;
    ss2 << my;
    EXPECT_EQ(s, ss2.str());

    tmp = "13";
    tmp += MonthYear_decl::string_delim + "1";
    my = tmp;
    s  = "13";
    s += MonthYear_decl::string_delim;
    s += "1";
    std::stringstream ss3;
    ss3 << my;
    EXPECT_EQ(s, ss3.str());

    EXPECT_EQ(13 + 12, my.asMonths());
}

TEST(date_time__MonthYear_baseTest, dont_normalize_ops_and_copy)
{
    MonthYear_base<dont_normalize> my(-1, 1);
    std::string tmp = "13";
    tmp += MonthYear_decl::string_delim + "0";
    MonthYear_base<dont_normalize> my_other(tmp);

    my += my_other;
    EXPECT_EQ(24, my.asMonths());

    my -= my_other;
    EXPECT_EQ(11, my.asMonths());

    MonthYear_base<dont_normalize> plus = my + my_other;
    EXPECT_EQ(24, plus.asMonths());

    MonthYear_base<dont_normalize> minus = my - my_other;
    EXPECT_EQ(-2, minus.asMonths());

    MonthYear_base<dont_normalize> my_copy(my);
    MonthYear_base<dont_normalize> my_copy2;
    my_copy2 = my;

    EXPECT_TRUE(my_copy == my);
    EXPECT_TRUE(my_copy <= my);
    EXPECT_TRUE(my_copy >= my);
    EXPECT_FALSE(my_copy2 != my);

    EXPECT_TRUE(my < my_other);
    EXPECT_TRUE(my <= my_other);
    EXPECT_TRUE(my_other > my);
    EXPECT_TRUE(my_other >= my);

    MonthYear_base<dont_normalize> my_move(MonthYear_base<dont_normalize>(-1, 1));
    MonthYear_base<dont_normalize> my_move2;
    my_move2 = MonthYear_base<dont_normalize>(13, 0);
    EXPECT_TRUE(my_move == my);
    EXPECT_TRUE(my_move2 == my_other);
}

TEST(date_time__MonthYear_baseTest, normalize)
{
    MonthYear_base<normalize> my;
    std::string s;
    std::string tmp;

    s = "0";
    s += MonthYear_decl::string_delim;
    s += "0";
    std::stringstream ss;
    ss << my;
    EXPECT_EQ(s, ss.str());

    my.set(13, 1);
    s = "1";
    s += MonthYear_decl::string_delim;
    s += "2";
    std::stringstream ss2;
    ss2 << my;
    EXPECT_EQ(s, ss2.str());

    tmp = "13";
    tmp += MonthYear_decl::string_delim + "1";
    my = tmp;
    s  = "1";
    s += MonthYear_decl::string_delim;
    s += "2";
    std::stringstream ss3;
    ss3 << my;
    EXPECT_EQ(s, ss3.str());

    EXPECT_EQ(13 + 12, my.asMonths());
}

TEST(date_time__MonthYear_baseTest, normalize_ops_and_copy)
{
    static_assert(std::is_same<MonthYear_base<normalize>, MonthYear>::value, "");
    MonthYear_base<normalize> my(-1, 1);
    std::string tmp = "13";
    tmp += MonthYear_decl::string_delim + "0";
    MonthYear my_other(tmp);

    my += my_other;
    EXPECT_EQ(24, my.asMonths());

    my -= my_other;
    EXPECT_EQ(11, my.asMonths());

    MonthYear plus = my + my_other;
    EXPECT_EQ(24, plus.asMonths());

    MonthYear minus = my - my_other;
    EXPECT_EQ(-2, minus.asMonths());

    MonthYear my_copy(my);
    MonthYear my_copy2;
    my_copy2 = my;

    EXPECT_TRUE(my_copy == my);
    EXPECT_TRUE(my_copy <= my);
    EXPECT_TRUE(my_copy >= my);
    EXPECT_FALSE(my_copy2 != my);

    EXPECT_TRUE(my < my_other);
    EXPECT_TRUE(my <= my_other);
    EXPECT_TRUE(my_other > my);
    EXPECT_TRUE(my_other >= my);

    MonthYear my_move(MonthYear(-1, 1));
    MonthYear my_move2;
    my_move2 = MonthYear(13, 0);
    EXPECT_TRUE(my_move == my);
    EXPECT_TRUE(my_move2 == my_other);
}
