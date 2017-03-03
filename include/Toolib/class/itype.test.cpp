#include "Toolib/class/IType.h"
#include "gtest/gtest.h"


namespace
{
// Now what about wanting to use some type that doesn't naturally inherit IFlexibleType.
struct SomeType
{
};
// or
using SomeOtherType = int;

// solution: some kind of wrapper/adapter
struct SomeType_flexed : public too::IType
{
    SomeType_flexed() = default;
    virtual ~SomeType_flexed() = default;
    SomeType_flexed(const SomeType_flexed& other) : too::IType(), rep(other.rep) {}
    SomeType_flexed& operator=(const SomeType_flexed& other)
    {
        this->rep = other.rep;
        return *this;
    }
#if TOO_HAS_CPP11_DEFAULT_MOVES
    SomeType_flexed(SomeType_flexed&&) = default;
    SomeType_flexed& operator=(SomeType_flexed&&) = default;
#endif
    SomeType_flexed(const SomeType& x) : rep(x) {}
    SomeType_flexed(SomeType&& x) : rep(std::move(x)) {}

    virtual too::owner<SomeType_flexed*> clone() override { return new SomeType_flexed(*this); }

private:
    SomeType rep;
};

struct IUser
{
    virtual void f(too::IType*) = 0;
};

struct UserA : public IUser
{
    virtual void f(too::IType*) {}
};
}

TEST(ITypeTest, test) {}
