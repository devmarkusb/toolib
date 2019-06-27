// Markus Borris, 2011
// This file is part of toolib library.

//!
/** Note: I assume that the lack of keyword export (MS VS) made it necessary to put the definitions
    m_init, m_flags, m_aux, m_auxLastflags into this the only file (it took lots of tries with
    awkward template stuff until it finally	worked).
*/
//! \file

#ifndef SCOPEFLAGGED_HPP_inh27dzf237bxfd327
#define SCOPEFLAGGED_HPP_inh27dzf237bxfd327

#include "toolib/math/bits.h"
#include <stack>


namespace too
{
//! Extends any class "user" by the functionality of describing arbitrary options by a set of bit flags.
/** These flags work in a scope guarded manner. An example helps best: \code
struct MyClass : public scope_flagged<MyClass>
{
    // ...
    enum {
        STH1 = 0x02,
        STH2 = 0x04,
        // ...
        SF_DEF = STH_DEF, // SF_DEF is always necessary to provide an initialization default.
    }
    // ...
    void m()
    {
        if (sflags_contains(STH1))
            dothis();
        else
            dothat();
    }
};
void f()
{
    MyClass mc;
    {
        mc::SG_flags sgf;
        sgf.remove(MyClass::STH1);
        sgf.m();
    } // Leaving the block, sgf is destructed and restores defaults.
}
\endcode
One example usage would be to provide certain output (ostream) format options for a class.
Step by step use guide:
1) Include scopeflagged.hpp.
2) Derive any class X from "public scope_flagged<X>".
3) Define at least an "enum { ... SF_DEF ... }" in your class X.
4) Note that in X your reading flag access is provided by "sflags_contains(...)";
   then have a look at too::math::bit::bits.
5) Note that outside of X (use case), you first open up a scope guard by "X::SG_flags scgx;"
   and then access your flags by "scgx."; again via class too::math::bit::bits.
6) That's it! Remaining questions are answered by the very easy source code. Also \see ../math/bits.h.
The beauty is that you can mix different guards and options for arbitrary classes out of a hierarchy
and still get the expected results. Every class has its own flag playground. Sometimes it is convenient
to resolve (compile time) ambiguities by introducing using-statements in the class definitions.
*/
template <class user, class bit_container_type = uint16_t>
class scope_flagged
{
public:
    scope_flagged() {}
    //! Default copy and assignment are ok. Doesn't make much sense, but shouln't be forbidden to not not spoil
    //! derivates.
    // scope_flagged(const scope_flagged&);
    // scope_flagged& operator=(const scope_flagged&);

    struct SG_flags : public toobit::bits<bit_container_type>
    {
        explicit SG_flags(bit_container_type flags = user::SF_DEF) // = m_flags.get())
            : toobit::bits<bit_container_type>(flags)
        {
            m_auxLastflags.push(*m_flags);
            m_flags = toobit::bits<bit_container_type>::getAddress();
        }
        ~SG_flags()
        {
            try
            {
                m_flags  = &m_aux;
                *m_flags = m_auxLastflags.top();
                m_auxLastflags.pop();
            }
            catch (...)
            {
            }
        }

        //! Default copy and assignment are ok. Doesn't make much sense, but shouln't be forbidden to not spoil
        //! derivates.
        // SG_flags(const SG_flags&);
        // SG_flags& operator=(const SG_flags&);
        static std::stack<bit_container_type> m_auxLastflags;
    };
    bool sflags_contains(bit_container_type flags) const { return (*m_flags | flags) == *m_flags; }
    //{ return toobit::bits<bit_container_type>(*m_flags).contains(flags); }
private:
    static bit_container_type m_init;
    static bit_container_type m_aux;
    static bit_container_type* m_flags;
};

// Definition of statics. Templates in this file due to the lack of keyword "export" in most compilers...
template <class user, class bit_container_type>
bit_container_type scope_flagged<user, bit_container_type>::m_init = user::SF_DEF;
template <class user, class bit_container_type>
bit_container_type scope_flagged<user, bit_container_type>::m_aux = user::SF_DEF;
template <class user, class bit_container_type>
bit_container_type* scope_flagged<user, bit_container_type>::m_flags = &scope_flagged<user, bit_container_type>::m_init;
template <class user, class bit_container_type>
std::stack<bit_container_type> scope_flagged<user, bit_container_type>::SG_flags::m_auxLastflags;

} // too

#endif
