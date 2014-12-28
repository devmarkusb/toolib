// Markus Borris, 2014
// This file is part of my Toolib library. Open source.

//!
/** Please note: class CProgSettings is not suitable to be put in a lib; we get linker warnings due to moc content.
*/
//! \file

#include "../../../Toolib/programsettings/impl_Qt/ProgSettings_Qt.h"
#include "../../../Toolib/string/impl_Qt/StringConvert_Qt.h"
#define TOO_USE_BOOST   1
#include "../../../Toolib/string/string_delim.h"
#include "Toolib/mem/make_unique.h"


namespace too
{
namespace implQt
{

inline std::unique_ptr<QSettings>& CProgSettings::m_settings()
{
    if (!m_settings_impl_doNotUseItDirectlyExceptOnInit)
        SetError(EError::INIT_NOT_CALLED_OR_FAILED);
    return m_settings_impl_doNotUseItDirectlyExceptOnInit;
}

inline const std::unique_ptr<QSettings>& CProgSettings::m_settings() const
{
    if (!m_settings_impl_doNotUseItDirectlyExceptOnInit)
        SetError(EError::INIT_NOT_CALLED_OR_FAILED);
    return m_settings_impl_doNotUseItDirectlyExceptOnInit;
}

void CProgSettings::Init(const too::string& OrganizationName, const too::string& ApplicationName)
{
    m_settings_impl_doNotUseItDirectlyExceptOnInit
            = std::make_unique<QSettings>(
                QSettings::IniFormat, QSettings::UserScope,
                toos2qs(OrganizationName), toos2qs(ApplicationName));
    m_FirstOccurredError = EError::E_NO_ERROR;
    GetError();
}

CProgSettings::EError CProgSettings::GetError() const
{
    if (m_FirstOccurredError == EError::E_NO_ERROR)
    {
        if (m_settings())
        {
            if (m_settings()->status() == QSettings::FormatError)
                SetError(EError::ERROR_READING_SETTINGS);
            else if (m_settings()->status() == QSettings::AccessError)
                SetError(EError::ERROR_WRITING_SETTINGS);
        }
    }
    return m_FirstOccurredError;
}

void CProgSettings::setValue(const QString& SecAndKey, const QVariant& Value)
{
    if (!m_settings())
        return;
    m_settings()->setValue(SecAndKey, Value);
}

QVariant CProgSettings::value(const QString& SecAndKey, const QVariant& Default) const
{
    if (!m_settings())
        return QVariant();
    return m_settings()->value(SecAndKey, Default);
}

void CProgSettings::Sync()
{
    if (!m_settings())
        return;
    m_settings()->sync();
    GetError();
}

QString CProgSettings::CreateQtKeyName(const too::string& SectionName, const too::string& KeyName) const
{
    return toos2qs(SectionName + _TOOSTR("/") + KeyName);
}

void CProgSettings::Remove(const too::string& SectionName, const too::string& KeyName)
{
    if (!m_settings())
        return;
    m_settings()->remove(CreateQtKeyName(SectionName, KeyName));
    // here writing can occur asynchronously, so GetError() doesn't make sense before a call to Sync()
}

bool CProgSettings::Contains(const too::string& SectionName, const too::string& KeyName) const
{
    if (!m_settings())
        return false;
    bool ret = m_settings()->contains(CreateQtKeyName(SectionName, KeyName));
    GetError();
    return ret;
}

void CProgSettings::Clear()
{
    if (!m_settings())
        return;
    m_settings()->clear();
    // here writing can occur asynchronously, so GetError() doesn't make sense before a call to Sync()
}

std::vector<CProgSettings::TSectionKeyPair> CProgSettings::GetAllKeys() const
{
    std::vector<TSectionKeyPair> retkeys;
    if (!m_settings())
        return retkeys;
    QStringList keys(m_settings()->allKeys());
    retkeys.reserve(keys.size());
    for (auto i : keys)
    {
        std::vector<too::string> keypath;
        too::str::tokenizeString(qs2toos(i), _TOOSTR("/"), keypath);
        // only support one level of sections, so take the last two entries
        if (keypath.empty())
            continue;
        TSectionKeyPair SecKey;
        SecKey.second = keypath.back();
        if (keypath.size() >= 2)
            SecKey.first = keypath[keypath.size() - 2];
        retkeys.push_back(SecKey);
    }
    return retkeys;
}

class Convert_var2qvar_visitor : public boost::static_visitor<QVariant>
{
public:
    // template version leads to a bunch of compile errors; didn't think about it much, since the variant below worked
//    template<typename T>
//    QVariant operator()(const T& v) const
//    {
//        if (std::is_same<T,too::string>::value)
//            return QVariant(toos2qs(v));
//        else
//            return QVariant(v);
//    }
    QVariant operator()(const CProgSettings::TInteger& v) const { return QVariant(v); }
    QVariant operator()(const double& v) const { return QVariant(v); }
    QVariant operator()(const too::string& v) const { return QVariant(toos2qs(v)); }
    QVariant operator()(const bool& v) const { return QVariant(v); }
};

QVariant CProgSettings::var2qvar(const CProgSettings::TVariant& v) const
{
    return boost::apply_visitor(Convert_var2qvar_visitor(), v);
}

CProgSettings::TVariant CProgSettings::qvar2var(const QVariant& v) const
{
    bool isOK = true;
    TVariant ret;
    switch(v.type())
    {
    case QMetaType::Int:
        ret = v.toInt(&isOK);
        break;
    case QMetaType::Double:
        ret = v.toDouble(&isOK);
        break;
    case QMetaType::QString:
        ret = qs2toos(v.toString());
        break;
    case QMetaType::Bool:
        ret = v.toBool();
        break;
    default:
        isOK = false;
        break;
    }
    if (!isOK)
        SetError(EError::INTERNAL_ERROR__VARIANT_CONVERSION);
    return ret;
}

CProgSettings::TVariant CProgSettings::Value(const too::string& SectionName, const too::string& KeyName, const CProgSettings::TVariant& Default) const
{
    if (!m_settings())
        return TVariant();
    QVariant val(m_settings()->value(CreateQtKeyName(SectionName, KeyName), var2qvar(Default)));
    GetError();
    if (!val.isValid() || val.isNull())
        SetError(EError::ERROR_READING_SETTINGS);
    return qvar2var(val);
}

void CProgSettings::SetValue(const too::string& SectionName, const too::string& KeyName, const CProgSettings::TVariant& Value)
{
    if (!m_settings())
        return;
    m_settings()->setValue(CreateQtKeyName(SectionName, KeyName), var2qvar(Value));
    // here writing can occur asynchronously, so GetError() doesn't make sense before a call to Sync()
}

}
}
