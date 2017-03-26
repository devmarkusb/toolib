// Markus Borris, 2016-17
// This file is part of Toolib library.

//!
/**
*/
//! \file

#ifndef LANGUAGES_H_sjdkghfnxz34gf7328f7gfr
#define LANGUAGES_H_sjdkghfnxz34gf7328f7gfr

#include "Toolib/assert.h"
#include "Toolib/error.h"
#include "Toolib/std/std_extensions.h"
#include <string>
#include <vector>


namespace too
{
namespace str
{
//! Language id type per ISO 639-1 plus an auto/systems choice.
using LangID_iso639_1 = std::string;

const LangID_iso639_1 auto_systems_language{"auto"};


//! Note: if you add to this, don't forget about the other functions in this file.
inline const std::vector<LangID_iso639_1>& getAllTheoreticallyPossibleLangIDs()
{
    static const std::vector<LangID_iso639_1> cachedval{
        "ar", // Arabic
        "bn", // Bengali, Bangla
        "de", // German
        "en", // English
        "es", // Spanish
        "fr", // French
        "hi", // Hindi
        "it", // Italian
        "jp", // Japanese
        "ko", // Korean
        "pt", // Portuguese
        "ru", // Russian
        "zh", // Chinese
    };
    return cachedval;
}

inline std::string getNativeLanguageName(const LangID_iso639_1&)
{
    throw too::not_implemented{"getNativeLanguageName"};
}

inline std::string getEnglishLanguageName(const LangID_iso639_1& id)
{
    if (id == auto_systems_language)
    {
        // that's not what you might want; please handle the translation for that word yourself
        // (on library level I can't decide, whether you want 'auto' or 'systems default' or whatever)
        TOO_ASSERT_THROW(false);
    }
    else if (id == "ar")
        return "Arabic";
    else if (id == "bn")
        return "Bengali";
    else if (id == "de")
        return "German";
    else if (id == "en")
        return "English";
    else if (id == "es")
        return "Spanish";
    else if (id == "fr")
        return "French";
    else if (id == "hi")
        return "Hindi";
    else if (id == "it")
        return "Italian";
    else if (id == "jp")
        return "Japanese";
    else if (id == "ko")
        return "Korean";
    else if (id == "pt")
        return "Portuguese";
    else if (id == "ru")
        return "Russian";
    else if (id == "zh")
        return "Chinese";
    else
        throw too::not_implemented{"unsupported or unknonwn ISO 639-1 language code"};
}
} // str
} // too

#endif
