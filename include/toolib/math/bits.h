// Markus Borris, 2011
// This file is part of toolib library.

//!
/**
*/
//! \file


#ifndef BITS_H_INCL_inewhjr4382zrn83
#define BITS_H_INCL_inewhjr4382zrn83

#include <cstdint>
#include <utility>


namespace too
{
namespace math
{
namespace bit
{
struct B
{
    enum _
    {
        F = 0,
        T = 1,
        L = 0,
        H = 1,
        O = 0,
        I = 1
    };
};

template <typename Ty>
B::_ i2B(Ty t = Ty())
{
    return static_cast<B::_>(t);
}


template <>
inline B::_ i2B<int>(int i)
{
    return static_cast<B::_>(i);
}

template <typename T>
class bits
{
public:
    explicit bits(T bits = T()) : m_bits(bits) {}
    // bits(const bits& b) { m_bits = b.m_bits; }
    // bits& operator=(const bits& b) { bits<T> temp(b); temp.swap(*this); return *this; }
    // void swap(bits& b) { std::swap(m_bits, b.m_bits); }

    T& set(T bits)
    {
        m_bits = bits;
        return m_bits;
    }
    T& add(T mask)
    {
        m_bits |= mask;
        return m_bits;
    }
    T& remove(T mask)
    {
        m_bits &= ~mask;
        return m_bits;
    }
    bool contains(T mask) const { return (m_bits | mask) == m_bits; }
    T& fill(B::_ b)
    {
        if (b == B::O)
            m_bits = 0;
        else
            m_bits = ~0;
        return m_bits;
    }
    T& fill(T b) { return fill(i2B<T>(b)); }
    T& clear() { return fill(B::O); }
    T& flip()
    {
        m_bits = ~m_bits;
        return m_bits;
    }
    T& get() { return m_bits; }
protected:
    T* getAddress() { return &m_bits; }
private:
    T m_bits;
};

typedef bits<uint8_t> bits8;
typedef bits<uint16_t> bits16;
typedef bits<uint32_t> bits32;
}
}
}

namespace toobit = too::math::bit;

#endif
