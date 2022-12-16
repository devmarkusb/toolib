// 2011-17

//! \file


#ifndef RAII_P_H_on824f287xrz2387r
#define RAII_P_H_on824f287xrz2387r

#include "ul/ul.h"
#if UL_OS_WINDOWS
#include <crtdbg.h>
#endif
#include <cstdint>

#include "ul/macros.h"


namespace mb::too
{
//! The most simple smart pointer out there (just RAII).
/** Encapsulates a pointer in a class that cares about deletion. (But prefer std::auto_ptr)*/
template <class T>
class raii_p
{
private:
    T* m_ptr; // pointer to allocated memory for some object

    //! Releases the memory.
    void flush()
    {
        if (m_ptr)
        {
            ul::mem::checked_delete(m_ptr);
            m_ptr = 0;
        }
    }

public:
    //! Allocates memory for an internally stored T* and calls its default constructor.
    /** Example: \code raii_p<someclass> psomeobject; \endcode*/
    raii_p()
        : m_ptr(new T())
    {
    }
    //! Starts memory management (i.e. auto deletion) for a properly Heap-allocated object pt.
    /** This gives opportunity to call a non-default constructor for pt, as in the
    example: \code raii_p<someclass> psomeobject(new someclass(somepara1, ...)); \endcode
    Be careful that you never delete pt by yourself outside. The responsibility for that is shifted
    to this class.
    \param pt pointer to some Heap-allocated memory.*/
    explicit raii_p(T* pt)
        : m_ptr(pt)
    {
#if UL_OS_WINDOWS
        UL_DEBUG_BREAK_IF(!_CrtIsValidHeapPointer(pt));
#endif
    }
    //! Releases the internally managed memory for the object.
    ~raii_p()
    {
        flush();
    }
    raii_p(const raii_p&) = delete;
    raii_p& operator=(const raii_p&) = delete;

    //! Gives opportunity to use raii_p<T> just like T*. Do not delete the return value!
    /** This can be extraordinarily relevant for performance issues, that is the use of loops. There
    you should always prefer this cast() to all the other member calls.*/
    T* cast() const
    {
        return m_ptr;
    }
    //! Gives opportunity to use raii_p<T> just like T* regarding "->"-access.
    T* operator->() const
    {
        return m_ptr;
    }
    //! Gives opportunity to use raii_p<T> just like T* regarding *-indirection (dereferencing).
    T& operator*() const
    {
        return *m_ptr;
    }
    //! Gives away memory control of the internally stored object.
    /** \return a usual T* pointer with user-responsibility and leaves the raii_p object in a clean state.*/
    T* release()
    {
        T* ret = m_ptr;
        m_ptr = 0;
        return ret;
    }
    //! Cleans the class content and inititialises again in a manner just like the corresponding constructor does.
    /** Please take care of the same remarks as for \see raii_p(T* pt).*/
    void reset(T* pt = 0)
    {
        if (pt != m_ptr)
        {
            if (m_ptr)
                ul::mem::checked_delete(m_ptr);
            m_ptr = pt;
#if UL_OS_WINDOWS
            UL_DEBUG_BREAK_IF(!_CrtIsValidHeapPointer(pt));
#endif
        }
    }
}; // raii_p

//===============
//=== raii_ap ===
//===============
//! The most simple smart pointer out there. Version dealing with 1-dimensional C arrays.
/** Encapsulates a pointer in a class that cares about deletion. */
template <class T>
class raii_ap
{
private:
    T* m_ptr; // pointer to allocated memory for some objects
    uint32_t m_count; // count of objects

    //! Releases the memory.
    void flush()
    {
        if (m_ptr)
        {
            ul::mem::checked_array_delete(m_ptr);
            m_ptr = 0;
        }
    }

public:
    //! Allocates memory for an internally stored T[] and calls its default constructor for every element.
    /** \param count number of objects of T.
    Example: \code raii_ap<someclass> psomeobject(5); \endcode*/
    explicit raii_ap(uint32_t count)
        : m_count(count)
        , m_ptr(new T[count])
    {
    }
    //! Starts memory management (i.e. auto deletion) for an already properly Heap-allocated array pt[] resp. pt*.
    /** Be careful that you never delete pt by yourself outside. The responsibility for that is shifted
    to this class.
    \param pt pointer to some Heap-allocated memory.
    \count number of instances of T.*/
    raii_ap(T* pt, uint32_t count)
        : m_count(count)
        , m_ptr(pt)
    {
#if UL_OS_WINDOWS
        UL_DEBUG_BREAK_IF(!_CrtIsValidHeapPointer(pt));
#endif
    }
    raii_ap(const raii_ap&) = delete;
    raii_ap& operator=(const raii_ap&) = delete;
    //! Releases the internally managed memory for the object.
    ~raii_ap()
    {
        flush();
    }

