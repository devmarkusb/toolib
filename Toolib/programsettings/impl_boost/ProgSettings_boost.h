// Markus Borris, 2015
// This file is part of my Toolib library. Open source.

//!
/** Please note: class CProgSettings is not suitable to be put in a lib; we get linker warnings due to moc content.
*/
//! \file

#ifndef PROGSETTINGS_BOOST_H
#define PROGSETTINGS_BOOST_H

#include "../iprogramsettings.h"
#include <memory>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/info_parser.hpp>
#include "../../class/non_copyable.h"


namespace too
{
namespace implBoost
{
namespace boost_pt = boost::property_tree;

class CProgSettings : public too::IProgSettings
{
public:
    enum class StorageFileFormat
    {
        INI,
        XML,
        JSON,
        BOOST_INFO,
    };

    inline CProgSettings(const too::string& FileNamePath, const too::string& FileExt, StorageFileFormat PreferredStorageFileFormat = StorageFileFormat::JSON);
    virtual inline ~CProgSettings();

    virtual inline void Init(const too::string& OrganizationName, const too::string& ApplicationName) override;

    virtual inline void SetValue(const too::string& SectionName, const too::string& KeyName, const TVariant& Value) override;
    virtual inline TVariant Value(const too::string& SectionName, const too::string& KeyName, const TVariant& Default = TVariant()) const override;

    virtual inline std::vector<TSectionKeyPair> GetAllKeys() const override;
    virtual inline void Clear() override;
    virtual inline bool Contains(const too::string& SectionName, const too::string& KeyName) const override;
    virtual inline void Remove(const too::string& SectionName, const too::string& KeyName) override;
    virtual inline void Sync() override;

    virtual inline EError GetError() const override;
    virtual inline void ResetError() override;

private:
    mutable EError              m_FirstOccurredError{EError::INIT_NOT_CALLED_OR_FAILED};
    StorageFileFormat           m_StorageFileFormat{StorageFileFormat::JSON};
    too::string                 m_FileName;
    boost_pt::ptree             m_PropTree;

    void SetError(EError e) const { if (m_FirstOccurredError == EError::E_NO_ERROR) m_FirstOccurredError = e; }
    //! \param first and \param second mustn't start or end with HIERARCHY_SEPARATOR
    static inline too::string concatenateWithHierarchySep(const too::string& first, const too::string& second);
};


//##############################################################################################################

inline CProgSettings::CProgSettings(const too::string& FileNamePath, const too::string& FileExt, StorageFileFormat PreferredStorageFileFormat)
    : m_StorageFileFormat(PreferredStorageFileFormat)
{
    m_FileName = concatenateWithHierarchySep(FileNamePath, FileExt);
}

CProgSettings::~CProgSettings()
{
    try { Sync(); } catch (...) {}
}

inline too::string CProgSettings::concatenateWithHierarchySep(const too::string& first, const too::string& second)
{
    too::string ret{first};
    ret+= HIERARCHY_SEPARATOR;
    ret+= second;
    return ret;
}

inline void CProgSettings::Init(const string&, const string&)
{
    try
    {
        switch (m_StorageFileFormat)
        {
        case StorageFileFormat::INI:
            boost::property_tree::read_ini(m_FileName, m_PropTree);
            break;
        case StorageFileFormat::XML:
            boost::property_tree::read_xml(m_FileName, m_PropTree);
            break;
        case StorageFileFormat::JSON:
            boost::property_tree::read_json(m_FileName, m_PropTree);
            break;
        case StorageFileFormat::BOOST_INFO:
            boost::property_tree::read_info(m_FileName, m_PropTree);
            break;
        default:
            return;
        }
    }
    catch (...)
    {
        SetError(EError::ERROR_READING_SETTINGS);
        return;
    }
    m_FirstOccurredError = EError::E_NO_ERROR;
}

inline void CProgSettings::SetValue(const string& SectionName, const string& KeyName, const IProgSettings::TVariant& Value)
{
    const too::string path{concatenateWithHierarchySep(SectionName, KeyName)};
    try { m_PropTree.put(path, Value); }
    catch (const boost_pt::ptree_bad_data&) { SetError(EError::INTERNAL_ERROR__VARIANT_CONVERSION); }
}

inline IProgSettings::TVariant CProgSettings::Value(const string& SectionName, const string& KeyName, const IProgSettings::TVariant& Default) const
{
    const too::string path{concatenateWithHierarchySep(SectionName, KeyName)};
    try { return m_PropTree.get<int>(path); }
    catch (const boost_pt::ptree_bad_path&) { return Default; }
    catch (const boost_pt::ptree_bad_data&) {}
    try { return m_PropTree.get<double>(path); }
    catch (const boost_pt::ptree_bad_data&) {}
    try { return m_PropTree.get<bool>(path); }
    catch (const boost_pt::ptree_bad_data&) {}
    try { return m_PropTree.get<too::string>(path); }
    catch (const boost_pt::ptree_bad_data&) { SetError(EError::INTERNAL_ERROR__VARIANT_CONVERSION); }
    return Default;
}

inline std::vector<IProgSettings::TSectionKeyPair> CProgSettings::GetAllKeys() const
{
    std::vector<IProgSettings::TSectionKeyPair> ret;
    for (const auto& path : m_PropTree)
        ret.push_back(std::make_pair(too::string(), path.first));
    return ret;
}

inline void CProgSettings::Clear()
{
    m_PropTree.clear();
}

inline bool CProgSettings::Contains(const string& SectionName, const string& KeyName) const
{
    const too::string path{concatenateWithHierarchySep(SectionName, KeyName)};
    return m_PropTree.find(path) != m_PropTree.not_found();
}

inline void CProgSettings::Remove(const string& SectionName, const string& KeyName)
{
    const too::string path{concatenateWithHierarchySep(SectionName, KeyName)};
    m_PropTree.erase(path);
}

inline void CProgSettings::Sync()
{
    try
    {
        switch (m_StorageFileFormat)
        {
        case StorageFileFormat::INI:
            boost::property_tree::write_ini(m_FileName, m_PropTree);
            break;
        case StorageFileFormat::XML:
            boost::property_tree::write_xml(m_FileName, m_PropTree);
            break;
        case StorageFileFormat::JSON:
            boost::property_tree::write_json(m_FileName, m_PropTree);
            break;
        case StorageFileFormat::BOOST_INFO:
            boost::property_tree::write_info(m_FileName, m_PropTree);
            break;
        default:
            return;
        }
    }
    catch (...)
    {
        SetError(EError::ERROR_WRITING_SETTINGS);
        return;
    }
}

inline IProgSettings::EError CProgSettings::GetError() const
{
    return m_FirstOccurredError;
}

inline void CProgSettings::ResetError()
{
    m_FirstOccurredError = EError::E_NO_ERROR;
}

}
}

#endif
