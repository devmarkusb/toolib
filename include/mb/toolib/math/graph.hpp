//! \file

#ifndef GRAPH_HPP_LNKJGNGKVFVUTZHIRTHCZREC5
#define GRAPH_HPP_LNKJGNGKVFVUTZHIRTHCZREC5

#include "quantity_unit.hpp"
#include "scale.hpp"
#include "../config.hpp"
#include "mb/ul/ul.hpp"
#include <algorithm>
#include <map>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

namespace mb::too::math {
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
struct TickStringReprSetup {
    //! Cf. to_string functions.
    ul::math::FloatFormat tick_float_format{ul::math::FloatFormat::default_choice};
    //! Cf. to_string functions.
    std::optional<int> tick_float_precision{};
};

//!
struct ChartAxisSetup {
    virtual ~ChartAxisSetup() = default;

    [[nodiscard]] virtual std::unique_ptr<ChartAxisSetup> clone() const {
        return std::make_unique<ChartAxisSetup>(*this);
    }

    //! If not provided, the maximum count of scale ticks on the axis is chosen automatically.
    std::optional<ScaleTickCount> max_tick_count;
    TickStringReprSetup tick_string_repr;
};

//!
struct ChartAxisProjSetup : public ChartAxisSetup {
    ~ChartAxisProjSetup() override = default;

    [[nodiscard]] std::unique_ptr<ChartAxisSetup> clone() const override {
        return std::unique_ptr<ChartAxisSetup>(new ChartAxisProjSetup(*this));
    }

    std::pair<ProjectionValue, ProjectionValue> projection_range;
};

//####################################################################################################################

//!
template <typename QuValueType>
class ChartAxis : private ul::NonCopyable {
public:
    ChartAxis(
        const ChartAxisSetup& setup, Quantity quantity, const QuValueType& min_qu_val, const QuValueType& max_qu_val);

    virtual ~ChartAxis() {
        UL_EXPECT(this->setup_);
    }

    [[nodiscard]] ScaleTickCount get_tick_count() const {
        return this->tick_count_;
    }

    [[nodiscard]] Quantity get_quantity() const {
        return this->quantity_;
    }

    QuValueType get_tick_start_val() const {
        return this->tick_start_qu_val_;
    }

    QuValueType get_tick_end_val() const {
        return this->tick_end_qu_val_;
    }

    QuValueType get_tick_step_val() const {
        return this->tick_step_qu_val_;
    }

    /** \return a new ratio for the Quantity Unit, if there is a better choice, i.e. a common ratio of the tick values
        can be obtained.*/
    [[nodiscard]] std::optional<ul::math::Rational> obtain_suitable_common_ratio_of_tickvals() const;
    void apply_ratio_to_tickvals(const ul::math::Rational& r);
    void apply_ratio_to_quantity_unit(const ul::math::Rational& r);

    [[nodiscard]] bool contains_zero_tick() const {
        return this->tick_start_qu_val_ <= QuValueType{} && QuValueType{} <= this->tick_end_qu_val_;
    }

    std::string tick_value_as_readable_string(const QuValueType& qu_val) const;

private:
    Quantity quantity_;

protected:
    static const int expected_ulp_difference_minmax{10};
    std::unique_ptr<const ChartAxisSetup> setup_;
    ScaleTickCount tick_count_{};
    QuValueType tick_step_qu_val_{};
    QuValueType tick_start_qu_val_{};
    QuValueType tick_end_qu_val_{};

private:
    void constr_common_impl(const QuValueType& min_qu_val, const QuValueType& max_qu_val);
    void expect_proper_setup() const;
    void ensure_proper_min_max(QuValueType& min_qu_val, QuValueType& max_qu_val) const;
    void calc_scaling(const QuValueType& min_qu_val, const QuValueType& max_qu_val);
};

//####################################################################################################################

//!
template <typename QuValueType>
class ChartAxisProj : public ChartAxis<QuValueType> {
public:
    ChartAxisProj(
        const ChartAxisProjSetup& setup, const Quantity& quantity, const QuValueType& min_qu_val,
        const QuValueType& max_qu_val);
    virtual ~ChartAxisProj() = default;

    void init_projections();

