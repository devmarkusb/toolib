//! \file

#ifndef RAII_P_H_ON824F287XRZ2387R
#define RAII_P_H_ON824F287XRZ2387R

#include "../config.h"
#include "mb/ul/ul.hpp"
#if UL_OS_WINDOWS
#include <crtdbg.h>
#endif
#include <cstdint>

namespace mb::too {
//! The most simple smart pointer out there (just RAII).
/** Encapsulates a pointer in a class that cares about deletion. (But prefer std::auto_ptr)*/
template <class T>
class RaiiP {
private:
    T* m_ptr_; // pointer to allocated memory for some object

    //! Releases the memory.
    void flush() {
        if (m_ptr_) {
            ul::mem::checked_delete(m_ptr_);
            m_ptr_ = 0;
        }
    }

public:
    //! Allocates memory for an internally stored T* and calls its default constructor.
    /** Example: \code raii_p<someclass> psomeobject; \endcode*/
    RaiiP()
        : m_ptr_(new T()) {
    }

    //! Starts memory management (i.e. auto deletion) for a properly Heap-allocated object pt.
    /** This gives opportunity to call a non-default constructor for pt, as in the
    example: \code raii_p<someclass> psomeobject(new someclass(somepara1, ...)); \endcode
    Be careful that you never delete pt by yourself outside. The responsibility for that is shifted
    to this class.
    \param pt pointer to some Heap-allocated memory.*/
    explicit RaiiP(T* pt)
        : m_ptr_(pt) {
#if UL_OS_WINDOWS
        UL_DEBUG_BREAK_IF(!_CrtIsValidHeapPointer(pt));
#endif
    }

    //! Releases the internally managed memory for the object.
    ~RaiiP() {
        flush();
    }

    RaiiP(const RaiiP&) = delete;
    RaiiP& operator=(const RaiiP&) = delete;

    //! Gives opportunity to use raii_p<T> just like T*. Do not delete the return value!
    /** This can be extraordinarily relevant for performance issues, that is the use of loops. There
    you should always prefer this cast() to all the other member calls.*/
    T* cast() const {
        return m_ptr_;
    }

    //! Gives opportunity to use raii_p<T> just like T* regarding "->"-access.
    T* operator->() const {
        return m_ptr_;
    }

    //! Gives opportunity to use raii_p<T> just like T* regarding *-indirection (dereferencing).
    T& operator*() const {
        return *m_ptr_;
    }

    //! Gives away memory control of the internally stored object.
    /** \return a usual T* pointer with user-responsibility and leaves the raii_p object in a clean state.*/
    T* release() {
        T* ret = m_ptr_;
        m_ptr_ = 0;
        return ret;
    }

