/** \file Implements a scope guard (ScopeGuard)
    according to the article by Andrei Alexandrescu and Petru Marginean, December 01, 2000.*/

#ifndef SCOPEGUARD_H_N2ZXR732RMYRZ3N2UR
#define SCOPEGUARD_H_N2ZXR732RMYRZ3N2UR

#include "../config.h"
#include <cstddef>

namespace mb::too {
class ScopeGuardImplBase; // fwd. decl.

//! This is the type designated for usage.
/** Example:
\code
std::vector<std::string> svec;
svec.push_back("example string");
too::ScopeGuard guardSVPush = too::MakeObjGuard(svec, &std::vector<std::string>::pop_back);
// ... do other stuff that might throw ...
guardSVPush.NoRollback();
\endcode

This is an example for usage with a member function with no parameters as possible rollback procedure.
There exist overloads of MakeObjGuard() and MakeGuard() suitable for use cases with 1 or 2 parameters and
for usual functions, also with 0, 1, or 2 parameters.

If you like to transfer a parameter by reference, use too::byRef(some_variable) instead of some_variable as parameter.

The call to NoRollback() is of course optional.*/
typedef const ScopeGuardImplBase& ScopeGuard;

//=================================
//=== Implementation base class ===
//=================================

//! Base interface of the ScopeGuard, providing the optional rollback mechanism.
class ScopeGuardImplBase {
public:
    //! Calling this method ensures, that no rollback is performed by the constructor.
    /** So all actions preceding the guarding are commited.*/
    void no_rollback() const noexcept {
        m_b_rollback_ = false;
    }

protected:
    //! Construction, assumes a later rollback.
    ScopeGuardImplBase()
        : m_b_rollback_(true) {
    }

    //! Copy, takes over responsibility.
    ScopeGuardImplBase(const ScopeGuardImplBase& other)
        : m_b_rollback_(other.m_b_rollback_) {
        other.no_rollback();
    }

    //! Does nothing.
    ~ScopeGuardImplBase() = default;
    //! This holds the decision over rollback.
    mutable bool m_b_rollback_;

private:
    //! Forbid assignment.
    ScopeGuardImplBase& operator=(const ScopeGuardImplBase&);
#pragma push_macro("new")
#undef new
    //! Forbid heap allocation. ScopeGuard is intended to be used as automatic mechanism in definite blocks.
    void* operator new(size_t);
#pragma pop_macro("new")
    //! Forbid address investigation.
    ScopeGuardImplBase* operator&();
    //! Forbid address investigation, const-version.
    const ScopeGuardImplBase* operator&() const;
};

//=========================================
//=== Functions with 0, 1, 2 parameters ===
//=========================================

//! Provides the mechanism for rollback functions with 0 parameters.
template <typename Fct>
class ScopeGuardImpl0 : public ScopeGuardImplBase {
public:
    //! Assigns the function.
    explicit ScopeGuardImpl0(Fct fct)
        : m_fct_(fct) {
    }

    //! Calls the function for rollback, if not prevented upfront by call of NoRollback().
    ~ScopeGuardImpl0() {
        if (m_b_rollback_) {
            try {
                m_fct_();
            } catch (...) {
            }
        }
    }

private:
    //! Reference to the rollback function.
    Fct m_fct_;
};

//! Provides the mechanism for rollback functions with 1 parameter.
template <typename Fct, typename Para1>
class ScopeGuardImpl1 : public ScopeGuardImplBase {
public:
    ScopeGuardImpl1(Fct fct, Para1 para1)
        : m_fct_(fct)
        , m_para1_(para1) {
    }

    ~ScopeGuardImpl1() {
        if (m_b_rollback_) {
            try {
                m_fct_(m_para1_);
            } catch (...) {
            }
        }
    }

private:
    Fct m_fct_;
    //! Reference to first parameter of the rollback function.
    const Para1 m_para1_;
};

//! Provides the mechanism for rollback functions with 2 parameters.
template <typename Fct, typename Para1, typename Para2>
class ScopeGuardImpl2 : public ScopeGuardImplBase {
public:
    ScopeGuardImpl2(Fct fct, Para1 para1, Para2 para2)
        : m_fct_(fct)
        , m_para1_(para1)
        , m_para2_(para2) {
    }

    ~ScopeGuardImpl2() {
        if (m_b_rollback_) {
            try {
                m_fct_(m_para1_, m_para2_);
            } catch (...) {
            }
        }
    }

private:
    Fct m_fct_;
    const Para1 m_para1_;
    //! Reference to second parameter of the rollback function.
    const Para2 m_para2_;
};

//================================================
//=== Member functions with 0, 1, 2 parameters ===
//================================================

//! Provides the mechanism for rollback member functions (for a specific object) with 0 parameters.
template <class Obj, typename MemFct>
class ObjScopeGuardImpl0 : public ScopeGuardImplBase {
public:
    //! Assigns the object and the member function.
    ObjScopeGuardImpl0(Obj& obj, MemFct mem_fct)
        : m_obj_(obj)
        , m_mem_fct_(mem_fct) {
    }

