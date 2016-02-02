// Markus Borris, 2015-16
// This file is part of Toolib library.

//!
/**
*/
//! \file

#ifndef GRAPH_H_INCL_lnkjgngkvfvutzhirthczrec5
#define GRAPH_H_INCL_lnkjgngkvfvutzhirthczrec5

#include <vector>
#include <utility>
#include <memory>
#include "Toolib/math/quantity_unit.h"
#include "Toolib/math/scale.h"
#include "Toolib/optional.h"
#include "Toolib/ptr.h"
#include "Toolib/error.h"
#include "ToolibDEF.h"


namespace too
{
namespace math
{

//!
template <typename X, typename Y>
using VectorOfPairs = std::vector<std::pair<X, Y>>;

//!
template <typename X, typename Y>
using PairOfVectors = std::pair<std::vector<X>, std::vector<Y>>;

//! This is understood to be the type for values referring to the position along an axis 'on paper'.
//! That means, if your axis ranges from 0 to 500 such units, this could correspond to physical values
//! from 0 to 10 Newton or whatever. There needs to be a distinction between physical values (expressed
//! via quantity/unit) and the projection values onto a chart screen/paper.
//! We use the notions physical value or quantity value synonymously.
using ProjectionValue = double;

//!
struct ChartAxis_setup
{
    //! If not provided, the maximum count of scale ticks on the axis is chosen automatically.
    too::opt<ScaleTickCount> max_tick_count;
    std::pair<ProjectionValue, ProjectionValue> projection_range;
};

//!
template <typename QuValueType>
class ChartAxis
{
public:
    ChartAxis(const ChartAxis_setup& setup, const Quantity& quantity, const std::vector<QuValueType>* qu_values);
    ChartAxis(const ChartAxis_setup& setup, const Quantity& quantity, const QuValueType& min_qu_val,
        const QuValueType& max_qu_val);

    ProjectionValue quvalue_to_projection(QuValueType x) const;
    QuValueType projection_to_quvalue(ProjectionValue p) const;

    ProjectionValue getProjectionMinVal() const { return this->setup.projection_range.first; }
    ProjectionValue getProjectionMaxVal() const { return this->setup.projection_range.second; }

    ScaleTickCount getTickCount() const { return this->tick_count; }

    Quantity getQuantity() const { return this->quantity; }
    QuValueType getTickStartVal() const { return this->tick_start_qu_val; }
    QuValueType getTickEndVal() const { return this->tick_end_qu_val; }
    QuValueType getTickStepVal() const { return this->tick_step_qu_val; }

private:
    const std::vector<QuValueType>* values;
    QuValueType minVal{};
    QuValueType maxVal{};
    ChartAxis_setup setup;
    Quantity quantity;
    ScaleTickCount tick_count{};
    using QuValue_to_Projection = Map_LinearScale_Interval_to_Interval<QuValueType>;
    std::unique_ptr<QuValue_to_Projection> map_quvalue_to_projection;
    QuValueType tick_step_qu_val{};
    QuValueType tick_start_qu_val{};
    QuValueType tick_end_qu_val{};

    void calcScaling();
};


//!
template <typename QuValueTypeX, typename QuValueTypeY>
class Chart2D
{
public:
    using QuValueTypeX_ = QuValueTypeX;
    using QuValueTypeY_ = QuValueTypeY;

    //! Be aware of providing \param qu_values with sufficient live-time around
    //! instances of this class. This is designed to save a copy step.
    Chart2D(const ChartAxis_setup& setupX, const ChartAxis_setup& setupY,
        const std::pair<Quantity, Quantity>& quantitiesXY, const VectorOfPairs<QuValueTypeX, QuValueTypeY>* qu_values);

    too::not_null<ChartAxis<QuValueTypeX>*> get_x_axis() const { return x_axis.get(); }
    too::not_null<ChartAxis<QuValueTypeX>*> get_y_axis() const { return y_axis.get(); }

    const VectorOfPairs<QuValueTypeX, QuValueTypeY>* getValues() const { return this->values; }

    void auto_optimize_scaling() { /*throw too::not_implemented("Chart2D::auto_optimize_scaling");*/}

private:
    const VectorOfPairs<QuValueTypeX, QuValueTypeY>* values;
    std::unique_ptr<ChartAxis<QuValueTypeX>> x_axis;
    std::unique_ptr<ChartAxis<QuValueTypeY>> y_axis;
};
}
}

//##########################################################################################################################
// template implementations

