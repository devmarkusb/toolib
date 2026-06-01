//! \file Not yet tested.

// Implementation taken from Meyers' book.

#ifndef COUNTED_CLASS_OBJECTS_HPP_CIREWHCT34BCTGU4372
#define COUNTED_CLASS_OBJECTS_HPP_CIREWHCT34BCTGU4372

#include "../config.hpp"

namespace mb::too {
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
class Counted {
public:
    //! Exception class for exceeding the object maximum.
    class TooManyObjects {};

    static long object_count() {
        return m_num_objects;
    }

protected:
    Counted() {
        init();
    }

    Counted(const Counted&) {
        init();
    }

    ~Counted() {
        --m_num_objects;
    }

private:
    static long m_num_objects;
    static const long max_objects;

    static void init() {
        if (m_num_objects >= max_objects)
            throw TooManyObjects();
        ++m_num_objects;
    }
};
template <class BeingCounted>
long Counted<BeingCounted>::m_num_objects; // defines and initializes to zero
} // namespace mb::too


#endif
