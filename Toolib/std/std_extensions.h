// Markus Borris, 2014
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef STD_EXTENSIONS_H_kjhsdnxuwezkngfywzui6r
#define STD_EXTENSIONS_H_kjhsdnxuwezkngfywzui6r

//! File should be deprecated with C++14.

#include <memory>
#include <type_traits>

namespace std
{
#ifndef make_unique
template <typename T, typename ...Args>
std::unique_ptr<T> make_unique(Args&& ...args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
#endif

#ifndef add_lvalue_reference_t
template <class T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;
#endif
#ifndef add_rvalue_reference_t
template <class T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;
#endif

#ifndef add_cv_t
template <class T>
using add_cv_t       = typename add_cv<T>::type;
#endif
#ifndef add_const_t
template <class T>
using add_const_t    = typename add_const<T>::type;
#endif
#ifndef add_volatile_t
template <class T>
using add_volatile_t = typename add_volatile<T>::type;
#endif
#ifndef remove_cv_t
template <class T>
using remove_cv_t       = typename remove_cv<T>::type;
#endif
#ifndef remove_const_t
template <class T>
using remove_const_t    = typename remove_const<T>::type;
#endif
#ifndef remove_volatile_t
template <class T>
using remove_volatile_t = typename remove_volatile<T>::type;
#endif

#ifndef enable_if_t
template <bool B, class T = void>
using enable_if_t = typename enable_if<B,T>::type;
#endif

#ifndef conditional_t
template <bool B, class T, class F>
using conditional_t = typename conditional<B,T,F>::type;
#endif

#ifndef add_pointer_t
template <class T>
using add_pointer_t = typename add_pointer<T>::type;
#endif

#ifndef decay_t
template <class T>
using decay_t = typename decay<T>::type;
#endif

#ifndef remove_all_extents_t
template <class T>
using remove_all_extents_t = typename remove_all_extents<T>::type;
#endif

#ifndef remove_reference_t
template <class T>
using remove_reference_t = typename remove_reference<T>::type;
#endif
}

#endif
