// 2015

//! \file

#include "toolib/filesys/path.h"
#include "toolib/string/string_token.h"
#include "ul/ul.h"
#include <algorithm>
#include <fstream>

namespace
{
const std::string OS_POSSIBLE_SEPARATORS = "/\\";
#if UL_OS_WINDOWS
const std::string OS_FOLDER_SEPARATOR = "\\";
#else
const std::string OS_FOLDER_SEPARATOR = "/";
#endif
} // namespace

namespace mb::too
{
namespace file
{
const std::string Path::FOLDER_SEPARATOR_TO_USE_HERE = "/";

Path::Path(const std::string& path, EForm form, EType type)
    : m_form(form)
    , m_type(type)
{
    *m_path = path;
}

Path::Path(std::string& path, bool useByReference, EForm form, EType type)
    : m_form(form)
    , m_type(type)
{
    if (useByReference)
        m_path = &path;
    else
        *m_path = path;
}

Path::Path(const Path& other)
{
    *m_path = *other.m_path;
    m_form = other.m_form;
    m_type = other.m_type;
}

Path::Path(Path&& other) noexcept
{
    Path::swap(other);
}

Path& Path::operator=(const Path& other)
{
    *m_path = *other.m_path;
    m_form = other.m_form;
    m_type = other.m_type;
    return *this;
}

Path& Path::operator=(Path&& other) noexcept
{
    Path::swap(other);
    return *this;
}

void Path::swap(Path& other)
{
    std::swap(*m_path, *other.m_path);
    std::swap(m_form, other.m_form);
    std::swap(m_type, other.m_type);
}

Path::operator std::string() const
{
    return *m_path;
}

Path& Path::operator+=(const Path& other)
{
    if (m_form == EForm::UNKNOWN)
        detectForm();
    ensureTrailingSeparator();
    std::vector<std::string> newparts;
    too::str::tokenizeString(*other.m_path, OS_POSSIBLE_SEPARATORS, newparts);
    const std::string sep(getSeparatorUsedHere());
    for (const std::string& part : newparts)
    {
        *m_path += part;
        *m_path += sep;
    }
    return *this;
}

std::string Path::getFolderPath() const
{
    if (m_type != EType::IS_FILE && m_type != EType::IS_UNKNOWN)
        return std::string();
    if (isEmpty() || (*m_path)[m_path->size() - 1] == getSeparatorUsedHere()[0])
        return std::string();
    size_t index = m_path->find_last_of(getSeparatorUsedHere());
    if (index == std::string::npos)
        return std::string();
    return m_path->substr(0, index + 1);
}

std::string Path::getFileName() const
{
    if (m_type != EType::IS_FILE && m_type != EType::IS_UNKNOWN)
        return std::string();
    if (isEmpty() || (*m_path)[m_path->size() - 1] == getSeparatorUsedHere()[0])
        return std::string();
    size_t index = m_path->find_last_of(getSeparatorUsedHere());
    if (index == std::string::npos)
        return std::string();
    return m_path->substr(index + 1);
}

std::string Path::getExtension(bool with_dot) const
{
    if (m_type != EType::IS_FILE && m_type != EType::IS_UNKNOWN)
        return std::string();
    size_t index = m_path->find_last_of('.');
    // no ext || hidden file (starts with dot) || filename ends with a dot
    if (index == std::string::npos || index == 0 || index == m_path->size() - 1)
        return std::string();
    if (with_dot)
        return std::string(".") + m_path->substr(index + 1);
    else
        return m_path->substr(index + 1);
}

bool Path::isAbsolute() const
{
    if (isEmpty())
        return false;
#if UL_OS_WINDOWS
    if (m_path->size() < 2)
        return false;
    else
        return (*m_path)[1] == ':';
#else
    return (*m_path)[0] == getSeparatorPlatformIndep()[0];
#endif
}

bool Path::isEmpty() const
{
    return m_path->empty();
}

Path& Path::cleanupNative()
{
#if UL_OS_WINDOWS
    std::replace(m_path->begin(), m_path->end(), '/', '\\');
#else
    std::replace(m_path->begin(), m_path->end(), '\\', '/');
#endif
    m_form = EForm::NATIVE;
    return *this;
}

Path& Path::cleanupPlatformIndep()
{
    std::replace(m_path->begin(), m_path->end(), '\\', FOLDER_SEPARATOR_TO_USE_HERE[0]);
    m_form = EForm::PLATFORMINDEPENDENT;
    return *this;
}

Path& Path::ensureTrailingSeparator()
{
    return ensureTrailingSeparator(m_form == EForm::NATIVE);
}

Path& Path::ensureTrailingSeparator(bool native)
{
    if (m_path->empty() || m_type == EType::IS_FILE || m_type == EType::IS_LINK)
        return *this;
    std::string SepToUse = FOLDER_SEPARATOR_TO_USE_HERE;
    if (native)
        SepToUse = OS_FOLDER_SEPARATOR;
    UL_ASSERT(!SepToUse.empty());
    if (m_path->back() != SepToUse[0])
        *m_path += SepToUse;
    return *this;
}

const std::string& Path::getSeparatorUsedHere() const
{
    if (m_form == EForm::UNKNOWN)
        detectForm();
    return m_form == EForm::NATIVE ? getSeparatorNative() : getSeparatorPlatformIndep();
}

const std::string& Path::getSeparatorNative()
{
    UL_ASSERT(!OS_FOLDER_SEPARATOR.empty());
    return OS_FOLDER_SEPARATOR;
}

const std::string& Path::getSeparatorPlatformIndep()
{
    UL_ASSERT(!FOLDER_SEPARATOR_TO_USE_HERE.empty());
    return FOLDER_SEPARATOR_TO_USE_HERE;
}

void Path::detectForm() const
{
    size_t pos = m_path->find_first_of(OS_POSSIBLE_SEPARATORS);
    if (pos != std::string::npos)
    {
        if ((*m_path)[pos] == getSeparatorPlatformIndep()[0])
            m_form = EForm::PLATFORMINDEPENDENT;
        else
            m_form = EForm::NATIVE;
    }
    else
        m_form = EForm::PLATFORMINDEPENDENT;
}
} // namespace file
} // namespace mb::too
