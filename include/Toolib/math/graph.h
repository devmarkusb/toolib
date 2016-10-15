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
#include "Toolib\class\non_copyable.h"


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


//####################################################################################################################

//!
struct ChartAxis_setup
{
    virtual ~ChartAxis_setup() {}

    virtual std::unique_ptr<ChartAxis_setup> clone() const
    {
        return too::make_unique<ChartAxis_setup>(*this);
    }

    //! If not provided, the maximum count of scale ticks on the axis is chosen automatically.
    too::opt<ScaleTickCount> max_tick_count;
};

//!
struct ChartAxisProj_setup : public ChartAxis_setup
{
    virtual ~ChartAxisProj_setup() {}

    virtual std::unique_ptr<ChartAxis_setup> clone() const
    {
        return too::make_unique<ChartAxisProj_setup>(*this);
    }

    std::pair<ProjectionValue, ProjectionValue> projection_range;
};


//####################################################################################################################

//!
template <typename QuValueType>
class ChartAxis : private too::non_copyable
{
public:
    ChartAxis(const ChartAxis_setup& setup, const Quantity& quantity, const std::vector<QuValueType>* qu_values);
    ChartAxis(const ChartAxis_setup& setup, const Quantity& quantity, const QuValueType& min_qu_val,
        const QuValueType& max_qu_val);
    virtual ~ChartAxis()
    {
        TOO_EXPECT(this->setup);
    }

    ScaleTickCount getTickCount() const { return this->tick_count; }

    Quantity getQuantity() const { return this->quantity; }
    QuValueType getTickStartVal() const { return this->tick_start_qu_val; }
    QuValueType getTickEndVal() const { return this->tick_end_qu_val; }
    QuValueType getTickStepVal() const { return this->tick_step_qu_val; }

    bool contains_zero_tick() const { return this->tick_start_qu_val <= QuValueType{} && QuValueType{} <= this->tick_end_qu_val; }

private:
    Quantity quantity;
    const std::vector<QuValueType>* values;
protected:
    QuValueType minVal{};
    QuValueType maxVal{};
    std::unique_ptr<const ChartAxis_setup> setup;
    ScaleTickCount tick_count{};
    QuValueType tick_step_qu_val{};
    QuValueType tick_start_qu_val{};
    QuValueType tick_end_qu_val{};

    void ensure_non_zero_range();
};


//####################################################################################################################

//!
template <typename QuValueType>
class ChartAxisProj : public ChartAxis<QuValueType>
{
public:
    ChartAxisProj(const ChartAxisProj_setup& setup, const Quantity& quantity, const std::vector<QuValueType>* qu_values);
    ChartAxisProj(const ChartAxisProj_setup& setup, const Quantity& quantity, const QuValueType& min_qu_val,
        const QuValueType& max_qu_val);
    virtual ~ChartAxisProj() {}

    ProjectionValue quvalue_to_projection(QuValueType x) const;
    ProjectionValue qurange_to_projectionrange(QuValueType width) const;
    QuValueType projection_to_quvalue(ProjectionValue p) const;
    QuValueType projectionrange_to_qurange(ProjectionValue width) const;

    ProjectionValue getProjectionMinVal() const;
    ProjectionValue getProjectionMaxVal() const;

private:
    using QuValue_to_Projection = Map_LinearScale_Interval_to_Interval<QuValueType>;
    std::unique_ptr<QuValue_to_Projection> map_quvalue_to_projection;
    const too::math::ChartAxisProj_setup* setup{nullptr};

    void constrImpl();
    void calcScaling();
};


//####################################################################################################################

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

    too::not_null<const ChartAxis<QuValueTypeX>*> get_x_axis() const { return x_axis.get(); }
    too::not_null<const ChartAxis<QuValueTypeX>*> get_y_axis() const { return y_axis.get(); }

    const VectorOfPairs<QuValueTypeX, QuValueTypeY>* getValues() const { return this->values; }

    void auto_optimize_scaling() { throw too::not_implemented("Chart2D::auto_optimize_scaling"); }

