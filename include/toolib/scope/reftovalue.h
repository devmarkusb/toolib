//! \file

#ifndef REFTOVALUE_H_O4NV5HT84N82CH
#define REFTOVALUE_H_O4NV5HT84N82CH

#include "../config.h"

namespace mb::too {
//! Transports a reference as a value.
/** Taken from Loki. Serves to implement the Colvin/Gibbons trick for SmartPtr/ScopeGuard*/
template <typename T>
class RefToValue {
public:
    explicit RefToValue(T& ref)
        : m_ref_(ref) {
    }

    RefToValue(const RefToValue& rhs)
        : m_ref_(rhs.m_ref_) {
    }

    RefToValue() = delete;
    RefToValue& operator=(const RefToValue&) = delete;

    explicit operator T&() const {
        return m_ref_;
    }

private:
    T& m_ref_;
};

template <typename T>
RefToValue<T> by_ref(T& t) {
    return RefToValue<T>(t);
}
} // namespace mb::too

TOO_HEADER_END

#endif
