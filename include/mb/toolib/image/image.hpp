//! \file

#ifndef IMAGE_HPP_dhfgniv8h5oo87zu4587t546xt5
#define IMAGE_HPP_dhfgniv8h5oo87zu4587t546xt5

#include "../config.hpp"
#include "../math/geometry.hpp"
#include <string>
#include <string_view>

namespace mb::too::img {
using Pixels = long;
using PixelsMargins = mb::too::math::MarginsImpl<Pixels>;

constexpr std::string_view fileext_bmp{".bmp"};
constexpr std::string_view fileext_jpg{".jpg"};
constexpr std::string_view fileext_png{".png"};
constexpr std::string_view fileext_svg{".svg"};
} // namespace mb::too::img


#endif
