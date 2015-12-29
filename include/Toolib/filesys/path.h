// Markus Borris, 2015
// This file is part of Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef PATH_H_INCL_isdgfx67g42319gyr6r8gfeuiwgfc
#define PATH_H_INCL_isdgfx67g42319gyr6r8gfeuiwgfc

#include <vector>
#include "../types.h"
#include "ToolibDEF.h"

namespace too
{
namespace file
{
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

    explicit CPath(const too::string& path = too::string(), EForm form = EForm::PLATFORMINDEPENDENT, EType type = EType::IS_UNKNOWN);
    //! no copy, use input by reference
    explicit CPath(too::string& path, bool useByReference = false, EForm form = EForm::PLATFORMINDEPENDENT, EType type = EType::IS_UNKNOWN);
    CPath(const CPath& other);
    CPath(CPath&& other);
    CPath& operator=(const CPath& other);
    CPath& operator=(CPath&& other);
    void swap(CPath& other);

    operator too::string() const;
    CPath& operator+=(const CPath& other);
    too::string getFolderPath() const;
    too::string getFileName() const;
    //! without dot
    too::string getExtension() const;
    bool isAbsolute() const;
    bool isEmpty() const;

    //! not yet capable of removing subsequent duplicate separators; just converts them to the native form
    CPath& cleanupNative();
    //! not yet capable of removing subsequent duplicate separators; just converts them to the platform independent form
    CPath& cleanupPlatformIndep();
    CPath& ensureTrailingSeparator();
    CPath& ensureTrailingSeparator(bool native);
    const too::string& getSeparatorUsedHere() const;

    static const too::string& getSeparatorNative();
    //! platform independent variant used by this class
    static const too::string& getSeparatorPlatformIndep();

private:
#include "../PPDefs/MSVC/SUPPRESS_WARNING_4251_BEGIN"
	//! platform independent variant used by this class
    static const too::string FOLDER_SEPARATOR_TO_USE_HERE;

    too::string     m_path_own_internal;
    too::string*    m_path = &m_path_own_internal;	//! contract: never nullptr
#include "../PPDefs/MSVC/SUPPRESS_WARNING_END"
	mutable EForm   m_form = EForm::PLATFORMINDEPENDENT;
    EType           m_type = EType::IS_UNKNOWN;

    void detectForm() const;
};

//todo check whether that works also within namespace
TOOLIBSHARED_EXPORT CPath operator+(const CPath& p1, const CPath& p2);

}
}


#endif
