// 2016

//! \file


#ifndef COLOR_H_sdfjlhjcge5zrxdgh85
#define COLOR_H_sdfjlhjcge5zrxdgh85

#include "../config.h"
#include <string>


namespace mb::too::img
{
class Color_dbl
{
public:
    Color_dbl() = default;
    //! Params expected to range between 0.0 and 1.0 (inclusive).
    Color_dbl(double r, double g, double b, double a = 1.0)
        : r_{r}
        , g_{g}
        , b_{b}
        , a_{a}
    {
    }

    [[nodiscard]] double r() const
    {
        return r_;
    }
    [[nodiscard]] double g() const
    {
        return g_;
    }
    [[nodiscard]] double b() const
    {
        return b_;
    }
    [[nodiscard]] double a() const
    {
        return a_;
    }

private:
    double r_{};
    double g_{};
    double b_{};
    double a_{};
};

class Color
{
public:
    Color() = default;
    //! Params expected in range 0..255.
    Color(int r, int g, int b, int a = 1.0)
        : r_{r}
        , g_{g}
        , b_{b}
        , a_{a}
    {
    }

    [[nodiscard]] int r() const
    {
        return r_;
    }
    [[nodiscard]] int g() const
    {
        return g_;
    }
    [[nodiscard]] int b() const
    {
        return b_;
    }
    [[nodiscard]] int a() const
    {
        return a_;
    }

private:
    int r_{};
    int g_{};
    int b_{};
    int a_{};
};
} // namespace mb::too::img

TOO_HEADER_END

#endif
