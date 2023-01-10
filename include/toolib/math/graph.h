//! \file

#ifndef GRAPH_H_lnkjgngkvfvutzhirthczrec5
#define GRAPH_H_lnkjgngkvfvutzhirthczrec5

#include "number.h"
#include "quantity_unit.h"
#include "ratio.h"
#include "scale.h"
#include "../config.h"
#include "ul/ul.h"
#include <algorithm>
#include <map>
#include <memory>
#include <utility>
#include <vector>

namespace mb::too::math
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


//! How each tick label on an axis should be formatted to (represented as) string.
struct TickStringRepr_setup
{
    //! Cf. to_string functions.
    ul::math::FloatFormat tick_float_format{ul::math::FloatFormat::default_};
    //! Cf. to_string functions.
    ul::opt<int> tick_float_precision{};
};

//!
struct ChartAxis_setup
{
    virtual ~ChartAxis_setup() = default;

    [[nodiscard]] virtual std::unique_ptr<ChartAxis_setup> clone() const
    {
        return std::make_unique<ChartAxis_setup>(*this);
    }

    //! If not provided, the maximum count of scale ticks on the axis is chosen automatically.
    ul::opt<ScaleTickCount> max_tick_count;
    TickStringRepr_setup tick_string_repr;
};

//!
struct ChartAxisProj_setup : public ChartAxis_setup
{
    ~ChartAxisProj_setup() override = default;

    [[nodiscard]] std::unique_ptr<ChartAxis_setup> clone() const override
    {
        return std::make_unique<ChartAxisProj_setup>(*this);
    }

    std::pair<ProjectionValue, ProjectionValue> projection_range;
};

//####################################################################################################################

//!
template <typename QuValueType>
class ChartAxis : private ul::NonCopyable
{
public:
    ChartAxis(
        const ChartAxis_setup& setup, Quantity quantity, const QuValueType& min_qu_val, const QuValueType& max_qu_val);

    virtual ~ChartAxis()
    {
        UL_EXPECT(this->setup);
    }

    [[nodiscard]] ScaleTickCount getTickCount() const
    {
        return this->tick_count;
    }

    [[nodiscard]] Quantity getQuantity() const
    {
        return this->quantity;
    }

    QuValueType getTickStartVal() const
    {
        return this->tick_start_qu_val;
    }

    QuValueType getTickEndVal() const
    {
        return this->tick_end_qu_val;
    }

    QuValueType getTickStepVal() const
    {
        return this->tick_step_qu_val;
    }

    /** \return a new ratio for the Quantity Unit, if there is a better choice, i.e. a common ratio of the tick values
        can be obtained.*/
    [[nodiscard]] ul::opt<Rational> obtain_suitable_common_ratio_of_tickvals() const;
    void apply_ratio_to_tickvals(const Rational& r);
    void apply_ratio_to_quantity_unit(const Rational& r);

    [[nodiscard]] bool contains_zero_tick() const
    {
        return this->tick_start_qu_val <= QuValueType{} && QuValueType{} <= this->tick_end_qu_val;
    }

    std::string tickValueAsReadableString(const QuValueType& qu_val) const;

private:
    Quantity quantity;

protected:
    static const int expected_ulp_difference_minmax{10};
    std::unique_ptr<const ChartAxis_setup> setup;
    ScaleTickCount tick_count{};
    QuValueType tick_step_qu_val{};
    QuValueType tick_start_qu_val{};
    QuValueType tick_end_qu_val{};

private:
    void constr_common_impl(const QuValueType& min_qu_val, const QuValueType& max_qu_val);
    void expectProperSetup() const;
    void ensureProperMinMax(QuValueType& min_qu_val, QuValueType& max_qu_val) const;
    void calcScaling(const QuValueType& min_qu_val, const QuValueType& max_qu_val);
};

//####################################################################################################################

//!
template <typename QuValueType>
class ChartAxisProj : public ChartAxis<QuValueType>
{
public:
    ChartAxisProj(
        const ChartAxisProj_setup& setup, const Quantity& quantity, const QuValueType& min_qu_val,
        const QuValueType& max_qu_val);
    virtual ~ChartAxisProj() = default;

