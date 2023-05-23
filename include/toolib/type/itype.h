/** \file This is left for documentation or future thinking. Apparently there should be
    no application for such a general interface.*/

#ifndef ITYPE_H_39653425HGJDHGXHRGBUBEIR43
#define ITYPE_H_39653425HGJDHGXHRGBUBEIR43

#include "../config.h"
#include "ul/ptr.h"

namespace mb::too {
/** Sometimes you are in the situation of declaring abstract interfaces and
    wanting to use a flexible type just as if you would rather like to write
    a template (which is not outright recommended for base classes and not
    possible for virtual functions).
    So IType is your 'type template parameter' so to say. And it is an alternate
    choice next to any or variant tyes. Of course here the approach is inheritence
    and dynamic typing.
    Usage:
    \code
    struct IUser
    {
        virtual void f(IType*) = 0;
    };

    struct UserA : public IUser
    {
        virtual void f(IType*) {}
    };
    \endcode
    Now the types you are really expecting in your implementation most of the time
    won't naturally be implementing/inheriting from IType.
    So will just have to write a wrapper/adapter.
    You can find an example implementation of IType, which means building an adapter class
    for an arbitrary type that you really wanted to use as IType, in the test code,
    IType.test.cpp.
*/
struct Itype {
    virtual ~Itype() = default;
    Itype() = default;
    Itype(const Itype&) = delete;
    Itype& operator=(const Itype&) = delete;
    Itype(Itype&&) = delete;
    Itype& operator=(Itype&&) = delete;

    virtual ul::Owner<Itype*> clone() = 0;
};
} // namespace mb::too

TOO_HEADER_END

#endif
