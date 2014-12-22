#include "gtest/gtest.h"
#include "Toolib/performance.h"
#include <chrono>
#include <thread>
#include "Toolib/argsused.h"


TEST(ToFormattedStringTest, Rounding)
{
    EXPECT_EQ("99.99 ps", too::CPerformanceProfiler::ToFormattedString(0.000000000099985));
    EXPECT_EQ("99.98 ps", too::CPerformanceProfiler::ToFormattedString(0.000000000099984));
    EXPECT_EQ("99.99 ns", too::CPerformanceProfiler::ToFormattedString(0.000000099986));
    EXPECT_EQ("99.98 ns", too::CPerformanceProfiler::ToFormattedString(0.000000099983));
    EXPECT_EQ("99.99 µs", too::CPerformanceProfiler::ToFormattedString(0.000099987));
    EXPECT_EQ("99.98 µs", too::CPerformanceProfiler::ToFormattedString(0.000099982));
    EXPECT_EQ("99.99 ms", too::CPerformanceProfiler::ToFormattedString(0.099988));
    EXPECT_EQ("99.98 ms", too::CPerformanceProfiler::ToFormattedString(0.099981));
    EXPECT_EQ("59.99 s", too::CPerformanceProfiler::ToFormattedString(59.989));
    EXPECT_EQ("59.98 s", too::CPerformanceProfiler::ToFormattedString(59.980));
    EXPECT_EQ("59:59.99", too::CPerformanceProfiler::ToFormattedString(3599.985));
    EXPECT_EQ("59:59.98", too::CPerformanceProfiler::ToFormattedString(3599.984));
}

TEST(ToFormattedStringTest, UnitStepping)
{
    EXPECT_EQ("99.99 ps", too::CPerformanceProfiler::ToFormattedString(0.000000000099990));
    EXPECT_EQ("99.99 ns", too::CPerformanceProfiler::ToFormattedString(0.000000099990));
    EXPECT_EQ("99.99 µs", too::CPerformanceProfiler::ToFormattedString(0.000099990));
    EXPECT_EQ("99.99 ms", too::CPerformanceProfiler::ToFormattedString(0.099990));
    EXPECT_EQ("0.10 s", too::CPerformanceProfiler::ToFormattedString(0.1));
    EXPECT_EQ("1.00 s", too::CPerformanceProfiler::ToFormattedString(1.0));
    EXPECT_EQ("01:00.00", too::CPerformanceProfiler::ToFormattedString(60.0));
    EXPECT_EQ("01:00:00", too::CPerformanceProfiler::ToFormattedString(3600.0));
}

TEST(ToFormattedStringTest, RoundingCausingUnitStepping)
{
    EXPECT_EQ("0.10 s", too::CPerformanceProfiler::ToFormattedString(0.099999));
}

TEST(ToFormattedStringTest, NoRounding)
{
    EXPECT_EQ("99:59:58", too::CPerformanceProfiler::ToFormattedString(359998.5));
    EXPECT_EQ("99:59:58", too::CPerformanceProfiler::ToFormattedString(359998.4));
}

TEST(ToFormattedStringTest, Zero)
{
    EXPECT_EQ("0.00 ps", too::CPerformanceProfiler::ToFormattedString(0.0));
}

TEST(ToFormattedStringTest, Neg)
{
    EXPECT_EQ("-1.00 s", too::CPerformanceProfiler::ToFormattedString(-1.0));
}

TEST(ToFormattedStringTest, Inf)
{
    EXPECT_EQ(">= 100 h", too::CPerformanceProfiler::ToFormattedString(360000));
}

