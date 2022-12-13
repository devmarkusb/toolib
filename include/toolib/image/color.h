// 2016

//! \file


#ifndef COLOR_H_sdfjlhjcge5zrxdgh85
#define COLOR_H_sdfjlhjcge5zrxdgh85

#include <string>


namespace mb::too
{
namespace img
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

    double r() const
    {
        return r_;
    }
    double g() const
    {
        return g_;
    }
    double b() const
    {
        return b_;
    }
    double a() const
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

    int r() const
    {
        return r_;
    }
    int g() const
    {
        return g_;
    }
    int b() const
    {
        return b_;
    }
    int a() const
    {
        return a_;
    }

private:
    int r_{};
    int g_{};
    int b_{};
    int a_{};
};
} // namespace img
} // namespace mb::too

#endif
