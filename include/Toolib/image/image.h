// Markus Borris, 2016
// This file is part of Toolib library.

//!
/**
*/
//! \file


#ifndef IMAGE_H_dhfgniv8h5oo87zu4587t546xt5
#define IMAGE_H_dhfgniv8h5oo87zu4587t546xt5

#include "../math/geometry.h"
#include <string>


namespace too
{

namespace img
{

using Pixels        = long;
using PixelsMargins = too::math::Margins_<Pixels>;

const std::string fileext_bmp{".bmp"};
const std::string fileext_jpg{".jpg"};
const std::string fileext_png{".png"};
const std::string fileext_svg{".svg"};
}
}

#endif