TEST(DISABLED_DumpAllItemsTest, PracticalScenario)
{
    too::CPerformanceProfiler perfscope0("1. 500ms");
    EXPECT_NEAR(0.0, perfscope0.Elapsed_CurrentItem(), 0.02);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    EXPECT_NEAR(0.5, perfscope0.Elapsed_CurrentItem(), 0.02);
    perfscope0.StartNewItem("2. nest0");
    {
        too::CPerformanceProfiler perfscope1("3. 100ms", 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        perfscope1.StartNewItem("4. nest1");
        {
            too::CPerformanceProfiler perfscope2("5. for", 2);
            for (size_t i = 1; i <= 5; ++i)
            {
                too::CPerformanceProfiler perfscope3("6. 500ms", 3);
                too::ignore_arg(perfscope3);
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
            perfscope2.StartNewItem("7. 200ms");
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
    perfscope0.StartNewItem("8. 300ms");
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    perfscope0.StartNewItem("9. 100ms");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    perfscope0.StopItem();
    too::CPerformanceProfiler::DumpAllItems(true);
    const std::vector<too::CPerformanceProfiler::SDumpDataset>& data = too::CPerformanceProfiler::DumpDataTest();
    ASSERT_EQ(9, data.size());
    EXPECT_EQ("1. 500ms", data[0].m_ItemName);
    EXPECT_EQ(1, data[0].m_Count);
    EXPECT_NEAR(0.5, data[0].m_Total, 0.02);
    EXPECT_NEAR(0.5, data[0].m_Average, 0.02);
    EXPECT_NEAR(0.5, data[0].m_Mean, 0.02);
    EXPECT_NEAR(0.0, data[0].m_StdDev, 0.0);

    EXPECT_EQ("2. nest0", data[1].m_ItemName);
    EXPECT_EQ(1, data[1].m_Count);
    EXPECT_NEAR(2.8, data[1].m_Total, 0.1);
    EXPECT_NEAR(2.8, data[1].m_Average, 0.1);
    EXPECT_NEAR(2.8, data[1].m_Mean, 0.1);
    EXPECT_NEAR(0.0, data[1].m_StdDev, 0.0);

    EXPECT_EQ(" 3. 100ms", data[2].m_ItemName);
    EXPECT_EQ(1, data[2].m_Count);
    EXPECT_NEAR(0.1, data[2].m_Total, 0.02);
    EXPECT_NEAR(0.1, data[2].m_Average, 0.02);
    EXPECT_NEAR(0.1, data[2].m_Mean, 0.02);
    EXPECT_NEAR(0.0, data[2].m_StdDev, 0.0);

    EXPECT_EQ(" 4. nest1", data[3].m_ItemName);
    EXPECT_EQ(1, data[3].m_Count);
    EXPECT_NEAR(2.7, data[3].m_Total, 0.1);
    EXPECT_NEAR(2.7, data[3].m_Average, 0.1);
    EXPECT_NEAR(2.7, data[3].m_Mean, 0.1);
    EXPECT_NEAR(0.0, data[3].m_StdDev, 0.0);

    EXPECT_EQ("  5. for", data[4].m_ItemName);
    EXPECT_EQ(1, data[4].m_Count);
    EXPECT_NEAR(2.5, data[4].m_Total, 0.1);
    EXPECT_NEAR(2.5, data[4].m_Average, 0.1);
    EXPECT_NEAR(2.5, data[4].m_Mean, 0.1);
    EXPECT_NEAR(0.0, data[4].m_StdDev, 0.0);

    EXPECT_EQ("   6. 500ms", data[5].m_ItemName);
    EXPECT_EQ(5, data[5].m_Count);
    EXPECT_NEAR(2.5, data[5].m_Total, 0.1);
    EXPECT_NEAR(0.5, data[5].m_Average, 0.02);
    EXPECT_NEAR(0.5, data[5].m_Mean, 0.02);
    EXPECT_NEAR(0.0, data[5].m_StdDev, 0.02);

    EXPECT_EQ("  7. 200ms", data[6].m_ItemName);
    EXPECT_EQ(1, data[6].m_Count);
    EXPECT_NEAR(0.2, data[6].m_Total, 0.02);
    EXPECT_NEAR(0.2, data[6].m_Average, 0.02);
    EXPECT_NEAR(0.2, data[6].m_Mean, 0.02);
    EXPECT_NEAR(0.0, data[6].m_StdDev, 0.0);

    EXPECT_EQ("8. 300ms", data[7].m_ItemName);
    EXPECT_EQ(1, data[7].m_Count);
    EXPECT_NEAR(0.3, data[7].m_Total, 0.02);
    EXPECT_NEAR(0.3, data[7].m_Average, 0.02);
    EXPECT_NEAR(0.3, data[7].m_Mean, 0.02);
    EXPECT_NEAR(0.0, data[7].m_StdDev, 0.0);

    EXPECT_EQ("9. 100ms", data[8].m_ItemName);
    EXPECT_EQ(1, data[8].m_Count);
    EXPECT_NEAR(0.1, data[8].m_Total, 0.02);
    EXPECT_NEAR(0.1, data[8].m_Average, 0.02);
    EXPECT_NEAR(0.1, data[8].m_Mean, 0.02);
    EXPECT_NEAR(0.0, data[8].m_StdDev, 0.0);

    too::CPerformanceProfiler::Reset();
    too::CPerformanceProfiler::DumpAllItems(true);
    EXPECT_EQ(0, data.size());
}