    ProjectionValue quvalue_to_projection(QuValueType x) const;
    ProjectionValue qurange_to_projectionrange(QuValueType width) const;
    QuValueType projection_to_quvalue(ProjectionValue p) const;
    QuValueType projectionrange_to_qurange(ProjectionValue width) const;

    [[nodiscard]] ProjectionValue get_projection_min_val() const;
    [[nodiscard]] ProjectionValue get_projection_max_val() const;

private:
    using QuValueToProjection = MapLinearScaleIntervalToInterval<QuValueType>;
    std::unique_ptr<QuValueToProjection> map_quvalue_to_projection_;
    const mb::too::math::ChartAxisProjSetup* setup_{nullptr}; // dyn-casted aux. alias of original setup member variable

    void constr_impl();
    void expect_proper_setup() const;
};


//####################################################################################################################

class ChartAnnotations;

//!
template <typename QuValueTypeX, typename QuValueTypeY>
class Chart2D {
public:
    using QuVX = QuValueTypeX;
    using QuVY = QuValueTypeY;

    //! Be aware of providing qu_values with sufficient live-time around
    //! instances of this class. This is designed to save a copy step. Furthermore the class will automatically
    //! take a common factor out of your data, if this makes aesthetic sense according to what quantitiesXY
    //! setup you provided for the quantities. (Cf. Unit inside Quantity.)
    Chart2D(
        const ChartAxisSetup& setup_x, const ChartAxisSetup& setup_y,
        const std::pair<Quantity, Quantity>& quantities_xy, VectorOfPairs<QuValueTypeX, QuValueTypeY>* qu_values);

    ul::NotNull<const ChartAxis<QuValueTypeX>*> get_x_axis() const {
        return x_axis_.get();
    }

    ul::NotNull<const ChartAxis<QuValueTypeX>*> get_y_axis() const {
        return y_axis_.get();
    }

    const VectorOfPairs<QuValueTypeX, QuValueTypeY>* get_values() const {
        return this->values_;
    }

    void set_annotations(std::unique_ptr<const ChartAnnotations> a) {
        this->annotations_ = std::move(a);
    }

    [[nodiscard]] const ChartAnnotations* get_annotations() const {
        return this->annotations_.get();
    }

private:
    using QuValueXY = std::pair<QuValueTypeX, QuValueTypeY>;

    VectorOfPairs<QuValueTypeX, QuValueTypeY>* values_;
    std::unique_ptr<ChartAxis<QuValueTypeX>> x_axis_;
    std::unique_ptr<ChartAxis<QuValueTypeY>> y_axis_;
    std::unique_ptr<const ChartAnnotations> annotations_;

    void pullout_common_factor_from_data();
};

//! Optional annotations for certain values (such that e.g. the graph could get a label at that point).
class ChartAnnotations {
public:
    //! Same index can occur multiple times.
    [[nodiscard]] const VectorOfPairs<size_t, std::string>& get_all() const {
        return this->annotations_;
    }

    //! Indices occur uniquely together with vector of associated annotations.
    [[nodiscard]] std::map<size_t, std::vector<std::string>> obtain_all_per_index() const {
        std::map<size_t, std::vector<std::string>> ret;
        for (const auto& an : this->annotations_) {
            ret[an.first].push_back(an.second);
        }
        return ret;
    }

    //! Add an optional annotation for a certain value index. The index is expected to be in a valid range.
    //! Also you might have to take care about not using the same index more than once. But that depends on your
    //! use-case - it is not forbidden.
    void add(const std::pair<const size_t, const std::string>& a) {
        this->annotations_.emplace_back(a);
    }

private:
    VectorOfPairs<size_t, std::string> annotations_;
};

} // namespace mb::too::math

//####################################################################################################################
// template implementations
//####################################################################################################################

#include "mb/toolib/math/scale.hpp"
#include "mb/ul/ul.hpp"

