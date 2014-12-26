// Markus Borris, 2014
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef LINKLIB_DEFS_TOO_H_INCL_sjdnxfzuergf3ui7yt83gzf7r87wr
#define LINKLIB_DEFS_TOO_H_INCL_sjdnxfzuergf3ui7yt83gzf7r87wr

#include <string>
#include <assert.h>
#include "../../PPDEFS.h"
#include "../ilibrary.h"
#include "../../mem/make_unique.h"
#include "../../types.h"
#if TOO_OS_WINDOWS == 1
#include <windows.h>
#include <sstream>
#include "../../string/str_convert.h"
#include "../../lex_cast.h"
#elif TOO_OS_LINUX == 1
#include <dlfcn.h>
#endif

#if TOO_OS_WINDOWS == 1
//! Perhaps TOO_LINKLIB_IMPL_CHOICE_TOOLIB_CFG_STATICLIB has to be defined when you compile a library as static lib and also when you use it.
#if !defined(TOO_LINKLIB_IMPL_CHOICE_TOOLIB_CFG_STATICLIB)
#define TOO_DECL_EXPORT     __declspec(dllexport)
#define TOO_DECL_IMPORT     __declspec(dllimport)
#elif TOO_LINKLIB_IMPL_CHOICE_TOOLIB_CFG_STATICLIB == 1
#define TOO_DECL_EXPORT
#define TOO_DECL_IMPORT
#endif
#elif TOO_OS_LINUX == 1
#define TOO_DECL_EXPORT
#define TOO_DECL_IMPORT
#else
//todo, don't think this alone works for all remaining platforms ;)
#define TOO_DECL_EXPORT
#define TOO_DECL_IMPORT
#endif


//! Declare the general dll calling convention.
#if defined(_STDCALL_SUPPORTED)
#define TOO_CALLINGCONVENTION   __stdcall
#else
#define TOO_CALLINGCONVENTION   __cdecl
#endif


//! Declare the dll calling convention for DllMain, Windows.
#if TOO_OS_WINDOWS == 1 && ((_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED))
#define TOO_WINAPI_DLLMAIN      __stdcall
#else
#define TOO_WINAPI_DLLMAIN
#endif
#define TOO_APIENTRY            WINAPI_DLLMAIN


namespace too
{
#if TOO_OS_WINDOWS == 1
class CLibrary_win : public CLibrary
{
public:
    explicit CLibrary_win(too::string FilePathNameWithoutExtension = too::string(), too::string Version = too::string())
        : CLibrary(FilePathNameWithoutExtension, Version)
    {}

    virtual void* ResolveSymbol(std::string Symbol)
    {
        if (!m_DllHandle)
        {
            SetError("Library not loaded.");
            return nullptr;
        }
        // LPCSTR always const char*
        FARPROC ret = GetProcAddress(m_DllHandle, Symbol.c_str());
        if (!ret)
        {
            std::stringstream ssErr;
            ssErr << "GetProcAddress " << Symbol << " failed: ";
            ssErr << too::lex_cast<std::string>(GetLastError());
            SetError(ssErr.str());
        }
        return reinterpret_cast<void*>(ret);
    }

    virtual bool Load()
    {
        too::string dll(GetFileName());
        dll+= _TOOSTR(".dll");
        // LPCTSTR is const wchar_t* and UTF16 assuming Windows-Unicode
        m_DllHandle = LoadLibrary(too::str::Utf8_string_To_Utf16_wstring(dll).c_str());
        if (!m_DllHandle)
        {
            too::stringstream ssErr;
            ssErr << "LoadLibrary " << dll << " failed: ";
            ssErr << too::lex_cast<too::string>(GetLastError());
            SetError(ssErr.str());
        }
        return m_DllHandle;
    }

    virtual bool Unload()
    {
        bool ret = FreeLibrary(m_DllHandle);
        if (!ret)
        {
            too::stringstream ssErr;
            ssErr << "FreeLibrary " << GetFileName() << " failed: ";
            ssErr << too::lex_cast<too::string, DWORD>(GetLastError());
            SetError(ssErr.str());
        }
        return ret;
    }

private:
    HMODULE     m_DllHandle{nullptr};
};
#elif TOO_OS_LINUX == 1
class CLibrary_linux : public CLibrary
{
public:
    explicit CLibrary_linux(too::string FilePathNameWithoutExtension = too::string(), too::string Version = too::string())
        : CLibrary(FilePathNameWithoutExtension, Version)
    {}

    virtual void* ResolveSymbol(std::string Symbol)
    {
        if (!m_DllHandle)
        {
            SetError("Library not loaded.");
            return nullptr;
        }
        return dlsym(m_DllHandle, Symbol.c_str());
    }

    virtual bool Load()
    {
        too::string dll(GetFileName());
        dll+= _TOOSTR(".so");
        m_DllHandle = dlopen(dll.c_str(), OPEN_MODE);
        return m_DllHandle;
    }

    virtual bool Unload()
    {
        return dlclose(m_DllHandle);
    }

private:
    void*               m_DllHandle{nullptr};
    static const int    OPEN_MODE  =   2;
};
#endif

inline std::unique_ptr<ILibrary> ILibrary::make(too::string FilePathNameWithoutExtension, too::string Version)
{
    std::unique_ptr<ILibrary> p{nullptr};
#if TOO_OS_WINDOWS == 1
    p = std::make_unique<CLibrary_win>(FilePathNameWithoutExtension, Version);
#elif TOO_OS_LINUX == 1
    p = std::make_unique<CLibrary_linux>(FilePathNameWithoutExtension, Version);
#else
#endif
    if (p && !FilePathNameWithoutExtension.empty())
        p->Load();
    return p;
}
}

#endif
