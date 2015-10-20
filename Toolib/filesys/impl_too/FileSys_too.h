// Markus Borris, 2015
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef FILESYS_TOO_H_lkgjhmobi5tjvhm5jgch5968ch6
#define FILESYS_TOO_H_lkgjhmobi5tjvhm5jgch5968ch6

#include "../filesys.h"
#include "../../../ToolibDEF.h"


#if TOO_OS_WINDOWS == 1
#undef CopyFile
#undef DeleteFile
#endif


namespace too
{
namespace impl
{

class TOOLIBSHARED_EXPORT CFileSys_too : public too::IFileSys
{
public:
    virtual bool SaveToTextFile(const too::string& FilePathNameExt, const too::string& Content) override;
    virtual bool LoadFromTextFile(const too::string& FilePathNameExt, too::string& Content) override;
    virtual bool CopyFile(const too::string& FilePathNameExt_From, const too::string& FilePathNameExt_To) override;
    virtual bool DeleteFile(const too::string& FilePathNameExt) override;
    virtual bool RenameFile(const too::string& FilePathNameExt_From, const too::string& FilePathNameExt_To) override;
    virtual bool CreateFolder(const too::string& FolderPath) override;
    virtual bool DeleteFolder(const too::string& FolderPath) override;
    virtual bool RenameFolder(const too::string& FolderPath_From, const too::string& FolderPath_To) override;
    virtual bool FolderExists(const too::string& FolderPath) override;
    virtual bool FileExists(const too::string& FilePathNameExt) override;

    virtual too::string toNativeSeparators(const too::string& Path) override;

    virtual bool GetSystemPath(ESysPathType Type, too::string& Path, bool WithTrailingSeperator) override;
};

}
}

#endif
