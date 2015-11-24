// Markus Borris, 2015
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef SCALE_H_INCL_enrhfu87n83464346ng
#define SCALE_H_INCL_enrhfu87n83464346ng

#include <utility>
#include <assert.h>


namespace too
{
namespace math
{

template <typename FloatingPointType>
class Map_LinearScale_Interval_to_Interval
{
public:
    Map_LinearScale_Interval_to_Interval(const std::pair<FloatingPointType, FloatingPointType>& FromInterval,
                                         const std::pair<FloatingPointType, FloatingPointType>& ToInterval)
        : m_FromInterval(FromInterval)
        , m_ToInterval(ToInterval)
    {
        assert(FromInterval.first < FromInterval.second);
        assert(ToInterval.first < ToInterval.second);
    }

    double operator()(const double& x)
    {
        return (x - m_FromInterval.first) * (m_ToInterval.second - m_ToInterval.first) / (m_FromInterval.second - m_FromInterval.first);
    }

private:
    std::pair<FloatingPointType, FloatingPointType> m_FromInterval;
    std::pair<FloatingPointType, FloatingPointType> m_ToInterval;
};

}
}

#endif
