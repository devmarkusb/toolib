#include "gtest/gtest.h"
#include "Toolib/class/non_copyable.h"

namespace
{

class A : private too::non_copyable
{
};

class B : public A
{
};
}

TEST(non_copyableTest, test)
{
    // also B is non-copyable!
    //    A a;
    //    A acopy(a);
    //    B b;
    //    B bcopy(b);
}
