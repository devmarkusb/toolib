#include "gtest/gtest.h"
#include "Toolib/assert.h"
#include <exception>


TEST(AssertThrowTest, Test)
{
#ifdef NDEBUG
    EXPECT_NO_THROW(too::Assert_throw<std::exception>(1 == 2), std::exception);
#else
    EXPECT_THROW(too::Assert_throw<std::exception>(1 == 2), std::exception);
    EXPECT_NO_THROW(too::Assert_throw<std::exception>(1 == 1));
#endif
}
