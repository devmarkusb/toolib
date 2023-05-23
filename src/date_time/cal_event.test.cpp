#include "toolib/date_time/cal_event.h"
#include "gtest/gtest.h"

using namespace mb::too::date_time;

namespace {}

struct CalEventTest : public ::testing::Test {
    CalEventTest() = default;

    void SetUp() override {
        int start = 4;
        int end = 9;
        recurr.set_start(&start);
        recurr.set_end(&end);
    }

    void TearDown() override {
    }

    ~CalEventTest() override = default;

    SingleEvent<int> seven{7};
    RecurringEvent<int> recurr{2};
};

TEST_F(CalEventTest, test) {
    CalEvent<int>* ce = &seven;
    std::unique_ptr<CalEvent<int>> ce_clone(ce->clone());

    std::unique_ptr<int> tp = ce_clone->get_first_time_point();
    EXPECT_EQ(7, *tp);
    tp = ce_clone->get_next_time_point(*tp);
    EXPECT_FALSE(tp);

    std::unique_ptr<CalEvent<int>> ces_clone(recurr.clone());
    std::unique_ptr<int> tps = ces_clone->get_first_time_point();
    EXPECT_EQ(4, *tps);
    int val = 6;
    while ((tps = ces_clone->get_next_time_point(*tps))) {
        EXPECT_EQ(val, *tps);
        val += 2;
    }
    EXPECT_EQ(10, val);
}

TEST_F(CalEventTest, zero_start) {
    recurr.set_start(nullptr);

    std::unique_ptr<int> tps = recurr.get_first_time_point();
    EXPECT_EQ(0, *tps);
    int val = 2;
    while ((tps = recurr.get_next_time_point(*tps)) && val <= 6) {
        EXPECT_EQ(val, *tps);
        val += 2;
    }
    auto later_time = recurr.get_next_time_point(1234566);
    EXPECT_FALSE(later_time);
}