private:
    const VectorOfPairs<QuValueTypeX, QuValueTypeY>* values;
    std::unique_ptr<ChartAxis<QuValueTypeX>> x_axis;
    std::unique_ptr<ChartAxis<QuValueTypeY>> y_axis;
};

} // math
} // too


//####################################################################################################################
// template implementations
//####################################################################################################################

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
    : quantity(quantity), values(values), setup(setup.clone())
{
    auto minmax = std::make_pair(QuValueType(), QuValueType());
    if (qu_values && !qu_values->empty())
        minmax   = std::minmax_element(std::begin(*qu_values), std::end(*qu_values));
    this->minVal = minmax.first;
    this->maxVal = minmax.second;
}

template <typename QuValueType>
ChartAxis<QuValueType>::ChartAxis(const ChartAxis_setup& setup, const Quantity& quantity, const QuValueType& min_qu_val,
    const QuValueType& max_qu_val)
    : quantity(quantity), minVal(min_qu_val), maxVal(max_qu_val), setup(setup.clone())
{
}

template <typename QuValueType>
void ChartAxis<QuValueType>::ensure_non_zero_range()
{
    if (this->minVal > this->maxVal)
        std::swap(this->minVal, this->maxVal);
    if (!too::math::almost_equal(this->minVal, this->maxVal))
        return;
    if (too::math::almost_equal(this->minVal, QuValueType()))
    {
        this->minVal = QuValueType();
        this->maxVal = QuValueType(1);
    }
    else
    {
        this->minVal -= QuValueType(1);
        this->maxVal += QuValueType(1);
    }
    TOO_ENSURE(!too::math::almost_equal(this->minVal, this->maxVal));
}


//####################################################################################################################

template <typename QuValueType>
ChartAxisProj<QuValueType>::ChartAxisProj(
    const ChartAxisProj_setup& setup, const Quantity& quantity, const std::vector<QuValueType>* qu_values)
    : ChartAxis<QuValueType>(setup, quantity, qu_values)
{
    constrImpl();
}

template <typename QuValueType>
ChartAxisProj<QuValueType>::ChartAxisProj(const ChartAxisProj_setup& setup, const Quantity& quantity, const QuValueType& min_qu_val,
    const QuValueType& max_qu_val)
    : ChartAxis<QuValueType>(setup, quantity, min_qu_val, max_qu_val)
{
    constrImpl();
}

template <typename QuValueType>
void ChartAxisProj<QuValueType>::constrImpl()
{
    this->setup = dynamic_cast<const too::math::ChartAxisProj_setup*>(ChartAxis::setup.get());
    TOO_EXPECT_THROW(this->setup);
    calcScaling();
}

template <typename QuValueType>
void ChartAxisProj<QuValueType>::calcScaling()
{
    this->tick_count = this->setup->max_tick_count ? *this->setup->max_tick_count : 11;

    ensure_non_zero_range();

    this->tick_step_qu_val  = calcNiceScaleTick(this->maxVal - this->minVal, this->tick_count);
    this->tick_start_qu_val = 0.0;
    this->tick_end_qu_val = 0.0;
    std::tie(tick_start_qu_val, tick_end_qu_val) = calcScaleTickFromTo(minVal, maxVal, tick_step_qu_val);
    this->tick_count = round_to<ScaleTickCount>((tick_end_qu_val - tick_start_qu_val) / tick_step_qu_val);

    this->map_quvalue_to_projection =
        too::make_unique<QuValue_to_Projection>(std::make_pair(tick_start_qu_val, tick_end_qu_val),
        std::make_pair(this->setup->projection_range.first, this->setup->projection_range.second));
}

template <typename QuValueType>
ProjectionValue ChartAxisProj<QuValueType>::quvalue_to_projection(QuValueType x) const
{
    TOO_EXPECT_THROW(this->map_quvalue_to_projection);
    const QuValue_to_Projection& q2p = *this->map_quvalue_to_projection;
    return q2p(x);
}

