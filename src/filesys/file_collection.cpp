//! \file

#include "toolib/filesys/file_collection.h"
#include "ul/ul.h"
#include <fstream>

#if UL_OS_ANDROID && UL_ANDROID_NDK_MAJOR < 22
#include "toolib/filesys/path.h"
#endif

namespace mb::too::file {
std::string FileCollection::get_base_name(const std::string& fn) {
    ul::std_fs::path p{fn};
#if UL_OS_ANDROID && UL_ANDROID_NDK_MAJOR < 22
    std::string fn_noext{fn};
    remove_extension(fn_noext);
    p = fn_noext;
#else
    p.replace_extension();
#endif
    std::string ret = p.string();
    size_t pos = ret.find_last_not_of("0123456789");
    if (pos == std::string::npos)
        return ret;
    std::string retsub = ret.substr(0, pos + 1);
    return retsub.empty() ? ret : retsub;
}

FileCollection::FileCollection(const std::string& file_name) {
    ul::std_fs::path p{file_name};
#if UL_OS_ANDROID && UL_ANDROID_NDK_MAJOR < 22
    std::string file_ext{Path{p.string()}.getExtension()};
#else
    std::string file_ext{p.extension().string()};
#endif
    std::string base_file_name{get_base_name(file_name)};
    std::string fn{base_file_name + file_ext};
    std::ifstream f(fn);
    if (f.good())
        this->file_list.push_back(fn);

    f.close();
    const unsigned char digits = obtain_number_of_digits_for_filenames_of_file_collection(base_file_name, file_ext);
    if (!digits) {
        this->file_list.push_back(file_name);
        return;
    }
    std::string file_nr_str;
    UL_PRAGMA_WARNINGS_PUSH
    UL_WARNING_DISABLE_CLANG(comma)
    for (unsigned int file_nr = 0; file_nr_str = ul::math::toLeadingZeros(file_nr, digits),
                      fn = std::string{base_file_name}.append(file_nr_str).append(file_ext), f.open(fn), f.good();
         ++file_nr, f.close())
        UL_PRAGMA_WARNINGS_POP {
            this->file_list.push_back(fn);
        }
}

std::vector<std::string> FileCollection::get_list_of_existent_files() const {
    return this->file_list;
}

unsigned char FileCollection::obtain_number_of_digits_for_filenames_of_file_collection(
    const std::string& base_file_name, const std::string& file_ext) {
    std::ifstream f;
    for (unsigned char digits = 1; digits < max_digits; ++digits) {
        std::string zeros{ul::math::toLeadingZeros(0, digits)};
        auto fn{std::string{base_file_name}.append(zeros).append(file_ext)};
        f.open(fn);
        if (f.good())
            return digits;
    }
    return 0;
}
} // namespace mb::too::file