//#include "detail/graph.hpp"

#include "Toolib/math/scale.h"
#include "Toolib/math/round.h"
#include "Toolib/std/std_extensions.h"


namespace too
{
namespace math
{

template <typename QuValueType>
ChartAxis<QuValueType>::ChartAxis(
    const ChartAxis_setup& setup, const Quantity& quantity, const std::vector<QuValueType>* qu_values)
    : values(values), setup(setup), quantity(quantity)
{
    auto minmax = std::make_pair(QuValueType(), QuValueType());
    if (qu_values && !qu_values->empty())
        minmax   = std::minmax_element(std::begin(*qu_values), std::end(*qu_values));
    this->minVal = minmax.first;
    this->maxVal = minmax.second;

    calcScaling();
}

template <typename QuValueType>
ChartAxis<QuValueType>::ChartAxis(const ChartAxis_setup& setup, const Quantity& quantity, const QuValueType& min_qu_val,
    const QuValueType& max_qu_val)
    : minVal(min_qu_val), maxVal(max_qu_val), setup(setup), quantity(quantity)
{
    calcScaling();
}

template <typename QuValueType>
void ChartAxis<QuValueType>::calcScaling()
{
    this->tick_count = setup.max_tick_count ? *setup.max_tick_count : 11;

    this->tick_step_qu_val  = calcNiceScaleTick(maxVal - minVal, this->tick_count);
    this->tick_start_qu_val = 0.0;
    this->tick_end_qu_val = 0.0;
    std::tie(tick_start_qu_val, tick_end_qu_val) = calcScaleTickFromTo(minVal, maxVal, tick_step_qu_val);
    this->tick_count = round_to<ScaleTickCount>((tick_end_qu_val - tick_start_qu_val) / tick_step_qu_val);

    this->map_quvalue_to_projection =
        std::make_unique<QuValue_to_Projection>(std::make_pair(tick_start_qu_val, tick_end_qu_val),
            std::make_pair(setup.projection_range.first, setup.projection_range.second));
}

template <typename QuValueType>
ProjectionValue ChartAxis<QuValueType>::quvalue_to_projection(QuValueType x) const
{
    TOO_EXPECT_THROW(this->map_quvalue_to_projection);
    const QuValue_to_Projection& q2p = *this->map_quvalue_to_projection;
    return q2p(x);
}

template <typename QuValueType>
QuValueType ChartAxis<QuValueType>::projection_to_quvalue(ProjectionValue p) const
{
    TOO_EXPECT_THROW(this->map_quvalue_to_projection);
    const QuValue_to_Projection& q2p = *this->map_quvalue_to_projection;
    return q2p.inverse(p);
}


//##########################################################################################################################

template <typename QuValueTypeX, typename QuValueTypeY>
Chart2D<QuValueTypeX, QuValueTypeY>::Chart2D(const ChartAxis_setup& setupX, const ChartAxis_setup& setupY,
    const std::pair<Quantity, Quantity>& quantitiesXY, const VectorOfPairs<QuValueTypeX, QuValueTypeY>* qu_values)
    : values(qu_values)
{
    using QuValueXY = std::pair<QuValueTypeX, QuValueTypeY>;
    auto minmax_X   = std::make_pair(QuValueTypeX(), QuValueTypeX());
    auto minmax_Y = std::make_pair(QuValueTypeY(), QuValueTypeY());
    if (qu_values && !qu_values->empty())
    {
        const auto minmax_X_pair = std::minmax_element(std::begin(*qu_values), std::end(*qu_values),
            [](const QuValueXY& lhs, const QuValueXY& rhs)
            {
                return lhs.first < rhs.first;
            });
        const auto minmax_Y_pair = std::minmax_element(std::begin(*qu_values), std::end(*qu_values),
            [](const QuValueXY& lhs, const QuValueXY& rhs)
            {
                return lhs.second < rhs.second;
            });

        minmax_X = {(*minmax_X_pair.first).first, (*minmax_X_pair.second).first};
        minmax_Y = {(*minmax_Y_pair.first).second, (*minmax_Y_pair.second).second};
    }

    this->x_axis =
        std::make_unique<ChartAxis<QuValueTypeX>>(setupX, quantitiesXY.first, minmax_X.first, minmax_X.second);
    this->y_axis =
        std::make_unique<ChartAxis<QuValueTypeX>>(setupY, quantitiesXY.second, minmax_Y.first, minmax_Y.second);
}
}
}

#endif
