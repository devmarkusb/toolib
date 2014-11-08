// Markus Borris, 2014
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef MAKE_UNIQUE_H
#define MAKE_UNIQUE_H

//! File should be deprecated with C++14.

#include <memory>

#ifndef make_unique
namespace std
{
    template<typename T, typename ...Args>
    std::unique_ptr<T> make_unique(Args&& ...args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}
#endif

#endif // MAKE_UNIQUE_H
