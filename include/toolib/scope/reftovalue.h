// 2011

//! \file


#ifndef REFTOVALUE_H_o4nv5ht84n82ch
#define REFTOVALUE_H_o4nv5ht84n82ch

#include "../config.h"

namespace mb::too
{
//! Transports a reference as a value.
/** Taken from Loki. Serves to implement the Colvin/Gibbons trick for SmartPtr/ScopeGuard*/
template <typename T>
class RefToValue
{
public:
    explicit RefToValue(T& ref)
        : m_ref(ref)
    {
    }

    RefToValue(const RefToValue& rhs)
        : m_ref(rhs.m_ref)
    {
    }

    RefToValue() = delete;
    RefToValue& operator=(const RefToValue&) = delete;

    explicit operator T&() const
    {
        return m_ref;
    }

private:
    T& m_ref;
};

template <typename T>
RefToValue<T> byRef(T& t)
{
    return RefToValue<T>(t);
}
} // namespace mb::too

TOO_HEADER_END

#endif