    //! Cleans the class content and inititialises again in a manner just like the corresponding constructor does.
    /** Please take care of the same remarks as for \see raii_p(T* pt).*/
    void reset(T* pt = 0) {
        if (pt != m_ptr_) {
            if (m_ptr_)
                ul::mem::checked_delete(m_ptr_);
            m_ptr_ = pt;
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
class RaiiAp {
private:
    T* m_ptr_; // pointer to allocated memory for some objects
    uint32_t m_count_; // count of objects

    //! Releases the memory.
    void flush() {
        if (m_ptr_) {
            ul::mem::checked_array_delete(m_ptr_);
            m_ptr_ = 0;
        }
    }

public:
    //! Allocates memory for an internally stored T[] and calls its default constructor for every element.
    /** \param count number of objects of T.
    Example: \code raii_ap<someclass> psomeobject(5); \endcode*/
    explicit RaiiAp(uint32_t count)
        : m_count_(count)
        , m_ptr_(new T[count]) {
    }

    //! Starts memory management (i.e. auto deletion) for an already properly Heap-allocated array pt[] resp. pt*.
    /** Be careful that you never delete pt by yourself outside. The responsibility for that is shifted
    to this class.
    \param pt pointer to some Heap-allocated memory.
    \param count number of instances of T.*/
    RaiiAp(T* pt, uint32_t count)
        : m_count_(count)
        , m_ptr_(pt) {
#if UL_OS_WINDOWS
        UL_DEBUG_BREAK_IF(!_CrtIsValidHeapPointer(pt));
#endif
    }

    RaiiAp(const RaiiAp&) = delete;
    RaiiAp& operator=(const RaiiAp&) = delete;

    //! Releases the internally managed memory for the object.
    ~RaiiAp() {
        flush();
    }

    //! Gives opportunity to use raii_ap<T> just like T*. Do not delete the return value!
    T* cast() const {
        UL_DEBUG_BREAK_IF(!m_ptr_);
        return m_ptr_;
    }

    //! Gives opportunity to use raii_ap<T> just like T* regarding "->"-access.
    T* operator->() const {
        UL_DEBUG_BREAK_IF(!m_ptr_);
        return m_ptr_;
    }

    //! Gives opportunity to use raii_ap<T> just like T* regarding *-indirection (dereferencing).
    T& operator*() const {
        UL_DEBUG_BREAK_IF(!m_ptr_);
        return *m_ptr_;
    }

    //! Gives opportunity to use raii_ap<T> just like T* regarding []-indirection (index access).
    /** Note: For performance critical applications one is
    better off using the cast() and work via pointer opposed to this function call (think of loops).*/
    T& operator[](uint32_t idx) const {
        UL_DEBUG_BREAK_IF(!m_ptr_);
        return m_ptr_[idx];
    }

    //! Gives away memory control of the internally stored object.
    /** \return a usual T* pointer with user-responsibility and leaves the raii_p object in a clean state.*/
    T* release() {
        T* ret = m_ptr_;
        m_ptr_ = 0;
        return ret;
    }

    //! \return Number of objects.
    [[nodiscard]] uint32_t size() const {
        return m_count_;
    }

    //! Cleans the class content and initializes again in a manner just like the corresponding constructor does.
    /** Please take care of the same remarks as for raii_ap(T*, u32).*/
    void reset(T* pt = 0, uint32_t count = 0) {
        if (pt != m_ptr_) {
            if (m_ptr_)
                checked_array_delete(m_ptr_);
            m_ptr_ = pt;
            m_count_ = count;
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
class RaiiAap {
private:
    T** m_ptr_; // pointer to allocated memory for some objects
    uint32_t m_count1_; // count of objects in 1st dimension
    uint32_t m_count2_; // count of objects in 2nd dimension

    //! Releases the memory.
    void flush() {
        if (!m_ptr_)
            return;
        for (uint32_t i = 0; i < m_count1_; ++i)
            ul::mem::checked_array_delete(m_ptr_[i]);
        ul::mem::checked_array_delete(m_ptr_);
        m_ptr_ = 0;
    }

public:
    //! Allocates memory for an internally stored T[][] and calls its default constructor for every element.
    /**
    \param count1 size in first dimension.
    \param count2 size in second dimension.*/
    RaiiAap(uint32_t count1, uint32_t count2)
        : m_count1_(count1)
        , m_count2_(count2)
        , m_ptr_(new T*[count1]) {
        for (uint32_t i = 0; i < count1; ++i)
            m_ptr_[i] = new T[count2];
    }

    RaiiAap(const RaiiAap&) = delete;
    RaiiAap& operator=(const RaiiAap&) = delete;

    //! Releases the internally managed memory for the object.
    ~RaiiAap() {
        flush();
    }

    //! Gives opportunity to use raii_aap<T> just like T**. Do not delete the return value!
    T** cast() const {
        UL_DEBUG_BREAK_IF(!m_ptr_);
        return m_ptr_;
    }

    //! Gives opportunity to use raii_aap<T> just like T** regarding first []-indirection (index access).
    /** For the second dimension a further [] has to follow up. Note: For performance critical applications one is
    better off using the cast() and work via pointer opposed to this function call (think of loops).*/
    T* operator[](uint32_t idx1) const {
        UL_DEBUG_BREAK_IF(!m_ptr_);
        return m_ptr_[idx1];
    }

    //! \return Number of objects in 1st dimension.
    [[nodiscard]] uint32_t size1() const {
        return m_count1_;
    }

    //! \return Number of objects in 2nd dimension.
    [[nodiscard]] uint32_t size2() const {
        return m_count2_;
    }
};
} // namespace mb::too

TOO_HEADER_END

#endif
