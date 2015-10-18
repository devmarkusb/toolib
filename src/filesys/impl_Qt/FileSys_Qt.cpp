#include "../../../Toolib/filesys/impl_Qt/FileSys_Qt.h"
#include <memory>
#include <QFile>
#include <QDir>
#include <QCoreApplication>
#include "../../../Toolib/argsused.h"
#include "../../../Toolib/scope/scopeguard.h"
#include "../../../Toolib/string/impl_Qt/StringConvert_Qt.h"


namespace too
{
namespace implQt
{

bool CFileSys_Qt::SaveToTextFile(const too::string& FilePathNameExt, const too::string& Content)
{
    QFile f(toos2qs(FilePathNameExt));
    if (!f.open(QIODevice::WriteOnly))
        return false;
    too::ScopeGuard guard_close = too::MakeObjGuard(f, &QFile::close);
    too::ignore_arg(guard_close);
    too::static_assert_string_BitsPerChar_and_Encoding<too::string, 8>();
    if (f.write(Content.c_str()) == -1)
        return false;
    return true;
}

bool CFileSys_Qt::LoadFromTextFile(const too::string& FilePathNameExt, too::string& Content)
{
    QFile f(toos2qs(FilePathNameExt));
    if (!f.open(QIODevice::ReadOnly))
        return false;
    too::ScopeGuard guard_close = too::MakeObjGuard(f, &QFile::close);
    too::ignore_arg(guard_close);
    const qint64 size = f.bytesAvailable();
    std::unique_ptr<char[]> buffer(new char[size]);
    const qint64 size_read = f.read(buffer.get(), size);
    if (size_read == -1)
        return false;
    too::static_assert_string_BitsPerChar_and_Encoding<too::string, 8>();
    Content.reserve(size_read);
    Content.assign(buffer.get());
    return true;
}

bool CFileSys_Qt::CopyFile(const too::string& FilePathNameExt_From, const too::string& FilePathNameExt_To)
{
    return QFile::copy(toos2qs(FilePathNameExt_From), toos2qs(FilePathNameExt_To));
}

bool CFileSys_Qt::DeleteFile(const too::string& FilePathNameExt)
{
    return QFile::remove(toos2qs(FilePathNameExt));
}

bool CFileSys_Qt::RenameFile(const too::string& FilePathNameExt_From, const too::string& FilePathNameExt_To)
{
    return QFile::rename(toos2qs(FilePathNameExt_From), toos2qs(FilePathNameExt_To));
}

bool CFileSys_Qt::CreateFolder(const too::string& FolderPath)
{
    return QDir().mkpath(toos2qs(FolderPath));
}

bool CFileSys_Qt::DeleteFolder(const too::string& FolderPath)
{
    return QDir().rmpath(toos2qs(FolderPath));
}

bool CFileSys_Qt::RenameFolder(const too::string& FolderPath_From, const too::string& FolderPath_To)
{
    return QDir().rename(toos2qs(FolderPath_From), toos2qs(FolderPath_To));
}

bool CFileSys_Qt::FolderExists(const too::string& FolderPath)
{
    return QDir().exists(toos2qs(FolderPath));
}

bool CFileSys_Qt::FileExists(const too::string& FilePathNameExt)
{
    return QFile::exists(toos2qs(FilePathNameExt));
}

too::string CFileSys_Qt::toNativeSeparators(const too::string& Path)
{
    return qs2toos(QDir::toNativeSeparators(toos2qs(Path)));
}

bool CFileSys_Qt::GetSystemPath(too::file::IFileSys::ESysPathType Type, too::string& Path, bool WithTrailingSeperator)
{
    switch (Type)
    {
    case ESysPathType::PROGDATA:
        Path = qs2toos(QDir::home().absolutePath());
        break;
    case ESysPathType::PROGRAM:
        Path = qs2toos(QCoreApplication::applicationDirPath());
        break;
    case ESysPathType::ROOT:
        Path = qs2toos(QDir::root().absolutePath());
        break;
    case ESysPathType::TEMP:
        Path = qs2toos(QDir::temp().absolutePath());
        break;
    case ESysPathType::USER:
        Path = qs2toos(QDir::home().absolutePath());
        break;
    case ESysPathType::CURRENT:
    default:
        Path = qs2toos(QDir::current().absolutePath());
    }
    if (WithTrailingSeperator)
        Path+= FOLDER_SEPARATOR_TO_USE_HERE;
    return true;
}

}
}
