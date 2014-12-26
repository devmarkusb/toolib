// Markus Borris, 2014
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef ILIBRARY_H
#define ILIBRARY_H

#include <memory>
#include "../types.h"


namespace too
{
class ILibrary
{
public:
    virtual ~ILibrary() = default;

    //! \param FilePathNameWithoutExtension without extension
    static inline std::unique_ptr<ILibrary> make(too::string FilePathNameWithoutExtension = too::string(),
                                                 too::string Version = too::string());

    virtual void* ResolveSymbol(std::string Symbol) = 0;
    virtual void SetFileName(too::string FilePathNameWithoutExtension, too::string Version = too::string()) = 0;
    virtual too::string GetFileName() const = 0;
    virtual bool Load() = 0;
    virtual bool Unload() = 0;
    virtual too::string GetError() const = 0;
};

//! Still too abstract, not a real implementation yet.
class CLibrary : public ILibrary
{
public:
    explicit CLibrary(too::string FilePathNameWithoutExtension = too::string(), too::string Version = too::string())
        : m_FilePathName(FilePathNameWithoutExtension), m_Version(Version)
    {
        // would call Load here, if FilePathNameWithoutExtension is non-empty, but calling a virtual function
        // from a constructor would always only call the function of the base/current class
    }

    virtual void* ResolveSymbol(std::string Symbol) = 0;

    virtual void SetFileName(too::string FilePathNameWithoutExtension, too::string Version = too::string())
    {
        m_FilePathName = FilePathNameWithoutExtension;
        m_Version = Version;
    }

    virtual too::string GetFileName() const
    {
        return m_FilePathName;
    }

    virtual bool Load() = 0;
    virtual bool Unload() = 0;

    virtual too::string GetError() const
    {
        return m_Error;
    }

protected:
    virtual void SetError(too::string Error) { m_Error = Error; }
    too::string GetVersion() const { return m_Version; }

private:
    too::string         m_FilePathName;
    too::string         m_Version;
    too::string         m_Error{};
};
}

#endif // ILIBRARY_H
