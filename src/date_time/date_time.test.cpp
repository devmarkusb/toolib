#include "toolib/date_time/date_time.h"
#include "gtest/gtest.h"
#include <sstream>
#include <type_traits>

using namespace mb::too::date_time;

TEST(date_time__normalize_do_itTest, already_done) {
    std::pair<Years, Months> ym;

    ym = {2016, 6};
    Normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(2016, 6), ym);

    ym = {0, 0};
    Normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, 0), ym);

    ym = {0, 1};
    Normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, 1), ym);

    ym = {1, 1};
    Normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(1, 1), ym);

    ym = {0, -1};
    Normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, -1), ym);

    ym = {-1, -1};
    Normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(-1, -1), ym);

    ym = {0, 12};
    Normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, 12), ym);

    ym = {0, -12};
    Normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, -12), ym);
}

TEST(date_time__normalize_do_itTest, month_0_year_not) {
    std::pair<Years, Months> ym;

    ym = {-1, 0};
    Normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, -12), ym);

    ym = {1, 0};
    Normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, 12), ym);

    ym = {2, 0};
    Normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(1, 12), ym);
}

TEST(date_time__normalize_do_itTest, equal_signs) {
    std::pair<Years, Months> ym;

    ym = {1, 13};
    Normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(2, 1), ym);

    ym = {0, 24};
    Normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(1, 12), ym);

    ym = {0, 13};
    Normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(1, 1), ym);

    ym = {0, 14};
    Normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(1, 2), ym);

    ym = {0, 25};
    Normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(2, 1), ym);

    ym = {-1, -13};
    Normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(-2, -1), ym);

    ym = {0, -24};
    Normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(-1, -12), ym);
}

TEST(date_time__normalize_do_itTest, different_signs) {
    std::pair<Years, Months> ym;

    ym = {-1, 1};
    Normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, -11), ym);

    ym = {-1, 11};
    Normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, -1), ym);

    ym = {-1, 12};
    Normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, 0), ym);

    ym = {-1, 24};
    Normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, 12), ym);

    ym = {-2, 24};
    Normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, 0), ym);

    ym = {-1, 13};
    Normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, 1), ym);

    ym = {1, -11};
    Normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, 1), ym);

    ym = {1, -12};
    Normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, 0), ym);

    ym = {2, -24};
    Normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, 0), ym);

    ym = {1, -13};
    Normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, -1), ym);

    ym = {1, -24};
    Normalize::do_it(ym);
    EXPECT_EQ(std::make_pair(0, -12), ym);
}

TEST(date_time__MonthYearBaseTest, dont_normalize) {
    MonthYearBase<DontNormalize> my;
    std::string s;
    std::string tmp;

    s = "0";
    s += MonthYearDecl::string_delim;
    s += "0";
    std::stringstream ss;
    ss << my;
    EXPECT_EQ(s, ss.str());

    my.set(13, 1);
    s = "13";
    s += MonthYearDecl::string_delim;
    s += "1";
    std::stringstream ss2;
    ss2 << my;
    EXPECT_EQ(s, ss2.str());

    tmp = "13";
    tmp += MonthYearDecl::string_delim + "1";
    my = tmp;
    s = "13";
    s += MonthYearDecl::string_delim;
    s += "1";
    std::stringstream ss3;
    ss3 << my;
    EXPECT_EQ(s, ss3.str());

    EXPECT_EQ(13 + 12, my.as_months());
}

TEST(date_time__MonthYearBaseTest, dont_normalize_ops_and_copy) {
    MonthYearBase<DontNormalize> my(-1, 1);
    std::string tmp = "13";
    tmp += MonthYearDecl::string_delim + "0";
    const MonthYearBase<DontNormalize> my_other(tmp);

    my += my_other;
    EXPECT_EQ(24, my.as_months());

    my -= my_other;
    EXPECT_EQ(11, my.as_months());

    const MonthYearBase<DontNormalize> plus = my + my_other;
    EXPECT_EQ(24, plus.as_months());

    const MonthYearBase<DontNormalize> minus = my - my_other;
    EXPECT_EQ(-2, minus.as_months());

    const MonthYearBase<DontNormalize> my_copy(my);
    MonthYearBase<DontNormalize> my_copy2;
    my_copy2 = my;

    EXPECT_TRUE(my_copy == my);
    EXPECT_TRUE(my_copy <= my);
    EXPECT_TRUE(my_copy >= my);
    EXPECT_FALSE(my_copy2 != my);

    EXPECT_TRUE(my < my_other);
    EXPECT_TRUE(my <= my_other);
    EXPECT_TRUE(my_other > my);
    EXPECT_TRUE(my_other >= my);

    const MonthYearBase<DontNormalize> my_move(MonthYearBase<DontNormalize>(-1, 1));
    MonthYearBase<DontNormalize> my_move2;
    my_move2 = MonthYearBase<DontNormalize>(13, 0);
    EXPECT_TRUE(my_move == my);
    EXPECT_TRUE(my_move2 == my_other);
}

TEST(date_time__MonthYearBaseTest, normalize) {
    MonthYearBase<Normalize> my;
    std::string s;
    std::string tmp;

    s = "0";
    s += MonthYearDecl::string_delim;
    s += "0";
    std::stringstream ss;
    ss << my;
    EXPECT_EQ(s, ss.str());

    my.set(13, 1);
    s = "1";
    s += MonthYearDecl::string_delim;
    s += "2";
    std::stringstream ss2;
    ss2 << my;
    EXPECT_EQ(s, ss2.str());

    tmp = "13";
    tmp += MonthYearDecl::string_delim + "1";
    my = tmp;
    s = "1";
    s += MonthYearDecl::string_delim;
    s += "2";
    std::stringstream ss3;
    ss3 << my;
    EXPECT_EQ(s, ss3.str());

    EXPECT_EQ(13 + 12, my.as_months());
}

TEST(date_time__MonthYearBaseTest, normalize_ops_and_copy) {
    static_assert(std::is_same<MonthYearBase<Normalize>, MonthYear>::value);
    MonthYearBase<Normalize> my(-1, 1);
    std::string tmp = "13";
    tmp += MonthYearDecl::string_delim + "0";
    const MonthYear my_other(tmp);

    my += my_other;
    EXPECT_EQ(24, my.as_months());

    my -= my_other;
    EXPECT_EQ(11, my.as_months());

    const MonthYear plus = my + my_other;
    EXPECT_EQ(24, plus.as_months());

    const MonthYear minus = my - my_other;
    EXPECT_EQ(-2, minus.as_months());

    const MonthYear my_copy(my);
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

    const MonthYear my_move(MonthYear(-1, 1));
    MonthYear my_move2;
    my_move2 = MonthYear(13, 0);
    EXPECT_TRUE(my_move == my);
    EXPECT_TRUE(my_move2 == my_other);
}