namespace mb::too::math {
template <typename QuValueType>
ChartAxis<QuValueType>::ChartAxis(
    const ChartAxisSetup& setup, Quantity quantity, const QuValueType& min_qu_val, const QuValueType& max_qu_val)
    : quantity_(std::move(quantity))
    , setup_(setup.clone()) {
    constr_common_impl(min_qu_val, max_qu_val);
}

template <typename QuValueType>
void ChartAxis<QuValueType>::constr_common_impl(const QuValueType& min_qu_val, const QuValueType& max_qu_val) {
    QuValueType minquval{min_qu_val};
    QuValueType maxquval{max_qu_val};
    ensure_proper_min_max(minquval, maxquval);
    expect_proper_setup();
    calc_scaling(minquval, maxquval);
}

template <typename QuValueType>
void ChartAxis<QuValueType>::expect_proper_setup() const {
    UL_EXPECT_THROW(this->setup_->max_tick_count ? *(this->setup_->max_tick_count) > 0 : true);
}

template <typename QuValueType>
void ChartAxis<QuValueType>::ensure_proper_min_max(QuValueType& min_qu_val, QuValueType& max_qu_val) const {
    UL_EXPECT_THROW(min_qu_val <= max_qu_val);
    if (ul::almost_equal_alltypes(min_qu_val, max_qu_val, expected_ulp_difference_minmax)) {
        min_qu_val -= 1;
        max_qu_val += 1;
    }
}

template <typename QuValueType>
void ChartAxis<QuValueType>::calc_scaling(const QuValueType& min_qu_val, const QuValueType& max_qu_val) {
    this->tick_count_ = this->setup_->max_tick_count ? *this->setup_->max_tick_count : 11;

    this->tick_step_qu_val_ = calc_nice_scale_tick(max_qu_val - min_qu_val, this->tick_count_);
    this->tick_start_qu_val_ = 0.0;
    this->tick_end_qu_val_ = 0.0;
    std::tie(this->tick_start_qu_val_, this->tick_end_qu_val_) =
        calc_scale_tick_from_to(min_qu_val, max_qu_val, tick_step_qu_val_);
    this->tick_count_ = ul::math::round_to<ScaleTickCount>((tick_end_qu_val_ - tick_start_qu_val_) / tick_step_qu_val_);
}

template <typename QuValueType>
std::optional<ul::math::Rational> ChartAxis<QuValueType>::obtain_suitable_common_ratio_of_tickvals() const {
    const auto unit = this->quantity_.get_unit();
    const auto max_abs = std::max(std::abs(this->tick_start_qu_val_), std::abs(this->tick_end_qu_val_));
    const auto suitable_ratio = unit.find_optimized_ratio(max_abs);
    const auto old_ratio = unit.get_ratio();

    if (old_ratio == suitable_ratio)
        return {};
    else
        return suitable_ratio;
}

template <typename QuValueType>
void ChartAxis<QuValueType>::apply_ratio_to_tickvals(const ul::math::Rational& r) {
    const auto unit = this->quantity_.get_unit();
    this->tick_start_qu_val_ = unit.convert_to_different_ratio(this->tick_start_qu_val_, r);
    this->tick_end_qu_val_ = unit.convert_to_different_ratio(this->tick_end_qu_val_, r);
    this->tick_step_qu_val_ = unit.convert_to_different_ratio(this->tick_step_qu_val_, r);

    if (!ul::math::is_power_of(r.as_floating_point<double>(), 10.0)) {
        // Wow :/ that's a nice error... you need to copy the this-members, because they
        // get modified within non-const calcScaling. Problem is, that the direct change of the
        // this-members in the called function modifies the functions const& parameters unexpectedly.
        // So don't alias them, but copy first...
        const auto minval = this->tick_start_qu_val_;
        const auto maxval = this->tick_end_qu_val_;
        calc_scaling(minval, maxval);
    }
}

template <typename QuValueType>
void ChartAxis<QuValueType>::apply_ratio_to_quantity_unit(const ul::math::Rational& r) {
    this->quantity_.get_unit().switch_ratio(r);
}

template <typename QuValueType>
std::string ChartAxis<QuValueType>::tick_value_as_readable_string(const QuValueType& qu_val) const {
    using namespace mb::too::math;
    const auto is_prec_defined = setup_->tick_string_repr.tick_float_precision;
    const int prec = is_prec_defined ? *setup_->tick_string_repr.tick_float_precision : 0;
    using ul::math::FloatFormat;
    using ul::math::to_string;
    switch (setup_->tick_string_repr.tick_float_format) {
        case FloatFormat::default_choice:
            if (is_prec_defined)
                return to_string<FloatFormat::default_choice>(qu_val, prec);
            else
                return to_string<FloatFormat::default_choice>(qu_val);
        case FloatFormat::fixed:
            if (is_prec_defined)
                return to_string<FloatFormat::fixed>(qu_val, prec);
            else
                return to_string<FloatFormat::fixed>(qu_val);
        case FloatFormat::scientific:
            if (is_prec_defined)
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
    const ChartAxisProjSetup& setup, const Quantity& quantity, const QuValueType& min_qu_val,
    const QuValueType& max_qu_val)
    : ChartAxis<QuValueType>(setup, quantity, min_qu_val, max_qu_val) {
    constr_impl();
}

template <typename QuValueType>
void ChartAxisProj<QuValueType>::constr_impl() {
    this->setup_ = dynamic_cast<const mb::too::math::ChartAxisProjSetup*>(ChartAxis<QuValueType>::setup_.get());
    UL_EXPECT_THROW(this->setup_);
    expect_proper_setup();
}

template <typename QuValueType>
void ChartAxisProj<QuValueType>::expect_proper_setup() const {
    UL_EXPECT_THROW(this->setup_->projection_range.first < this->setup_->projection_range.second);
    UL_EXPECT_THROW(!ul::almost_equal(
        this->setup_->projection_range.first, this->setup_->projection_range.second,
        ChartAxisProj<QuValueType>::expected_ulp_difference_minmax));
}

template <typename QuValueType>
void ChartAxisProj<QuValueType>::init_projections() {
    this->map_quvalue_to_projection_ = std::make_unique<QuValueToProjection>(
        std::make_pair(ChartAxisProj<QuValueType>::tick_start_qu_val_, ChartAxisProj<QuValueType>::tick_end_qu_val_),
        std::make_pair(this->setup_->projection_range.first, this->setup_->projection_range.second));
}

template <typename QuValueType>
ProjectionValue ChartAxisProj<QuValueType>::quvalue_to_projection(QuValueType x) const {
    UL_EXPECT_THROW(this->map_quvalue_to_projection_);
    const QuValueToProjection& q2p = *this->map_quvalue_to_projection_;
    return q2p(x);
}

template <typename QuValueType>
ProjectionValue ChartAxisProj<QuValueType>::qurange_to_projectionrange(QuValueType width) const {
    UL_EXPECT_THROW(this->map_quvalue_to_projection_);
    const QuValueToProjection& q2p = *this->map_quvalue_to_projection_;
    return std::abs(q2p(width) - q2p(QuValueType()));
}

template <typename QuValueType>
QuValueType ChartAxisProj<QuValueType>::projection_to_quvalue(ProjectionValue p) const {
    UL_EXPECT_THROW(this->map_quvalue_to_projection_);
    const QuValueToProjection& q2p = *this->map_quvalue_to_projection_;
    return q2p.inverse(p);
}

template <typename QuValueType>
QuValueType ChartAxisProj<QuValueType>::projectionrange_to_qurange(ProjectionValue width) const {
    UL_EXPECT_THROW(this->map_quvalue_to_projection_);
    const QuValueToProjection& q2p = *this->map_quvalue_to_projection_;
    return std::abs(q2p.inverse(width) - q2p.inverse(ProjectionValue()));
}

template <typename QuValueType>
ProjectionValue ChartAxisProj<QuValueType>::get_projection_min_val() const {
    return this->setup_->projection_range.first;
}

template <typename QuValueType>
ProjectionValue ChartAxisProj<QuValueType>::get_projection_max_val() const {
    return this->setup_->projection_range.second;
}

//####################################################################################################################

template <typename QuValueTypeX, typename QuValueTypeY>
Chart2D<QuValueTypeX, QuValueTypeY>::Chart2D(
    const ChartAxisSetup& setup_x, const ChartAxisSetup& setup_y, const std::pair<Quantity, Quantity>& quantities_xy,
    VectorOfPairs<QuValueTypeX, QuValueTypeY>* qu_values)
    : values_(qu_values) {
    auto minmax_x = std::make_pair(QuValueTypeX(), QuValueTypeX());
    auto minmax_y = std::make_pair(QuValueTypeY(), QuValueTypeY());
    if (qu_values && !qu_values->empty()) {
        const auto minmax_x_pair = std::minmax_element(
            std::begin(*qu_values), std::end(*qu_values), [](const QuValueXY& lhs, const QuValueXY& rhs) {
                return lhs.first < rhs.first;
            });
        const auto minmax_y_pair = std::minmax_element(
            std::begin(*qu_values), std::end(*qu_values), [](const QuValueXY& lhs, const QuValueXY& rhs) {
                return lhs.second < rhs.second;
            });

        minmax_x = {(*minmax_x_pair.first).first, (*minmax_x_pair.second).first};
        minmax_y = {(*minmax_y_pair.first).second, (*minmax_y_pair.second).second};
    }

    const auto x_is_as_proj_axis_setup = dynamic_cast<const mb::too::math::ChartAxisProjSetup*>(&setup_x);
    if (x_is_as_proj_axis_setup)
        this->x_axis_ = std::make_unique<ChartAxisProj<QuValueTypeX>>(
            *x_is_as_proj_axis_setup, quantities_xy.first, minmax_x.first, minmax_x.second);
    else
        this->x_axis_ =
            std::make_unique<ChartAxis<QuValueTypeX>>(setup_x, quantities_xy.first, minmax_x.first, minmax_x.second);
    auto x_is_as_proj_axis = dynamic_cast<ChartAxisProj<QuValueTypeX>*>(this->x_axis_.get());

    const auto y_is_as_proj_axis_setup = dynamic_cast<const mb::too::math::ChartAxisProjSetup*>(&setup_y);
    if (y_is_as_proj_axis_setup)
        this->y_axis_ = std::make_unique<ChartAxisProj<QuValueTypeY>>(
            *y_is_as_proj_axis_setup, quantities_xy.second, minmax_y.first, minmax_y.second);
    else
        this->y_axis_ =
            std::make_unique<ChartAxis<QuValueTypeY>>(setup_y, quantities_xy.second, minmax_y.first, minmax_y.second);
    auto y_is_as_proj_axis = dynamic_cast<ChartAxisProj<QuValueTypeY>*>(this->y_axis_.get());

    UL_ASSERT(this->x_axis_ && this->y_axis_);
    pullout_common_factor_from_data();

    if (x_is_as_proj_axis)
        x_is_as_proj_axis->init_projections();
    if (y_is_as_proj_axis)
        y_is_as_proj_axis->init_projections();

    UL_ENSURE(this->x_axis_ && this->y_axis_);
}

template <typename QuValueTypeX, typename QuValueTypeY>
void Chart2D<QuValueTypeX, QuValueTypeY>::pullout_common_factor_from_data() {
    const auto x_new_ratio = this->x_axis_->obtain_suitable_common_ratio_of_tickvals();
    const auto y_new_ratio = this->y_axis_->obtain_suitable_common_ratio_of_tickvals();

    if (x_new_ratio) {
        const auto new_ratio = *x_new_ratio;
        this->x_axis_->apply_ratio_to_tickvals(new_ratio);
        auto unit = this->x_axis_->get_quantity().get_unit();
        if (this->values_)
            std::for_each(std::begin(*this->values_), std::end(*this->values_), [&unit, &new_ratio](QuValueXY& xy) {
                xy.first = unit.convert_to_different_ratio(xy.first, new_ratio);
            });

        this->x_axis_->apply_ratio_to_quantity_unit(new_ratio);
    }
    if (y_new_ratio) {
        const auto new_ratio = *y_new_ratio;
        this->y_axis_->apply_ratio_to_tickvals(new_ratio);
        auto unit = this->y_axis_->get_quantity().get_unit();
        if (this->values_)
            std::for_each(std::begin(*this->values_), std::end(*this->values_), [&unit, &new_ratio](QuValueXY& xy) {
                xy.second = unit.convert_to_different_ratio(xy.second, new_ratio);
            });

        this->y_axis_->apply_ratio_to_quantity_unit(new_ratio);
    }
}
} // namespace mb::too::math


#endif
