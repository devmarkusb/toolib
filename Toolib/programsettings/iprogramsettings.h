// Markus Borris, 2014
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef IPROGRAMSETTINGS_H_skzhfisxfuxskydfnixusgfiwz
#define IPROGRAMSETTINGS_H_skzhfisxfuxskydfnixusgfiwz

#include <boost/variant.hpp>
#include <vector>
#include "../types.h"


namespace too
{
class IProgSettings
{
public:
    virtual ~IProgSettings() = default;

    static IProgSettings* GetInstance();
    virtual void Init(const too::string& OrganizationName, const too::string& ApplicationName) = 0;

    using TInteger = int;
    using TVariant = boost::variant<TInteger, double, too::string, bool>;

    virtual void SetValue(const too::string& SectionName, const too::string& KeyName, const TVariant& Value) = 0;
    void SetValue(const too::string& KeyName, const TVariant& Value) { SetValue(too::string(), KeyName, Value); }
    virtual TVariant Value(const too::string& SectionName, const too::string& KeyName, const TVariant& Default = TVariant()) const = 0;
    TVariant Value(const too::string& KeyName, const TVariant& Default = TVariant()) const { return Value(too::string(), KeyName, Default); }

    using TSectionKeyPair = std::pair<too::string, too::string>;
    virtual std::vector<TSectionKeyPair> GetAllKeys() const = 0;
    virtual void Clear() = 0;
    virtual bool Contains(const too::string& SectionName, const too::string& KeyName) const = 0;
    bool Contains(const too::string& KeyName) { return Contains(too::string(), KeyName); }
    virtual void Remove(const too::string& SectionName, const too::string& KeyName) = 0;
    void Remove(const too::string& KeyName) { Remove(too::string(), KeyName); }
    //! You need to call this only, if you want to know whether there occurred an error by GetError().
    /** It initiates a persistent writing to data store, which would nevertheless happen asynchronously.
        Perhaps most useful when writing a bunch of program data immediately before program exit.*/
    virtual void Sync() = 0;

    enum class EError
    {
        E_NO_ERROR = 0, // scary: NO_ERROR seems to be defined somewhere in system headers!?
        INIT_NOT_CALLED_OR_FAILED,
        ERROR_WRITING_SETTINGS,
        ERROR_READING_SETTINGS,
        INTERNAL_ERROR__VARIANT_CONVERSION,
    };
    virtual EError GetError() const = 0;
};
}

#endif
