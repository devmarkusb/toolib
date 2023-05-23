//! \file

#include "toolib/filesys/path.h"
#include "ul/string/string_token.h"
#include "ul/ul.h"
#include <algorithm>
#include <fstream>

namespace {
const std::string os_possible_separators = "/\\"; // NOLINT
#if UL_OS_WINDOWS
const std::string os_folder_separator = "\\"; // NOLINT
#else
const std::string os_folder_separator = "/"; // NOLINT
#endif
} // namespace

namespace mb::too::file {
const std::string Path::folder_separator_to_use_here = "/";

void remove_extension(std::string& fn) {
    const size_t lastdot = fn.find_last_of('.');
    if (lastdot == std::string::npos)
        return;
    fn = fn.substr(0, lastdot);
}

Path::Path(const std::string& path, EForm form, EType type)
    : m_form_(form)
    , m_type_(type) {
    *m_path_ = path;
}

Path::Path(std::string& path, bool use_by_reference, EForm form, EType type)
    : m_form_(form)
    , m_type_(type) {
    if (use_by_reference)
        m_path_ = &path;
    else
        *m_path_ = path;
}

Path::Path(const Path& other)
    : m_form_(other.m_form_)
    , m_type_(other.m_type_) {
    *m_path_ = *other.m_path_;
}

Path::Path(Path&& other) noexcept {
    Path::swap(other);
}

Path& Path::operator=(const Path& other) {
    if (this == std::addressof(other))
        return *this;
    *m_path_ = *other.m_path_;
    m_form_ = other.m_form_;
    m_type_ = other.m_type_;
    return *this;
}

Path& Path::operator=(Path&& other) noexcept {
    Path::swap(other);
    return *this;
}

void Path::swap(Path& other) {
    std::swap(*m_path_, *other.m_path_);
    std::swap(m_form_, other.m_form_);
    std::swap(m_type_, other.m_type_);
}

Path::operator std::string() const {
    return *m_path_;
}

Path& Path::operator+=(const Path& other) {
    if (m_form_ == EForm::unknown)
        detect_form();
    ensure_trailing_separator();
    std::vector<std::string> newparts;
    ul::str::tokenize_string(*other.m_path_, os_possible_separators, newparts);
    const std::string sep(get_separator_used_here());
    for (const std::string& part : newparts) {
        *m_path_ += part;
        *m_path_ += sep;
    }
    return *this;
}

std::string Path::get_folder_path() const {
    if (m_type_ != EType::is_file && m_type_ != EType::is_unknown)
        return {};
    if (is_empty() || (*m_path_)[m_path_->size() - 1] == get_separator_used_here()[0])
        return {};
    const size_t index = m_path_->find_last_of(get_separator_used_here());
    if (index == std::string::npos)
        return {};
    return m_path_->substr(0, index + 1);
}

std::string Path::get_file_name() const {
    if (m_type_ != EType::is_file && m_type_ != EType::is_unknown)
        return {};
    if (is_empty() || (*m_path_)[m_path_->size() - 1] == get_separator_used_here()[0])
        return {};
    const size_t index = m_path_->find_last_of(get_separator_used_here());
    if (index == std::string::npos)
        return {};
    return m_path_->substr(index + 1);
}

std::string Path::get_extension(bool with_dot) const {
    if (m_type_ != EType::is_file && m_type_ != EType::is_unknown)
        return {};
    const size_t index = m_path_->find_last_of('.');
    // no ext || hidden file (starts with dot) || filename ends with a dot
    if (index == std::string::npos || index == 0 || index == m_path_->size() - 1)
        return {};
    if (with_dot)
        return std::string(".") + m_path_->substr(index + 1);
    return m_path_->substr(index + 1);
}

bool Path::is_absolute() const {
    if (is_empty())
        return false;
#if UL_OS_WINDOWS
    if (m_path->size() < 2)
        return false;
    else
        return (*m_path)[1] == ':';
#else
    return (*m_path_)[0] == get_separator_platform_indep()[0];
#endif
}

bool Path::is_empty() const {
    return m_path_->empty();
}

Path& Path::cleanup_native() {
#if UL_OS_WINDOWS
    std::replace(m_path->begin(), m_path->end(), '/', '\\');
#else
    std::replace(m_path_->begin(), m_path_->end(), '\\', '/');
#endif
    m_form_ = EForm::native;
    return *this;
}

Path& Path::cleanup_platform_indep() {
    std::replace(m_path_->begin(), m_path_->end(), '\\', folder_separator_to_use_here[0]);
    m_form_ = EForm::platformindependent;
    return *this;
}

Path& Path::ensure_trailing_separator() {
    return ensure_trailing_separator(m_form_ == EForm::native);
}

Path& Path::ensure_trailing_separator(bool native) {
    if (m_path_->empty() || m_type_ == EType::is_file || m_type_ == EType::is_link)
        return *this;
    std::string sep_to_use = folder_separator_to_use_here;
    if (native)
        sep_to_use = os_folder_separator;
    UL_ASSERT(!sep_to_use.empty());
    if (m_path_->back() != sep_to_use[0])
        *m_path_ += sep_to_use;
    return *this;
}

const std::string& Path::get_separator_used_here() const {
    if (m_form_ == EForm::unknown)
        detect_form();
    return m_form_ == EForm::native ? get_separator_native() : get_separator_platform_indep();
}

const std::string& Path::get_separator_native() {
    UL_ASSERT(!os_folder_separator.empty());
    return os_folder_separator;
}

const std::string& Path::get_separator_platform_indep() {
    UL_ASSERT(!folder_separator_to_use_here.empty());
    return folder_separator_to_use_here;
}

void Path::detect_form() const {
    const size_t pos = m_path_->find_first_of(os_possible_separators);
    if (pos != std::string::npos) {
        if ((*m_path_)[pos] == get_separator_platform_indep()[0])
            m_form_ = EForm::platformindependent;
        else
            m_form_ = EForm::native;
    } else
        m_form_ = EForm::platformindependent;
}
} // namespace mb::too::file
