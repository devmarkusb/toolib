// Markus Borris, 2011
// This file is part of Toolib library. Open source.

//!
/**
*/
//! \file


#ifndef AUTOPTR_H_INCL_ni238z37z2734z32
#define AUTOPTR_H_INCL_ni238z37z2734z32

#include <memory>


namespace too
{
//! Like std::auto_ptr, just without any "gimmicks" and with implicit casting. Only functionality: auto deletion.
template <class I>
class autoptr : public std::auto_ptr<I>
{
private:
    autoptr(const autoptr& ap);
    autoptr& operator=(const autoptr& ap);
    template <class J>
    autoptr(const autoptr<J>& ap);
    template <class J>
    autoptr& operator=(const autoptr<J>& ap);

public:
    autoptr(I* pi) : std::auto_ptr<I>(pi) {}
    operator I*() { return std::auto_ptr<I>::get(); }
};
}

#endif