    //! Calls the function for rollback, if not prevented upfront by call of NoRollback().
    ~ObjScopeGuardImpl0() {
        if (m_b_rollback_) {
            try {
                (m_obj_.*m_mem_fct_)();
            } catch (...) {
            }
        }
    }

private:
    //! Reference to the object for which the rollback can be performed.
    Obj& m_obj_;
    //! Pointer to the rollback function.
    MemFct m_mem_fct_;
};

//! Provides the mechanism for rollback member functions (for a specific object) with 1 parameter.
template <class Obj, typename MemFct, typename Para1>
class ObjScopeGuardImpl1 : public ScopeGuardImplBase {
public:
    ObjScopeGuardImpl1(Obj& obj, MemFct mem_fct, Para1 para1)
        : m_obj_(obj)
        , m_mem_fct_(mem_fct)
        , m_para1_(para1) {
    }

    ~ObjScopeGuardImpl1() {
        if (m_b_rollback_) {
            try {
                (m_obj_.*m_mem_fct_)(m_para1_);
            } catch (...) {
            }
        }
    }

private:
    Obj& m_obj_;
    MemFct m_mem_fct_;
    //! Reference to first parameter of the rollback function.
    const Para1 m_para1_;
};

//! Provides the mechanism for rollback member functions (for a specific object) with 2 parameters.
template <class Obj, typename MemFct, typename Para1, typename Para2>
class ObjScopeGuardImpl2 : public ScopeGuardImplBase {
public:
    ObjScopeGuardImpl2(Obj& obj, MemFct mem_fct, Para1 para1, Para2 para2)
        : m_obj_(obj)
        , m_mem_fct_(mem_fct)
        , m_para1_(para1)
        , m_para2_(para2) {
    }

    ~ObjScopeGuardImpl2() {
        if (m_b_rollback_) {
            try {
                (m_obj_.*m_mem_fct_)(m_para1_, m_para2_);
            } catch (...) {
            }
        }
    }

private:
    Obj& m_obj_;
    MemFct m_mem_fct_;
    const Para1 m_para1_;
    //! Reference to second parameter of the rollback function.
    const Para2 m_para2_;
};

//===============================
//=== Creation help functions ===
//===============================

//=== for functions ===

//! Creates a scope guard with a specific rollback function.
template <typename Fct>
ScopeGuardImpl0<Fct> make_guard(Fct fct) {
    return ScopeGuardImpl0<Fct>(fct);
}

//! Creates a scope guard with a specific rollback function with 1 parameter.
template <typename Fct, typename Para1>
ScopeGuardImpl1<Fct, Para1> make_guard(Fct fct, Para1 para1) {
    return ScopeGuardImpl1<Fct, Para1>(fct, para1);
}

//! Creates a scope guard with a specific rollback function with 2 parameters.
template <typename Fct, typename Para1, typename Para2>
ScopeGuardImpl2<Fct, Para1, Para2> make_guard(Fct fct, Para1 para1, Para2 para2) {
    return ScopeGuardImpl2<Fct, Para1, Para2>(fct, para1, para2);
}

//=== for member functions ===

//! Creates a scope guard with a specific rollback object plus member function.
template <class Obj, typename MemFct>
ObjScopeGuardImpl0<Obj, MemFct> make_obj_guard(Obj& obj, MemFct mem_fct) {
    return ObjScopeGuardImpl0<Obj, MemFct>(obj, mem_fct);
}

//! Creates a scope guard with a specific rollback object plus member function with 1 parameter.
template <class Obj, typename MemFct, typename Para1>
ObjScopeGuardImpl1<Obj, MemFct, Para1> make_obj_guard(Obj& obj, MemFct mem_fct, Para1 para1) {
    return ObjScopeGuardImpl1<Obj, MemFct, Para1>(obj, mem_fct, para1);
}

//! Creates a scope guard with a specific rollback object plus member function with 2 parameters.
template <class Obj, typename MemFct, typename Para1, typename Para2>
ObjScopeGuardImpl2<Obj, MemFct, Para1, Para2> make_obj_guard(Obj& obj, MemFct mem_fct, Para1 para1, Para2 para2) {
    return ObjScopeGuardImpl2<Obj, MemFct, Para1, Para2>(obj, mem_fct, para1, para2);
}
} // namespace mb::too

// always useful together with scopeguard
#include "reftovalue.h"

TOO_HEADER_END

#endif
