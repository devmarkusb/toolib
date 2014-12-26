// Markus Borris, 2014
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef LINKLIB_DEFS_QT_H_INCL_skdhoikmhcnvncynmxcof78s7
#define LINKLIB_DEFS_QT_H_INCL_skdhoikmhcnvncynmxcof78s7

#include "../ilibrary.h"
#include <QLibrary>
#include <memory>
#include "../../string/impl_Qt/StringConvert_Qt.h"
#include "../../mem/make_unique.h"


#define TOO_DECL_EXPORT     Q_DECL_EXPORT
#define TOO_DECL_IMPORT     Q_DECL_IMPORT


namespace too
{
namespace implQt
{
class CLibrary_Qt : public CLibrary
{
public:
    explicit CLibrary_Qt(too::string FilePathNameWithoutExtension = too::string(),
                         too::string Version = too::string())
        : CLibrary(FilePathNameWithoutExtension, Version)
        , m_impl(toos2qs(FilePathNameWithoutExtension), toos2qs(Version))
    {}

    virtual void* ResolveSymbol(std::string Symbol)
    {
        return reinterpret_cast<void*>(m_impl.resolve(Symbol.c_str()));
    }

    virtual void SetFileName(too::string FilePathNameWithoutExtension, too::string Version = too::string())
    {
        m_impl.setFileNameAndVersion(toos2qs(FilePathNameWithoutExtension), toos2qs(Version));
    }

    virtual too::string GetFileName() const
    {
        return qs2toos(m_impl.fileName());
    }

    virtual bool Load()
    {
        return m_impl.load();
    }

    virtual bool Unload()
    {
        return m_impl.unload();
    }

    virtual too::string GetError() const
    {
        too::string ret(qs2toos(m_impl.errorString()));
        // unfortunately this is the string Qt returns in case of no error
        if (ret == _TOOSTR("Unknown error"))
            ret.clear();
        return ret;
    }

private:
    QLibrary        m_impl;
};
}

inline std::unique_ptr<ILibrary> ILibrary::make(too::string FilePathNameWithoutExtension,
                                                too::string Version)
{
    std::unique_ptr<ILibrary> p(std::make_unique<implQt::CLibrary_Qt>(FilePathNameWithoutExtension, Version));
    if (p && !FilePathNameWithoutExtension.empty())
        p->Load();
    return p;
}
}

#endif
