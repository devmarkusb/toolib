// Markus Borris, 2011
// This file is part of Toolib library. Open source.

//!
/** Filename stands for "mixed-mode type conversions".
*/
//! \file


#ifndef MMTYPECONV_H_INCL_nbi4lnv5
#define MMTYPECONV_H_INCL_nbi4lnv5

#include <string>

#include "Toolib/PPDEFS.h"

namespace too
{
namespace CLI
{
#if TOO_STANDARD_CPP_CLI
using System::String;
using System::IntPtr;
using System::Runtime::InteropServices::Marshal;
inline String ^ stringA2cli(const char* s)
{
    return gcnew String(s);
} inline const char* stringA2cpp(String ^ s)
{
    IntPtr p = Marshal::StringToHGlobalAnsi(s);
    std::string ret(static_cast<char*>(p.ToPointer()));
    Marshal::FreeHGlobal(p);
    return ret.c_str();
}
inline std::string sstringA2cpp(String ^ s)
{
    IntPtr p = Marshal::StringToHGlobalAnsi(s);
    std::string ret(static_cast<char*>(p.ToPointer()));
    Marshal::FreeHGlobal(p);
    return ret;
}
inline String ^ stringW2cli(wchar_t* s)
{
    return gcnew String(s);
} inline const wchar_t* stringW2cpp(String ^ s)
{
    IntPtr p = Marshal::StringToHGlobalUni(s);
    std::wstring ret(static_cast<wchar_t*>(p.ToPointer()));
    Marshal::FreeHGlobal(p);
    return ret.c_str();
}
inline std::wstring sstringW2cpp(String ^ s)
{
    IntPtr p = Marshal::StringToHGlobalUni(s);
    std::wstring ret(static_cast<wchar_t*>(p.ToPointer()));
    Marshal::FreeHGlobal(p);
    return ret;
}
template <typename T>
inline void array2cpp_readonly(array<T> ^ a, T*& converted_a)
{
    pin_ptr<T> pa = &a[0];
    converted_a   = pa;
}
template <typename T>
inline void array2cpp(array<T> ^ a, T*& converted_a, size_t size)
{
    Marshal::Copy(a, 0, static_cast<IntPtr>(converted_a), size);
}
template <typename T>
    inline array<T> ^
    array2cli(T*& a, size_t size)
{
    array<T> ^ converted_a = gcnew array<T> ^ (size);
    Marshal::Copy(static_cast<IntPtr>(a), converted_a, 0, size);
    return converted_a;
} template <typename T>
    inline array<IntPtr> ^ array2cli(T**& a, size_t size)
{
    array<IntPtr> ^ converted_a = gcnew array<IntPtr> ^ (size);
    Marshal::Copy(static_cast<IntPtr>(a), converted_a, 0, size);
    return converted_a;
}
#endif
}
} // too

#endif
