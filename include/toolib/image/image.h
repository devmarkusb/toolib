//! \file


#ifndef IMAGE_H_dhfgniv8h5oo87zu4587t546xt5
#define IMAGE_H_dhfgniv8h5oo87zu4587t546xt5

#include "../config.h"
#include "../math/geometry.h"
#include <string>

namespace mb::too::img {
using Pixels = long;
using PixelsMargins = too::math::MarginsImpl<Pixels>;

const std::string fileext_bmp{".bmp"};
const std::string fileext_jpg{".jpg"};
const std::string fileext_png{".png"};
const std::string fileext_svg{".svg"};
} // namespace mb::too::img

TOO_HEADER_END

#endif