    void initProjections();

    ProjectionValue quvalue_to_projection(QuValueType x) const;
    ProjectionValue qurange_to_projectionrange(QuValueType width) const;
    QuValueType projection_to_quvalue(ProjectionValue p) const;
    QuValueType projectionrange_to_qurange(ProjectionValue width) const;

    [[nodiscard]] ProjectionValue getProjectionMinVal() const;
    [[nodiscard]] ProjectionValue getProjectionMaxVal() const;

private:
    using QuValue_to_Projection = Map_LinearScale_Interval_to_Interval<QuValueType>;
    std::unique_ptr<QuValue_to_Projection> map_quvalue_to_projection;
    const too::math::ChartAxisProj_setup* setup{nullptr}; // dyn-casted aux. alias of original setup member variable

    void constr_impl();
    void expectProperSetup() const;
};


//####################################################################################################################

class ChartAnnotations;

//!
template <typename QuValueTypeX, typename QuValueTypeY>
class Chart2D
{
public:
    using QuValueTypeX_ = QuValueTypeX;
    using QuValueTypeY_ = QuValueTypeY;

    //! Be aware of providing qu_values with sufficient live-time around
    //! instances of this class. This is designed to save a copy step. Furthermore the class will automatically
    //! take a common factor out of your data, if this makes aesthetic sense according to what quantitiesXY
    //! setup you provided for the quantities. (Cf. Unit inside Quantity.)
    Chart2D(
        const ChartAxis_setup& setupX, const ChartAxis_setup& setupY, const std::pair<Quantity, Quantity>& quantitiesXY,
        VectorOfPairs<QuValueTypeX, QuValueTypeY>* qu_values);

    ul::not_null<const ChartAxis<QuValueTypeX>*> get_x_axis() const
    {
        return x_axis.get();
    }

    ul::not_null<const ChartAxis<QuValueTypeX>*> get_y_axis() const
    {
        return y_axis.get();
    }

    const VectorOfPairs<QuValueTypeX, QuValueTypeY>* getValues() const
    {
        return this->values;
    }

    void setAnnotations(std::unique_ptr<const ChartAnnotations> a)
    {
        this->annotations = std::move(a);
    }

    [[nodiscard]] const ChartAnnotations* getAnnotations() const
    {
        return this->annotations.get();
    }

private:
    using QuValueXY = std::pair<QuValueTypeX, QuValueTypeY>;

    VectorOfPairs<QuValueTypeX, QuValueTypeY>* values;
    std::unique_ptr<ChartAxis<QuValueTypeX>> x_axis;
    std::unique_ptr<ChartAxis<QuValueTypeY>> y_axis;
    std::unique_ptr<const ChartAnnotations> annotations;

    void pullout_common_factor_from_data();
};

//! Optional annotations for certain values (such that e.g. the graph could get a label at that point).
class ChartAnnotations
{
public:
    //! Same index can occur multiple times.
    [[nodiscard]] const VectorOfPairs<size_t, std::string>& getAll() const
    {
        return this->annotations;
    }

    //! Indices occur uniquely together with vector of associated annotations.
    [[nodiscard]] std::map<size_t, std::vector<std::string>> obtainAllPerIndex() const
    {
        std::map<size_t, std::vector<std::string>> ret;
        for (const auto& an : this->annotations)
        {
            ret[an.first].push_back(an.second);
        }
        return ret;
    }

    //! Add an optional annotation for a certain value index. The index is expected to be in a valid range.
    //! Also you might have to take care about not using the same index more than once. But that depends on your
    //! use-case - it is not forbidden.
    void add(const std::pair<const size_t, const std::string>& a)
    {
        this->annotations.emplace_back(a);
    }

private:
    VectorOfPairs<size_t, std::string> annotations;
};

} // namespace mb::too::math

//####################################################################################################################
// template implementations
//####################################################################################################################

#include "toolib/math/round.h"
#include "toolib/math/scale.h"
#include "ul/ul.h"

