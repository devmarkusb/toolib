/** \file
    todos:
    1. Rework to use the notion 'interval'. */

#ifndef MINMAX_VALUES_HPP_IUWENYG34GNYT487T
#define MINMAX_VALUES_HPP_IUWENYG34GNYT487T

#include "../config.hpp"

#include <algorithm>
#include <type_traits>
#include <utility>

namespace mb::too::math {
template <typename ValueType>
struct MinValue final {
    using V = ValueType;

    explicit MinValue(V min)
        : min_{std::move(min)} {
    }

    bool is_within_range(const V& v) const {
        return v >= min_;
    }

    bool is_overlapping_range(const V&, const V& end) const {
        return end >= min_;
    }

    //! Requires TransformFct to be a ValueType(const ValueType&) callable.
    template <typename TransformFct>
    void transform(TransformFct&& t) {
        min_ = t(min_);
    }

    //! Requires Fct to be a void(const ValueType&) callable.
    template <typename Fct>
    void for_each_in_overlapping_range(const V& begin, const V& end, Fct&& f) const {
        for (V v = std::max(min_, begin); v <= end; ++v) {
            f(v);
        }
    }

    V min() const {
        return min_;
    }

private:
    V min_;
};

//! Requires TransformFct to be a ValueTypeDst(const ValueTypeSrc&) callable.
template <typename ValueTypeSrc, typename TransformFct>
auto transform(const MinValue<ValueTypeSrc>& src, TransformFct&& t) {
    using ValueTypeDst = std::invoke_result_t<TransformFct, const ValueTypeSrc&>;
    return MinValue<ValueTypeDst>{t(src.min())};
}

template <typename ValueType>
struct MaxValue final {
    using V = ValueType;

    explicit MaxValue(V max)
        : max_{std::move(max)} {
    }

    bool is_within_range(const V& v) const {
        return v <= max_;
    }

    bool is_overlapping_range(const V& begin, const V&) const {
        return begin <= max_;
    }

    //! Requires TransformFct to be a ValueType(const ValueType&) callable.
    template <typename TransformFct>
    void transform(TransformFct&& t) {
        max_ = t(max_);
    }

    //! Requires Fct to be a void(const ValueType&) callable.
    template <typename Fct>
    void for_each_in_overlapping_range(const V& begin, const V& end, Fct&& f) const {
        for (V v = begin; v <= std::min(max_, end); ++v) {
            f(v);
        }
    }

    V max() const {
        return max_;
    }

private:
    V max_;
};

//! Requires TransformFct to be a ValueTypeDst(const ValueTypeSrc&) callable.
template <typename ValueTypeSrc, typename TransformFct>
auto transform(const MaxValue<ValueTypeSrc>& src, TransformFct&& t) {
    using ValueTypeDst = std::invoke_result_t<TransformFct, const ValueTypeSrc&>;
    return MaxValue<ValueTypeDst>{t(src.max())};
}

/** Represents a min, max value pair as a closed interval.
    Note that the (pathological) case of min > max correctly represents an empty interval, without any overhead.*/
template <typename ValueType>
struct MinMaxValue final {
    using V = ValueType;

    MinMaxValue(V min, V max)
        : min_{std::move(min)}
        , max_{std::move(max)} {
    }

    bool is_within_range(const V& v) const {
        return v >= min_ && v <= max_;
    }

    bool is_overlapping_range(const V& begin, const V& end) const {
        return end >= min_ && begin <= max_;
    }

    /** \tparam TransformFct required to be a ValueType(const ValueType&) callable.*/
    template <typename TransformFct>
    void transform(TransformFct&& t) {
        min_ = t(min_);
        max_ = t(max_);
    }

    /** \tparam Fct required to be a void(const ValueType&) callable.*/
    template <typename Fct>
    void for_each_in_overlapping_range(const V& begin, const V& end, Fct&& f) const {
        for (V v = std::max(min_, begin); v <= std::min(max_, end); ++v) {
            f(v);
        }
    }

    V min() const {
        return min_;
    }

    V max() const {
        return max_;
    }

private:
    V min_;
    V max_;
};

/** \tparam TransformFct required to be a ValueTypeDst(const ValueTypeSrc&) callable.*/
template <typename ValueTypeSrc, typename TransformFct>
auto transform(const MinMaxValue<ValueTypeSrc>& src, TransformFct&& t) {
    using ValueTypeDst = std::invoke_result_t<TransformFct, const ValueTypeSrc&>;
    return MinMaxValue<ValueTypeDst>{t(src.min()), t(src.max())};
}
} // namespace mb::too::math


#endif
