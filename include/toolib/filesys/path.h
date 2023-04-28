//! \file

#ifndef PATH_H_isdgfx67g42319gyr6r8gfeuiwgfc
#define PATH_H_isdgfx67g42319gyr6r8gfeuiwgfc

#include "toolibDEF.h"
#include "../config.h"
#include "ul/warnings.h"
#include <string>
#include <vector>

namespace mb::too::file {
TOOLIBSHARED_EXPORT void remove_extension(std::string& fn);

//! String manipulation helper class only. No file system operations.
/** DEPRECATED, use ul::std_fs::path instead.*/
class TOOLIBSHARED_EXPORT Path {
public:
    enum class EType {
        IS_UNKNOWN,
        IS_FOLDER,
        IS_FILE,
        IS_LINK,
    };
    enum class EForm {
        UNKNOWN,
        PLATFORMINDEPENDENT,
        NATIVE,
    };

    explicit Path(
        const std::string& path = std::string(), EForm form = EForm::PLATFORMINDEPENDENT,
        EType type = EType::IS_UNKNOWN);
    //! no copy, use input by reference
    explicit Path(
        std::string& path, bool useByReference = false, EForm form = EForm::PLATFORMINDEPENDENT,
        EType type = EType::IS_UNKNOWN);
    Path(const Path& other);
    Path(Path&& other) noexcept;
    Path& operator=(const Path& other);
    Path& operator=(Path&& other) noexcept;
    void swap(Path& other);

    /*implicit*/ operator std::string() const;
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
    UL_PRAGMA_WARNINGS_PUSH
    UL_WARNING_DISABLE_MSVC(4251)
    //! platform independent variant used by this class
    static const std::string FOLDER_SEPARATOR_TO_USE_HERE;

    std::string m_path_own_internal;
    std::string* m_path = &m_path_own_internal; //! contract: never nullptr
    UL_PRAGMA_WARNINGS_POP
    mutable EForm m_form = EForm::PLATFORMINDEPENDENT;
    EType m_type = EType::IS_UNKNOWN;

    void detectForm() const;
};

TOOLIBSHARED_EXPORT Path operator+(const Path& p1, const Path& p2);
} // namespace mb::too::file

TOO_HEADER_END

#endif
