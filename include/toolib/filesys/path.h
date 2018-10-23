// Markus Borris, 2015-16
// This file is part of toolib library.

//!
/**
*/
//! \file

#ifndef PATH_H_INCL_isdgfx67g42319gyr6r8gfeuiwgfc
#define PATH_H_INCL_isdgfx67g42319gyr6r8gfeuiwgfc

#include "toolibDEF.h"
#include "toolib/warnings.h"
#include <string>
#include <vector>


namespace too
{
namespace file
{
//! String manipulation helper class only. No file system operations.
/** DEPRECATED, use too::std_fs::path instead.*/
class TOOLIBSHARED_EXPORT Path
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

    explicit Path(const std::string& path = std::string(), EForm form = EForm::PLATFORMINDEPENDENT,
        EType type = EType::IS_UNKNOWN);
    //! no copy, use input by reference
    explicit Path(std::string& path, bool useByReference = false, EForm form = EForm::PLATFORMINDEPENDENT,
        EType type = EType::IS_UNKNOWN);
    Path(const Path& other);
    Path(Path&& other);
    Path& operator=(const Path& other);
    Path& operator=(Path&& other);
    void swap(Path& other);

    operator std::string() const;
    Path& operator+=(const Path& other);
    std::string getFolderPath() const;
    //! Inclusive extension. Use function remove_extension if desired.
    std::string getFileName() const;
    //! without dot
    std::string getExtension(bool with_dot = false) const;
    bool isAbsolute() const;
    bool isEmpty() const;

    //! not yet capable of removing subsequent duplicate separators; just converts them to the native form
    Path& cleanupNative();
    //! not yet capable of removing subsequent duplicate separators; just converts them to the platform independent form
    Path& cleanupPlatformIndep();
    Path& ensureTrailingSeparator();
    Path& ensureTrailingSeparator(bool native);
    const std::string& getSeparatorUsedHere() const;

    static const std::string& getSeparatorNative();
    //! platform independent variant used by this class
    static const std::string& getSeparatorPlatformIndep();

private:
TOO_PRAGMA_WARNINGS_PUSH
TOO_WARNING_DISABLE_MSVC(4251)
    //! platform independent variant used by this class
    static const std::string FOLDER_SEPARATOR_TO_USE_HERE;

    std::string m_path_own_internal;
    std::string* m_path = &m_path_own_internal; //! contract: never nullptr
TOO_PRAGMA_WARNINGS_POP
    mutable EForm m_form = EForm::PLATFORMINDEPENDENT;
    EType m_type         = EType::IS_UNKNOWN;

    void detectForm() const;
};

TOOLIBSHARED_EXPORT Path operator+(const Path& p1, const Path& p2);
} // file
} // too


#endif
