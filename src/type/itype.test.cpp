#include "mb/toolib/type/itype.hpp"
#include "gtest/gtest.h"


namespace ul = mb::ul;

namespace {
// Now what about wanting to use some type that doesn't naturally inherit IFlexibleType.
struct SomeType {};

// or
using SomeOtherType = int;

// solution: some kind of wrapper/adapter
struct SomeTypeFlexed : public too::Itype {
    SomeTypeFlexed() = default;
    ~SomeTypeFlexed() override = default;

    SomeTypeFlexed(const SomeTypeFlexed& other)
        : rep_(other.rep_) {
    }

    SomeTypeFlexed& operator=(const SomeTypeFlexed& other) {
        this->rep_ = other.rep_;
        return *this;
    }

    SomeTypeFlexed(SomeTypeFlexed&&) = delete;
    SomeTypeFlexed& operator=(SomeTypeFlexed&&) = delete;

    explicit SomeTypeFlexed(const SomeType& x)
        : rep_(x) {
    }

    explicit SomeTypeFlexed(SomeType&& x)
        : rep_(x) {
    }

    ul::Owner<SomeTypeFlexed*> clone() override {
        return new SomeTypeFlexed(*this);
    }

private:
    SomeType rep_;
};

struct IUser {
    virtual ~IUser() = default;
    virtual void f(too::Itype*) = 0;
};

struct UserA : public IUser {
    ~UserA() override = default;

    void f(too::Itype* /*unused*/) override {
    }
};
} // namespace

TEST(ITypeTest, test) {
}
