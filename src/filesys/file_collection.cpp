// Markus Borris, 2016
// This file is part of toolib library.

//!
/**
 */
//! \file

#include "toolib/filesys/file_collection.h"
#include "toolib/math/number.h"
#include "toolib/std/std_ext_filesystem.h"
#include "toolib/warnings.h"
#include <fstream>


namespace too
{

namespace file
{

std::string FileCollection::get_base_name(const std::string& fn)
{
    too::std_fs::path p{fn};
    p.replace_extension();
    std::string ret = p.string();
    size_t pos = ret.find_last_not_of("0123456789");
    if (pos == std::string::npos)
        return ret;
    std::string retsub = ret.substr(0, pos + 1);
    return retsub.empty() ? ret : retsub;
}

FileCollection::FileCollection(const std::string& file_name)
{
    too::std_fs::path p{file_name};
    std::string file_ext{p.extension().string()};
    std::string base_file_name{get_base_name(file_name)};
    std::string fn{base_file_name + file_ext};
    std::ifstream f(fn);
    if (f.good())
        this->file_list.push_back(fn);

    f.close();
    const unsigned char digits = obtain_number_of_digits_for_filenames_of_file_collection(base_file_name, file_ext);
    if (!digits)
    {
        this->file_list.push_back(file_name);
        return;
    }
    std::string file_nr_str;
    TOO_PRAGMA_WARNINGS_PUSH
    TOO_WARNING_DISABLE_CLANG(comma)
    for (unsigned int file_nr = 0; file_nr_str = too::math::toLeadingZeros(file_nr, digits),
                      fn = base_file_name + file_nr_str + file_ext, f.open(fn), f.good();
         ++file_nr, f.close())
        TOO_PRAGMA_WARNINGS_POP
        {
            this->file_list.push_back(fn);
        }
}

std::vector<std::string> FileCollection::get_list_of_existent_files() const
{
    return this->file_list;
}

unsigned char FileCollection::obtain_number_of_digits_for_filenames_of_file_collection(
    const std::string& base_file_name, const std::string& file_ext) const
{
    std::ifstream f;
    for (unsigned char digits = 1; digits < max_digits; ++digits)
    {
        std::string zeros{too::math::toLeadingZeros(0, digits)};
        std::string fn{base_file_name + zeros + file_ext};
        f.open(fn);
        if (f.good())
            return digits;
    }
    return 0;
}
} // namespace file
} // namespace too