namespace mb::too::math
{
template <typename QuValueType>
ChartAxis<QuValueType>::ChartAxis(
    const ChartAxis_setup& setup, Quantity quantity, const QuValueType& min_qu_val, const QuValueType& max_qu_val)
    : quantity(std::move(quantity))
    , setup(setup.clone())
{
    constr_common_impl(min_qu_val, max_qu_val);
}

template <typename QuValueType>
void ChartAxis<QuValueType>::constr_common_impl(const QuValueType& min_qu_val, const QuValueType& max_qu_val)
{
    QuValueType minquval{min_qu_val};
    QuValueType maxquval{max_qu_val};
    ensureProperMinMax(minquval, maxquval);
    expectProperSetup();
    calcScaling(minquval, maxquval);
}

template <typename QuValueType>
void ChartAxis<QuValueType>::expectProperSetup() const
{
    UL_EXPECT_THROW(this->setup->max_tick_count ? *(this->setup->max_tick_count) > 0 : true);
}

template <typename QuValueType>
void ChartAxis<QuValueType>::ensureProperMinMax(QuValueType& min_qu_val, QuValueType& max_qu_val) const
{
    UL_EXPECT_THROW(min_qu_val <= max_qu_val);
    if (ul::almost_equal_alltypes(min_qu_val, max_qu_val, expected_ulp_difference_minmax))
    {
        min_qu_val -= 1;
        max_qu_val += 1;
    }
}

template <typename QuValueType>
void ChartAxis<QuValueType>::calcScaling(const QuValueType& min_qu_val, const QuValueType& max_qu_val)
{
    this->tick_count = this->setup->max_tick_count ? *this->setup->max_tick_count : 11;

    this->tick_step_qu_val = calcNiceScaleTick(max_qu_val - min_qu_val, this->tick_count);
    this->tick_start_qu_val = 0.0;
    this->tick_end_qu_val = 0.0;
    std::tie(this->tick_start_qu_val, this->tick_end_qu_val) =
        calcScaleTickFromTo(min_qu_val, max_qu_val, tick_step_qu_val);
    this->tick_count = round_to<ScaleTickCount>((tick_end_qu_val - tick_start_qu_val) / tick_step_qu_val);
}

template <typename QuValueType>
ul::opt<Rational> ChartAxis<QuValueType>::obtain_suitable_common_ratio_of_tickvals() const
{
    const auto unit = this->quantity.getUnit();
    const auto max_abs = std::max(std::abs(this->tick_start_qu_val), std::abs(this->tick_end_qu_val));
    const auto suitable_ratio = unit.findOptimizedRatio(max_abs);
    const auto old_ratio = unit.getRatio();

    if (old_ratio == suitable_ratio)
        return {};
    else
        return suitable_ratio;
}

template <typename QuValueType>
void ChartAxis<QuValueType>::apply_ratio_to_tickvals(const Rational& r)
{
    const auto unit = this->quantity.getUnit();
    this->tick_start_qu_val = unit.convertToDifferentRatio(this->tick_start_qu_val, r);
    this->tick_end_qu_val = unit.convertToDifferentRatio(this->tick_end_qu_val, r);
    this->tick_step_qu_val = unit.convertToDifferentRatio(this->tick_step_qu_val, r);

    if (!too::math::isPowerOf(r.asFloatingPoint<double>(), 10.0))
    {
        // Wow :/ that's a nice error... you need to copy the this-members, because they
        // get modified within non-const calcScaling. Problem is, that the direct change of the
        // this-members in the called function modifies the functions const& parameters unexpectedly.
        // So don't alias them, but copy first...
        const auto minval = this->tick_start_qu_val;
        const auto maxval = this->tick_end_qu_val;
        calcScaling(minval, maxval);
    }
}

template <typename QuValueType>
void ChartAxis<QuValueType>::apply_ratio_to_quantity_unit(const Rational& r)
{
    this->quantity.getUnit().switchRatio(r);
}

template <typename QuValueType>
std::string ChartAxis<QuValueType>::tickValueAsReadableString(const QuValueType& qu_val) const
{
    using namespace mb::too::math;
    const auto isPrecDefined = setup->tick_string_repr.tick_float_precision;
    const int prec = isPrecDefined ? *setup->tick_string_repr.tick_float_precision : 0;
    using ul::math::FloatFormat;
    using ul::math::to_string;
    switch (setup->tick_string_repr.tick_float_format)
    {
        case FloatFormat::default_:
            if (isPrecDefined)
                return to_string<FloatFormat::default_>(qu_val, prec);
            else
                return to_string<FloatFormat::default_>(qu_val);
        case FloatFormat::fixed:
            if (isPrecDefined)
                return to_string<FloatFormat::fixed>(qu_val, prec);
            else
                return to_string<FloatFormat::fixed>(qu_val);
        case FloatFormat::scientific:
            if (isPrecDefined)
                return to_string<FloatFormat::scientific>(qu_val, prec);
            else
                return to_string<FloatFormat::scientific>(qu_val);
        default:
            UL_ASSERT_THROW(false);
    }
}

//####################################################################################################################

template <typename QuValueType>
ChartAxisProj<QuValueType>::ChartAxisProj(
    const ChartAxisProj_setup& setup, const Quantity& quantity, const QuValueType& min_qu_val,
    const QuValueType& max_qu_val)
    : ChartAxis<QuValueType>(setup, quantity, min_qu_val, max_qu_val)
{
    constr_impl();
}

template <typename QuValueType>
void ChartAxisProj<QuValueType>::constr_impl()
{
    this->setup = dynamic_cast<const too::math::ChartAxisProj_setup*>(ChartAxis<QuValueType>::setup.get());
    UL_EXPECT_THROW(this->setup);
    expectProperSetup();
}

template <typename QuValueType>
void ChartAxisProj<QuValueType>::expectProperSetup() const
{
    UL_EXPECT_THROW(this->setup->projection_range.first < this->setup->projection_range.second);
    UL_EXPECT_THROW(!ul::almost_equal(
        this->setup->projection_range.first, this->setup->projection_range.second,
        ChartAxisProj<QuValueType>::expected_ulp_difference_minmax));
}

template <typename QuValueType>
void ChartAxisProj<QuValueType>::initProjections()
{
    this->map_quvalue_to_projection = std::make_unique<QuValue_to_Projection>(
        std::make_pair(ChartAxisProj<QuValueType>::tick_start_qu_val, ChartAxisProj<QuValueType>::tick_end_qu_val),
        std::make_pair(this->setup->projection_range.first, this->setup->projection_range.second));
}

template <typename QuValueType>
ProjectionValue ChartAxisProj<QuValueType>::quvalue_to_projection(QuValueType x) const
{
    UL_EXPECT_THROW(this->map_quvalue_to_projection);
    const QuValue_to_Projection& q2p = *this->map_quvalue_to_projection;
    return q2p(x);
}

template <typename QuValueType>
ProjectionValue ChartAxisProj<QuValueType>::qurange_to_projectionrange(QuValueType width) const
{
    UL_EXPECT_THROW(this->map_quvalue_to_projection);
    const QuValue_to_Projection& q2p = *this->map_quvalue_to_projection;
    return std::abs(q2p(width) - q2p(QuValueType()));
}

template <typename QuValueType>
QuValueType ChartAxisProj<QuValueType>::projection_to_quvalue(ProjectionValue p) const
{
    UL_EXPECT_THROW(this->map_quvalue_to_projection);
    const QuValue_to_Projection& q2p = *this->map_quvalue_to_projection;
    return q2p.inverse(p);
}

template <typename QuValueType>
QuValueType ChartAxisProj<QuValueType>::projectionrange_to_qurange(ProjectionValue width) const
{
    UL_EXPECT_THROW(this->map_quvalue_to_projection);
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
Chart2D<QuValueTypeX, QuValueTypeY>::Chart2D(
    const ChartAxis_setup& setupX, const ChartAxis_setup& setupY, const std::pair<Quantity, Quantity>& quantitiesXY,
    VectorOfPairs<QuValueTypeX, QuValueTypeY>* qu_values)
    : values(qu_values)
{
    auto minmax_X = std::make_pair(QuValueTypeX(), QuValueTypeX());
    auto minmax_Y = std::make_pair(QuValueTypeY(), QuValueTypeY());
    if (qu_values && !qu_values->empty())
    {
        const auto minmax_X_pair = std::minmax_element(
            std::begin(*qu_values), std::end(*qu_values),
            [](const QuValueXY& lhs, const QuValueXY& rhs)
            {
                return lhs.first < rhs.first;
            });
        const auto minmax_Y_pair = std::minmax_element(
            std::begin(*qu_values), std::end(*qu_values),
            [](const QuValueXY& lhs, const QuValueXY& rhs)
            {
                return lhs.second < rhs.second;
            });

        minmax_X = {(*minmax_X_pair.first).first, (*minmax_X_pair.second).first};
        minmax_Y = {(*minmax_Y_pair.first).second, (*minmax_Y_pair.second).second};
    }

    const auto x_is_as_proj_axis_setup = dynamic_cast<const too::math::ChartAxisProj_setup*>(&setupX);
    if (x_is_as_proj_axis_setup)
        this->x_axis = std::make_unique<ChartAxisProj<QuValueTypeX>>(
            *x_is_as_proj_axis_setup, quantitiesXY.first, minmax_X.first, minmax_X.second);
    else
        this->x_axis =
            std::make_unique<ChartAxis<QuValueTypeX>>(setupX, quantitiesXY.first, minmax_X.first, minmax_X.second);
    auto x_is_as_proj_axis = dynamic_cast<ChartAxisProj<QuValueTypeX>*>(this->x_axis.get());

    const auto y_is_as_proj_axis_setup = dynamic_cast<const too::math::ChartAxisProj_setup*>(&setupY);
    if (y_is_as_proj_axis_setup)
        this->y_axis = std::make_unique<ChartAxisProj<QuValueTypeY>>(
            *y_is_as_proj_axis_setup, quantitiesXY.second, minmax_Y.first, minmax_Y.second);
    else
        this->y_axis =
            std::make_unique<ChartAxis<QuValueTypeY>>(setupY, quantitiesXY.second, minmax_Y.first, minmax_Y.second);
    auto y_is_as_proj_axis = dynamic_cast<ChartAxisProj<QuValueTypeY>*>(this->y_axis.get());

    UL_ASSERT(this->x_axis && this->y_axis);
    pullout_common_factor_from_data();

    if (x_is_as_proj_axis)
        x_is_as_proj_axis->initProjections();
    if (y_is_as_proj_axis)
        y_is_as_proj_axis->initProjections();

    UL_ENSURE(this->x_axis && this->y_axis);
}

template <typename QuValueTypeX, typename QuValueTypeY>
void Chart2D<QuValueTypeX, QuValueTypeY>::pullout_common_factor_from_data()
{
    const auto x_new_ratio = this->x_axis->obtain_suitable_common_ratio_of_tickvals();
    const auto y_new_ratio = this->y_axis->obtain_suitable_common_ratio_of_tickvals();

    if (x_new_ratio)
    {
        const auto new_ratio = *x_new_ratio;
        this->x_axis->apply_ratio_to_tickvals(new_ratio);
        auto unit = this->x_axis->getQuantity().getUnit();
        if (this->values)
            std::for_each(
                std::begin(*this->values), std::end(*this->values),
                [&unit, &new_ratio](QuValueXY& xy)
                {
                    xy.first = unit.convertToDifferentRatio(xy.first, new_ratio);
                });

        this->x_axis->apply_ratio_to_quantity_unit(new_ratio);
    }
    if (y_new_ratio)
    {
        const auto new_ratio = *y_new_ratio;
        this->y_axis->apply_ratio_to_tickvals(new_ratio);
        auto unit = this->y_axis->getQuantity().getUnit();
        if (this->values)
            std::for_each(
                std::begin(*this->values), std::end(*this->values),
                [&unit, &new_ratio](QuValueXY& xy)
                {
                    xy.second = unit.convertToDifferentRatio(xy.second, new_ratio);
                });

        this->y_axis->apply_ratio_to_quantity_unit(new_ratio);
    }
}
} // namespace mb::too::math

TOO_HEADER_END

#endif
