// Markus Borris, 2015
// This file is part of Toolib library.

//!
/**
*/
//! \file

#ifndef PATH_H_INCL_isdgfx67g42319gyr6r8gfeuiwgfc
#define PATH_H_INCL_isdgfx67g42319gyr6r8gfeuiwgfc

#include "ToolibDEF.h"
#include <string>
#include <vector>


namespace too
{
namespace file
{

TOOLIBSHARED_EXPORT void remove_extension(std::string& fn);
TOOLIBSHARED_EXPORT bool file_exists(const std::string& fn);

//! String manipulation helper class only. No file system operations.
class TOOLIBSHARED_EXPORT CPath
{
public:
    enum class EType
    {
        IS_UNKNOWN,
        IS_FOLDER,
        IS_FILE,
        IS_LINK,
    };
    enum class EForm
    {
        UNKNOWN,
        PLATFORMINDEPENDENT,
        NATIVE,
    };

    explicit CPath(const std::string& path = std::string(), EForm form = EForm::PLATFORMINDEPENDENT,
        EType type = EType::IS_UNKNOWN);
    //! no copy, use input by reference
    explicit CPath(std::string& path, bool useByReference = false, EForm form = EForm::PLATFORMINDEPENDENT,
        EType type = EType::IS_UNKNOWN);
    CPath(const CPath& other);
    CPath(CPath&& other);
    CPath& operator=(const CPath& other);
    CPath& operator=(CPath&& other);
    void swap(CPath& other);

    operator std::string() const;
    CPath& operator+=(const CPath& other);
    std::string getFolderPath() const;
    //! Inclusive extension. Use function remove_extension if desired.
    std::string getFileName() const;
    //! without dot
    std::string getExtension(bool with_dot = false) const;
    bool isAbsolute() const;
    bool isEmpty() const;

    //! not yet capable of removing subsequent duplicate separators; just converts them to the native form
    CPath& cleanupNative();
    //! not yet capable of removing subsequent duplicate separators; just converts them to the platform independent form
    CPath& cleanupPlatformIndep();
    CPath& ensureTrailingSeparator();
    CPath& ensureTrailingSeparator(bool native);
    const std::string& getSeparatorUsedHere() const;

    static const std::string& getSeparatorNative();
    //! platform independent variant used by this class
    static const std::string& getSeparatorPlatformIndep();

private:
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNING_4251_BEGIN"
    //! platform independent variant used by this class
    static const std::string FOLDER_SEPARATOR_TO_USE_HERE;

    std::string m_path_own_internal;
    std::string* m_path = &m_path_own_internal; //! contract: never nullptr
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNING_END"
    mutable EForm m_form = EForm::PLATFORMINDEPENDENT;
    EType m_type         = EType::IS_UNKNOWN;

    void detectForm() const;
};

// todo check whether that works also within namespace
TOOLIBSHARED_EXPORT CPath operator+(const CPath& p1, const CPath& p2);
}
}


#endif
