// 2011

//!
/**
 */
//! \file


#ifndef REFTOVALUE_H_o4nv5ht84n82ch
#define REFTOVALUE_H_o4nv5ht84n82ch


namespace mb::too
{
//! Transports a reference as a value.
/** Taken from Loki. Serves to implement the Colvin/Gibbons trick for SmartPtr/ScopeGuard*/
template <typename T>
class RefToValue
{
public:
    RefToValue(T& ref)
        : m_ref(ref)
    {
    }
    RefToValue(const RefToValue& rhs)
        : m_ref(rhs.m_ref)
    {
    }
    operator T&() const
    {
        return m_ref;
    }

private:
    RefToValue();
    RefToValue& operator=(const RefToValue&);

    T& m_ref;
};

template <typename T>
inline RefToValue<T> ByRef(T& t)
{
    return RefToValue<T>(t);
}
} // namespace mb::too

#endif
