#include "toolib/type/itype.h"
#include "gtest/gtest.h"

namespace too = mb::too;
namespace ul = mb::ul;

namespace {
// Now what about wanting to use some type that doesn't naturally inherit IFlexibleType.
struct SomeType {};

// or
using SomeOtherType = int;

// solution: some kind of wrapper/adapter
struct SomeType_flexed : public too::itype {
    SomeType_flexed() = default;
    ~SomeType_flexed() override = default;

    SomeType_flexed(const SomeType_flexed& other)
        : too::itype()
        , rep(other.rep) {
    }

    SomeType_flexed& operator=(const SomeType_flexed& other) {
        this->rep = other.rep;
        return *this;
    }

    SomeType_flexed(SomeType_flexed&&) = delete;
    SomeType_flexed& operator=(SomeType_flexed&&) = delete;

    explicit SomeType_flexed(const SomeType& x)
        : rep(x) {
    }

    explicit SomeType_flexed(SomeType&& x)
        : rep(std::move(x)) {
    }

    virtual ul::owner<SomeType_flexed*> clone() override {
        return new SomeType_flexed(*this);
    }

private:
    SomeType rep;
};

struct IUser {
    virtual ~IUser() = default;
    virtual void f(too::itype*) = 0;
};

struct UserA : public IUser {
    virtual ~UserA() = default;

    virtual void f(too::itype*) {
    }
};
} // namespace

TEST(ITypeTest, test) {
}
