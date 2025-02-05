//! \file

#ifndef IMAGE_H_dhfgniv8h5oo87zu4587t546xt5
#define IMAGE_H_dhfgniv8h5oo87zu4587t546xt5

#include "../config.h"
#include "../math/geometry.h"
#include <string>
#include <string_view>

namespace mb::too::img {
using Pixels = long;
using PixelsMargins = too::math::MarginsImpl<Pixels>;

constexpr std::string_view fileext_bmp{".bmp"};
constexpr std::string_view fileext_jpg{".jpg"};
constexpr std::string_view fileext_png{".png"};
constexpr std::string_view fileext_svg{".svg"};
} // namespace mb::too::img

TOO_HEADER_END

#endif