    //! Gives opportunity to use raii_ap<T> just like T*. Do not delete the return value!
    T* cast() const
    {
        UL_DEBUG_BREAK_IF(!m_ptr);
        return m_ptr;
    }
    //! Gives opportunity to use raii_ap<T> just like T* regarding "->"-access.
    T* operator->() const
    {
        UL_DEBUG_BREAK_IF(!m_ptr);
        return m_ptr;
    }
    //! Gives opportunity to use raii_ap<T> just like T* regarding *-indirection (dereferencing).
    T& operator*() const
    {
        UL_DEBUG_BREAK_IF(!m_ptr);
        return *m_ptr;
    }
    //! Gives opportunity to use raii_ap<T> just like T* regarding []-indirection (index access).
    /** Note: For performance critical applications one is
    better off using the cast() and work via pointer opposed to this function call (think of loops).*/
    T& operator[](uint32_t idx) const
    {
        UL_DEBUG_BREAK_IF(!m_ptr);
        return m_ptr[idx];
    }
    //! Gives away memory control of the internally stored object.
    /** \return a usual T* pointer with user-responsibility and leaves the raii_p object in a clean state.*/
    T* release()
    {
        T* ret = m_ptr;
        m_ptr = 0;
        return ret;
    }
    //! \return Number of objects.
    [[nodiscard]] uint32_t size() const
    {
        return m_count;
    }
    //! Cleans the class content and initializes again in a manner just like the corresponding constructor does.
    /** Please take care of the same remarks as for raii_ap(T*, u32).*/
    void reset(T* pt = 0, uint32_t count = 0)
    {
        if (pt != m_ptr)
        {
            if (m_ptr)
                checked_array_delete(m_ptr);
            m_ptr = pt;
            m_count = count;
#if UL_OS_WINDOWS
            UL_DEBUG_BREAK_IF(!_CrtIsValidHeapPointer(pt));
#endif
        }
    }
}; // raii_ap

//================
//=== raii_aap ===
//================
//! The most simple smart pointer out there. Version dealing with 2-dimensional C arrays.
/** Encapsulates a pointer in a class that cares about deletion. */
template <class T>
class raii_aap
{
private:
    T** m_ptr; // pointer to allocated memory for some objects
    uint32_t m_count1; // count of objects in 1st dimension
    uint32_t m_count2; // count of objects in 2nd dimension

    //! Releases the memory.
    void flush()
    {
        if (!m_ptr)
            return;
        for (uint32_t i = 0; i < m_count1; ++i)
            ul::mem::checked_array_delete(m_ptr[i]);
        ul::mem::checked_array_delete(m_ptr);
        m_ptr = 0;
    }

public:
    //! Allocates memory for an internally stored T[] and calls its default constructor for every element.
    /** \param count number of objects of T.
    Example: \code raii_aap<someclass> psomeobject(5); \endcode*/
    raii_aap(uint32_t count1, uint32_t count2)
        : m_count1(count1)
        , m_count2(count2)
        , m_ptr(new T*[count1])
    {
        for (uint32_t i = 0; i < count1; ++i)
            m_ptr[i] = new T[count2];
    }
    raii_aap(const raii_aap&) = delete;
    raii_aap& operator=(const raii_aap&) = delete;
    //! Releases the internally managed memory for the object.
    ~raii_aap()
    {
        flush();
    }

    //! Gives opportunity to use raii_aap<T> just like T**. Do not delete the return value!
    T** cast() const
    {
        UL_DEBUG_BREAK_IF(!m_ptr);
        return m_ptr;
    }
    //! Gives opportunity to use raii_aap<T> just like T** regarding first []-indirection (index access).
    /** For the second dimension a further [] has to follow up. Note: For performance critical applications one is
    better off using the cast() and work via pointer opposed to this function call (think of loops).*/
    T* operator[](uint32_t idx1) const
    {
        UL_DEBUG_BREAK_IF(!m_ptr);
        return m_ptr[idx1];
    }
    //! \return Number of objects in 1st dimension.
    [[nodiscard]] uint32_t size1() const
    {
        return m_count1;
    }
    //! \return Number of objects in 2nd dimension.
    [[nodiscard]] uint32_t size2() const
    {
        return m_count2;
    }
};
} // namespace mb::too

#include "ul/macros_end.h"

#endif
