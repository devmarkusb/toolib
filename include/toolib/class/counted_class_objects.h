// 2011-16

//! \file Not yet tested.


// Implementation taken from Meyers' book.

#ifndef COUNTED_CLASS_OBJECTS_H_cirewhct34bctgu4372
#define COUNTED_CLASS_OBJECTS_H_cirewhct34bctgu4372

#include "../config.h"


namespace mb::too
{
//! Base class for classes that want to restrict their instantiations to a certain number.
/** Usage: \code
class Printer: private Counted<Printer> {
public:
 static Printer* makePrinter() { return new Printer; }
 static Printer* makePrinter(const Printer& rhs);
 ~Printer();
 ...
 using Counted<Printer>::objectCount;
 using Counted<Printer>::TooManyObjects;
private:
 Printer();
 Printer(const Printer& rhs);
};
const size_t Counted<Printer>::maxObjects = 1;
void f()
{
 try { std::auto_ptr<Printer> p = Printer::makePrinter(); }
 catch (Printer::TooManyObjects&) {...}
 // use it...
}
\endcode
For a maximum number of one, you might use a different approach of using a global static function
returning a reference to a static object.
*/
template <class BeingCounted>
class Counted
{
public:
    //! Exception class for exceeding the object maximum.
    class TooManyObjects
    {
    };
    static long objectCount()
    {
        return m_numObjects;
    }

protected:
    Counted()
    {
        init();
    }
    Counted(const Counted&)
    {
        init();
    }

    ~Counted()
    {
        --m_numObjects;
    }

private:
    static long m_numObjects;
    static const long maxObjects;

    void init()
    {
        if (m_numObjects >= maxObjects)
            throw TooManyObjects();
        ++m_numObjects;
    }
};
template <class BeingCounted>
long Counted<BeingCounted>::m_numObjects; // defines and initializes to zero
} // namespace mb::too

TOO_HEADER_END

#endif
