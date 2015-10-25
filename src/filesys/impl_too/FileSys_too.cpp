#include "../../../Toolib/filesys/impl_too/FileSys_too.h"
#include <fstream>
#include <cstdio>
#include "../../../Toolib/argsused.h"


namespace too
{
namespace impl
{

bool CFileSys_too::SaveToTextFile(const too::string& FilePathNameExt, const too::string& Content)
{
    std::ofstream file(FilePathNameExt);
    if (!file)
        return false;
    file << Content;
    return file;
}

bool CFileSys_too::LoadFromTextFile(const too::string& FilePathNameExt, too::string& Content)
{
    std::ifstream file(FilePathNameExt);
    if (!file)
        return false;
    file.seekg(0, std::ios::end);
    if (!file)
        return false;
    const int size = file.tellg();
    if (!file || size == -1)
        return false;
    Content.resize(size);   // need the precise size for the string, I guess
    file.seekg(0);
    if (!file)
        return false;
    file.read(&Content[0], size);
    return file;
}

bool CFileSys_too::CopyFile(const too::string& FilePathNameExt_From, const too::string& FilePathNameExt_To)
{
    std::ifstream src(FilePathNameExt_From, std::ios::binary);
    if (!src)
        return false;
    std::ofstream dst(FilePathNameExt_To, std::ios::binary);
    if (!dst)
        return false;
    dst << src.rdbuf();
    return dst;
}

bool CFileSys_too::DeleteFile(const too::string& FilePathNameExt)
{    
    return !std::remove(FilePathNameExt.c_str());
}

bool CFileSys_too::RenameFile(const too::string& FilePathNameExt_From, const too::string& FilePathNameExt_To)
{
    too::ignore_arg(FilePathNameExt_From);
    too::ignore_arg(FilePathNameExt_To);
    return false;
}

bool CFileSys_too::CreateFolder(const too::string& FolderPath)
{
    too::ignore_arg(FolderPath);
    return false;
}

bool CFileSys_too::DeleteFolder(const too::string& FolderPath)
{
    too::ignore_arg(FolderPath);
    return false;
}

bool CFileSys_too::RenameFolder(const too::string& FolderPath_From, const too::string& FolderPath_To)
{
    too::ignore_arg(FolderPath_From);
    too::ignore_arg(FolderPath_To);
    return false;
}

bool CFileSys_too::FolderExists(const too::string& FolderPath)
{
    too::ignore_arg(FolderPath);
    return false;
}

bool CFileSys_too::FileExists(const too::string& FilePathNameExt)
{
    std::ifstream file(FilePathNameExt, std::ios_base::binary);
    return file;
}

too::string CFileSys_too::toNativeSeparators(const too::string& Path)
{
    too::ignore_arg(Path);
    return too::string();
}

bool CFileSys_too::GetSystemPath(too::file::IFileSys::ESysPathType Type, too::string& Path, bool WithTrailingSeperator)
{
    switch (Type)
    {
    case ESysPathType::PROGDATA:
        break;
    case ESysPathType::PROGRAM:
        break;
    case ESysPathType::ROOT:
        break;
    case ESysPathType::TEMP:
        break;
    case ESysPathType::USER:
        break;
    case ESysPathType::CURRENT:
    default:
        break;
    }
    if (WithTrailingSeperator)
        Path+= FOLDER_SEPARATOR_TO_USE_HERE;
    return false;
}

}
}
