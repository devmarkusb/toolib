#include "Toolib/date_time/cal_event.h"
#include "gtest/gtest.h"

using namespace too::date_time;


namespace
{
}

struct CalEventTest : public ::testing::Test
{
    CalEventTest() {}

    void SetUp()
    {
        int start = 4, end = 9;
        recurr.setStart(&start);
        recurr.setEnd(&end);
    }

    void TearDown() {}

    ~CalEventTest() {}

    SingleEvent<int> seven{7};
    RecurringEvent<int> recurr{2};
};


TEST_F(CalEventTest, test)
{
    CalEvent<int>* ce = &seven;
    std::unique_ptr<CalEvent<int>> ce_clone(ce->clone());

    std::unique_ptr<int> tp = ce_clone->getFirstTimePoint();
    EXPECT_EQ(7, *tp);
    tp = ce_clone->getNextTimePoint(*tp);
    EXPECT_FALSE(tp);

    std::unique_ptr<CalEvent<int>> ces_clone(recurr.clone());
    std::unique_ptr<int> tps = ces_clone->getFirstTimePoint();
    EXPECT_EQ(4, *tps);
    int val = 6;
    while (tps = ces_clone->getNextTimePoint(*tps))
    {
        EXPECT_EQ(val, *tps);
        val += 2;
    }
    EXPECT_EQ(10, val);
}

TEST_F(CalEventTest, zero_start)
{
    recurr.setStart(nullptr);

    std::unique_ptr<int> tps = recurr.getFirstTimePoint();
    EXPECT_EQ(0, *tps);
    int val = 2;
    while ((tps = recurr.getNextTimePoint(*tps)) && val <= 6)
    {
        EXPECT_EQ(val, *tps);
        val += 2;
    }
    auto laterTime = recurr.getNextTimePoint(1234566);
    EXPECT_FALSE(laterTime);
}
