//! \file

#ifndef PATH_H_ISDGFX67G42319GYR6R8GFEUIWGFC
#define PATH_H_ISDGFX67G42319GYR6R8GFEUIWGFC

#include "../config.h"
#include "mb/ul/buildenv/warnings.hpp"
#include <string>
#include <string_view>
#include <vector>

namespace mb::too::file {
void remove_extension(std::string& fn);

//! String manipulation helper class only. No file system operations.
/** DEPRECATED, use ul::std_fs::path instead.*/
class Path {
public:
    enum class EType {
        is_unknown,
        is_folder,
        is_file,
        is_link,
    };
    enum class EForm {
        unknown,
        platformindependent,
        native,
    };

    explicit Path(
        const std::string& path = std::string(), EForm form = EForm::platformindependent,
        EType type = EType::is_unknown);
    //! no copy, use input by reference
    explicit Path(
        std::string& path, bool use_by_reference = false, EForm form = EForm::platformindependent,
        EType type = EType::is_unknown);
    Path(const Path& other);
    Path(Path&& other) noexcept;
    Path& operator=(const Path& other);
    Path& operator=(Path&& other) noexcept;
    void swap(Path& other);

    /*implicit*/ operator std::string() const;
    Path& operator+=(const Path& other);
    std::string get_folder_path() const;
    //! Inclusive extension. Use function remove_extension if desired.
    std::string get_file_name() const;
    //! without dot
    std::string get_extension(bool with_dot = false) const;
    bool is_absolute() const;
    bool is_empty() const;

    //! not yet capable of removing subsequent duplicate separators; just converts them to the native form
    Path& cleanup_native();
    //! not yet capable of removing subsequent duplicate separators; just converts them to the platform independent form
    Path& cleanup_platform_indep();
    Path& ensure_trailing_separator();
    Path& ensure_trailing_separator(bool native);
    std::string_view get_separator_used_here() const;

    static std::string_view get_separator_native();
    //! platform independent variant used by this class
    static std::string_view get_separator_platform_indep();

private:
    UL_PRAGMA_WARNINGS_PUSH
    UL_WARNING_DISABLE_MSVC(4251)
    //! platform independent variant used by this class
    static constexpr std::string_view folder_separator_to_use_here{"/"};

    std::string m_path_own_internal_;
    std::string* m_path_ = &m_path_own_internal_; //! contract: never nullptr
    UL_PRAGMA_WARNINGS_POP
    mutable EForm m_form_ = EForm::platformindependent;
    EType m_type_ = EType::is_unknown;

    void detect_form() const;
};

Path operator+(const Path& p1, const Path& p2);
} // namespace mb::too::file


#endif