template <typename QuValueType>
ProjectionValue ChartAxisProj<QuValueType>::qurange_to_projectionrange(QuValueType width) const
{
    TOO_EXPECT_THROW(this->map_quvalue_to_projection);
    const QuValue_to_Projection& q2p = *this->map_quvalue_to_projection;
    return std::abs(q2p(width) - q2p(QuValueType()));
}

template <typename QuValueType>
QuValueType ChartAxisProj<QuValueType>::projection_to_quvalue(ProjectionValue p) const
{
    TOO_EXPECT_THROW(this->map_quvalue_to_projection);
    const QuValue_to_Projection& q2p = *this->map_quvalue_to_projection;
    return q2p.inverse(p);
}

template <typename QuValueType>
QuValueType ChartAxisProj<QuValueType>::projectionrange_to_qurange(ProjectionValue width) const
{
    TOO_EXPECT_THROW(this->map_quvalue_to_projection);
    const QuValue_to_Projection& q2p = *this->map_quvalue_to_projection;
    return std::abs(q2p.inverse(width) - q2p.inverse(ProjectionValue()));
}

template <typename QuValueType>
ProjectionValue ChartAxisProj<QuValueType>::getProjectionMinVal() const
{
    return this->setup->projection_range.first;
}

template <typename QuValueType>
ProjectionValue ChartAxisProj<QuValueType>::getProjectionMaxVal() const
{
    return this->setup->projection_range.second;
}


//####################################################################################################################

template <typename QuValueTypeX, typename QuValueTypeY>
Chart2D<QuValueTypeX, QuValueTypeY>::Chart2D(const ChartAxis_setup& setupX, const ChartAxis_setup& setupY,
    const std::pair<Quantity, Quantity>& quantitiesXY, const VectorOfPairs<QuValueTypeX, QuValueTypeY>* qu_values)
    : values(qu_values)
{
    using QuValueXY = std::pair<QuValueTypeX, QuValueTypeY>;
    auto minmax_X   = std::make_pair(QuValueTypeX(), QuValueTypeX());
    auto minmax_Y   = std::make_pair(QuValueTypeY(), QuValueTypeY());
    if (qu_values && !qu_values->empty())
    {
        const auto minmax_X_pair = std::minmax_element(std::begin(*qu_values), std::end(*qu_values),
            [](const QuValueXY& lhs, const QuValueXY& rhs) { return lhs.first < rhs.first; });
        const auto minmax_Y_pair = std::minmax_element(std::begin(*qu_values), std::end(*qu_values),
            [](const QuValueXY& lhs, const QuValueXY& rhs) { return lhs.second < rhs.second; });

        minmax_X = {(*minmax_X_pair.first).first, (*minmax_X_pair.second).first};
        minmax_Y = {(*minmax_Y_pair.first).second, (*minmax_Y_pair.second).second};
    }

    const auto x_is_as_proj_axis = dynamic_cast<const too::math::ChartAxisProj_setup*>(&setupX);
    if (x_is_as_proj_axis)
        this->x_axis = too::make_unique<ChartAxisProj<QuValueTypeX>>(
            *x_is_as_proj_axis, quantitiesXY.first, minmax_X.first, minmax_X.second);
    else
        this->x_axis =
            too::make_unique<ChartAxis<QuValueTypeX>>(setupX, quantitiesXY.first, minmax_X.first, minmax_X.second);

    const auto y_is_as_proj_axis = dynamic_cast<const too::math::ChartAxisProj_setup*>(&setupY);
    if (y_is_as_proj_axis)
        this->y_axis = too::make_unique<ChartAxisProj<QuValueTypeX>>(
            *y_is_as_proj_axis, quantitiesXY.second, minmax_Y.first, minmax_Y.second);
    else
        this->y_axis =
            too::make_unique<ChartAxis<QuValueTypeX>>(setupY, quantitiesXY.second, minmax_Y.first, minmax_Y.second);
}

} // math
} // too

#endif
