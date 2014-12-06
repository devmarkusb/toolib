// Markus Borris, 2014
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef PROGSETTINGS_QT_H
#define PROGSETTINGS_QT_H

#include "../../iprogramsettings.h"
#include <QObject>
#include <QSettings>
#include <memory>
#include "../../class/non_copyable.h"


namespace implQt
{
class CProgSettings : public QObject, public too::IProgSettings, private too::non_copyable
{
    Q_OBJECT

public:
    virtual ~CProgSettings() = default;

    virtual void Init(const too::string& OrganizationName, const too::string& ApplicationName);

    virtual void SetValue(const too::string& SectionName, const too::string& KeyName, const TVariant& Value);
    virtual TVariant Value(const too::string& SectionName, const too::string& KeyName, const TVariant& Default = TVariant()) const;

    virtual std::vector<TSectionKeyPair> GetAllKeys() const;
    virtual void Clear();
    virtual bool Contains(const too::string& SectionName, const too::string& KeyName) const;
    virtual void Remove(const too::string& SectionName, const too::string& KeyName);
    virtual void Sync();

    virtual EError GetError() const;

    // ### QML access ###
    //! \param SecAndKey contains section and key like "sectionname/keyname" separated by "/"
    Q_INVOKABLE void setValue(const QString& SecAndKey, const QVariant& Value);
    Q_INVOKABLE QVariant value(const QString& SecAndKey, const QVariant& Default = QVariant()) const;

private:
    inline std::unique_ptr<QSettings>&  m_settings();
    inline const std::unique_ptr<QSettings>& m_settings() const;
    mutable EError                      m_FirstOccurredError{EError::INIT_NOT_CALLED_OR_FAILED};

    void SetError(EError e) const { if (m_FirstOccurredError == EError::NO_ERROR) m_FirstOccurredError = e; }
    QString CreateQtKeyName(const too::string& SectionName, const too::string& KeyName) const;
    QVariant var2qvar(const TVariant& v) const;
    TVariant qvar2var(const QVariant& v) const;

private:
    std::unique_ptr<QSettings>          m_settings_impl_doNotUseItDirectlyExceptOnInit{nullptr};
};
}

#endif // PROGSETTINGS_QT_H
