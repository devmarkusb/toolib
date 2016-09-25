// Markus Borris, 2011-16
// This file is part of Toolib library.

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
class autoptr : public std::unique_ptr<I>
{
private:
    autoptr(const autoptr& ap);
    autoptr& operator=(const autoptr& ap);
    template <class J>
    autoptr(const autoptr<J>& ap);
    template <class J>
    autoptr& operator=(const autoptr<J>& ap);

public:
    autoptr(I* pi) : std::unique_ptr<I>(pi) {}
    operator I*() { return std::unique_ptr<I>::get(); }
};
}

#endif
