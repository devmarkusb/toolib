// Markus Borris, 2015
// This file is part of Toolib library.

//!
/**
*/
//! \file

#include "Toolib/filesys/path.h"
#include "Toolib/PPDEFS.h"
#include "Toolib/string/string_token.h"
#include <assert.h>
#include <algorithm>

namespace
{
const std::string OS_POSSIBLE_SEPARATORS = "/\\";
#if TOO_OS_WINDOWS
const std::string OS_FOLDER_SEPARATOR = "\\";
#else
const std::string OS_FOLDER_SEPARATOR = "/";
#endif
}

using namespace too::file;

const std::string CPath::FOLDER_SEPARATOR_TO_USE_HERE = "/";

CPath::CPath(const std::string& path, EForm form, EType type) : m_form(form), m_type(type)
{
    *m_path = path;
}

CPath::CPath(std::string& path, bool useByReference, EForm form, EType type) : m_form(form), m_type(type)
{
    if (useByReference)
        m_path = &path;
    else
        *m_path = path;
}

CPath::CPath(const CPath& other)
{
    *m_path = *other.m_path;
    m_form  = other.m_form;
    m_type  = other.m_type;
}

CPath::CPath(CPath&& other)
{
    CPath::swap(other);
}

CPath& CPath::operator=(const CPath& other)
{
    *m_path = *other.m_path;
    m_form  = other.m_form;
    m_type  = other.m_type;
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

CPath::operator std::string() const
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
    const std::string sep(getSeparatorUsedHere());
    for (const std::string& part : newparts)
    {
        *m_path += part;
        *m_path += sep;
    }
    return *this;
}

std::string CPath::getFolderPath() const
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

std::string CPath::getFileName() const
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

std::string CPath::getExtension() const
{
    if (m_type != EType::IS_FILE && m_type != EType::IS_UNKNOWN)
        return std::string();
    size_t index = m_path->find_last_of('.');
    // no ext || hidden file (starts with dot) || filename ends with a dot
    if (index == std::string::npos || index == 0 || index == m_path->size() - 1)
        return std::string();
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
    std::string SepToUse = FOLDER_SEPARATOR_TO_USE_HERE;
    if (native)
        SepToUse = OS_FOLDER_SEPARATOR;
    assert(!SepToUse.empty());
    if (m_path->back() != SepToUse[0])
        *m_path += SepToUse;
    return *this;
}

const std::string& CPath::getSeparatorUsedHere() const
{
    if (m_form == EForm::UNKNOWN)
        detectForm();
    return m_form == EForm::NATIVE ? getSeparatorNative() : getSeparatorPlatformIndep();
}

const std::string& CPath::getSeparatorNative()
{
    assert(!OS_FOLDER_SEPARATOR.empty());
    return OS_FOLDER_SEPARATOR;
}

const std::string& CPath::getSeparatorPlatformIndep()
{
    assert(!FOLDER_SEPARATOR_TO_USE_HERE.empty());
    return FOLDER_SEPARATOR_TO_USE_HERE;
}

void CPath::detectForm() const
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
