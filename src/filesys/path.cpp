// Markus Borris, 17.02.2015
#include "../../Toolib/filesys/path.h"
#include "../../Toolib/PPDEFS.h"
#include "../../Toolib/string/string_delim.h"
#include <assert.h>
#include <algorithm>

namespace
{
const too::string OS_POSSIBLE_SEPARATORS = _TOOSTR("/\\");
#if TOO_OS_WINDOWS
const too::string OS_FOLDER_SEPARATOR = _TOOSTR("\\");
#else
const too::string OS_FOLDER_SEPARATOR = _TOOSTR("/");
#endif
}

using namespace too::file;

const too::string CPath::FOLDER_SEPARATOR_TO_USE_HERE = _TOOSTR("/");

CPath::CPath(const too::string& path, EForm form, EType type)
    : m_form(form)
    , m_type(type)
{
    *m_path = path;
}

CPath::CPath(too::string& path, bool useByReference, EForm form, EType type)
    : m_form(form)
    , m_type(type)
{
    if (useByReference)
        m_path = &path;
    else
        *m_path = path;
}

CPath::CPath(const CPath& other)
{
    *m_path = *other.m_path;
    m_form = other.m_form;
    m_type = other.m_type;
}

CPath::CPath(CPath&& other)
{
    CPath::swap(other);
}

CPath& CPath::operator=(const CPath& other)
{
    *m_path = *other.m_path;
    m_form = other.m_form;
    m_type = other.m_type;
    return *this;
}

CPath& CPath::operator=(CPath&& other)
{
    CPath::swap(other);
    return *this;
}

void CPath::swap(CPath& other)
{
    std::swap(*m_path, *other.m_path);
    std::swap(m_form, other.m_form);
    std::swap(m_type, other.m_type);
}

CPath::operator too::string() const
{
    return *m_path;
}

CPath& CPath::operator+=(const CPath& other)
{
    if (m_form == EForm::UNKNOWN)
        detectForm();
    ensureTrailingSeparator();
    std::vector<std::string> newparts;
    too::str::tokenizeString(*other.m_path, OS_POSSIBLE_SEPARATORS, newparts);
    const too::string sep(getSeparatorUsedHere());
    for (const std::string& part : newparts)
    {
        *m_path+= part;
        *m_path+= sep;
    }
    return *this;
}

too::string CPath::getFolderPath() const
{
    if (m_type != EType::IS_FILE && m_type != EType::IS_UNKNOWN)
        return too::string();
    if (isEmpty() || (*m_path)[m_path->size() - 1] == getSeparatorUsedHere()[0])
        return too::string();
    size_t index = m_path->find_last_of(getSeparatorUsedHere());
    if (index == too::string::npos)
        return too::string();
    return m_path->substr(0, index + 1);
}

too::string CPath::getFileName() const
{
    if (m_type != EType::IS_FILE && m_type != EType::IS_UNKNOWN)
        return too::string();
    if (isEmpty() || (*m_path)[m_path->size() - 1] == getSeparatorUsedHere()[0])
        return too::string();
    size_t index = m_path->find_last_of(getSeparatorUsedHere());
    if (index == too::string::npos)
        return too::string();
    return m_path->substr(index + 1);
}

too::string CPath::getExtension() const
{
    if (m_type != EType::IS_FILE && m_type != EType::IS_UNKNOWN)
        return too::string();
    size_t index = m_path->find_last_of('.');
    // no ext || hidden file (starts with dot) || filename ends with a dot
    if (index == too::string::npos || index == 0 || index == m_path->size() - 1)
        return too::string();
    return m_path->substr(index + 1);
}

bool CPath::isAbsolute() const
{
    if (isEmpty())
        return false;
#if TOO_OS_WINDOWS
    if (m_path->size() < 2)
        return false;
    else
        return (*m_path)[1] == ':';
#else
    return (*m_path)[0] == getSeparatorPlatformIndep()[0];
#endif
}

bool CPath::isEmpty() const
{
    return m_path->empty();
}

CPath& CPath::cleanupNative()
{
#if TOO_OS_WINDOWS
    std::replace(m_path->begin(), m_path->end(), '/', '\\');
#else
    std::replace(m_path->begin(), m_path->end(), '\\', '/');
#endif
    m_form = EForm::NATIVE;
    return *this;
}

CPath& CPath::cleanupPlatformIndep()
{
    std::replace(m_path->begin(), m_path->end(), '\\', FOLDER_SEPARATOR_TO_USE_HERE[0]);
    m_form = EForm::PLATFORMINDEPENDENT;
    return *this;
}

CPath& CPath::ensureTrailingSeparator()
{
    return ensureTrailingSeparator(m_form == EForm::NATIVE);
}

CPath& CPath::ensureTrailingSeparator(bool native)
{
    if (m_path->empty() || m_type == EType::IS_FILE || m_type == EType::IS_LINK)
        return *this;
    too::string SepToUse = FOLDER_SEPARATOR_TO_USE_HERE;
    if (native)
        SepToUse = OS_FOLDER_SEPARATOR;
    assert(!SepToUse.empty());
    if (m_path->back() != SepToUse[0])
        *m_path+= SepToUse;
    return *this;
}

const too::string& CPath::getSeparatorUsedHere() const
{
    if (m_form == EForm::UNKNOWN)
        detectForm();
    return m_form == EForm::NATIVE ? getSeparatorNative() : getSeparatorPlatformIndep();
}

const too::string& CPath::getSeparatorNative()
{
    assert(!OS_FOLDER_SEPARATOR.empty());
    return OS_FOLDER_SEPARATOR;
}

const too::string& CPath::getSeparatorPlatformIndep()
{
    assert(!FOLDER_SEPARATOR_TO_USE_HERE.empty());
    return FOLDER_SEPARATOR_TO_USE_HERE;
}

void CPath::detectForm() const
{
    size_t pos = m_path->find_first_of(OS_POSSIBLE_SEPARATORS);
    if (pos != too::string::npos)
    {
        if ((*m_path)[pos] == getSeparatorPlatformIndep()[0])
            m_form = EForm::PLATFORMINDEPENDENT;
        else
            m_form = EForm::NATIVE;
    }
    else
        m_form = EForm::PLATFORMINDEPENDENT;
}
