#ifndef CONSTS_H
#define CONSTS_H

#include "_gui/interface/globals_gui.h"
#include "Toolib/assert.h"
#include "Toolib/filesys/path.h"
#include <string>


namespace consts
{

const std::string LOG_FILEEXT           = ".log";
const std::string BACKUP_FILEEXT_SUFFIX = "#bak";
const std::string TRANSLATIONS_SUBDIR   = "translations";


inline std::string PROGRAM_EXE_DIR()
{
    std::string Dir;
    if (!globals::filesys().GetSystemPath(uiw::IFileSys::ESysPathType::PROGRAM, Dir, true))
    {
        TOO_ASSERT(false);
        return {};
    }
    return Dir;
}

inline std::string EXE_TRANSLATIONS_DIR() { return PROGRAM_EXE_DIR() + TRANSLATIONS_SUBDIR; }

inline const std::string& PROGRAM_NAME_TECHNICAL()
{
    static const std::string ret("WealthPlanner");
    return ret;
}

inline const std::string& PROGRAM_SUBDIR() { return PROGRAM_NAME_TECHNICAL(); }

inline bool ExtendBaseDirByProgSubdir_EnsureExistence(std::string& Dir, bool WithTrailingSeparator = true)
{
    using too::file::CPath;
    CPath Dir_helper(Dir, CPath::EForm::PLATFORMINDEPENDENT, CPath::EType::IS_FOLDER);
    Dir_helper.ensureTrailingSeparator();
    Dir = Dir_helper;
    Dir += PROGRAM_SUBDIR();
    if (WithTrailingSeparator)
        Dir += uiw::IFileSys::FOLDER_SEPARATOR_TO_USE_HERE;
    if (!globals::filesys().FolderExists(Dir))
    {
        if (!globals::filesys().CreateFolder(Dir))
            return false;
    }
    return true;
}

inline std::string PROGRAM_LOG_FILEPATHNAMEEXT()
{
    std::string Dir;
    if (!globals::filesys().GetSystemPath(uiw::IFileSys::ESysPathType::PROGDATA, Dir, true))
    {
        TOO_ASSERT(false);
        return {};
    }
    if (!ExtendBaseDirByProgSubdir_EnsureExistence(Dir))
    {
        TOO_ASSERT(false);
        return {};
    }
    return Dir + PROGRAM_NAME_TECHNICAL() + LOG_FILEEXT;
}
}

#endif
