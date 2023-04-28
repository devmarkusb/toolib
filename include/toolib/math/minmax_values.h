/**
\file
todos:
1. Rework to use the notion 'interval'.
*/

#ifndef MINMAX_VALUES_H_iuwenyg34gnyt487t
#define MINMAX_VALUES_H_iuwenyg34gnyt487t

#include "../config.h"

#include <algorithm>
#include <type_traits>
#include <utility>

namespace mb::too::math {
template <typename ValueType>
struct MinValue final {
    using value_type = ValueType;

    explicit MinValue(ValueType min)
        : min_{std::move(min)} {
    }

    bool isWithinRange(const ValueType& v) const {
        return v >= min_;
    }

    bool isOverlappingRange(const ValueType&, const ValueType& end) const {
        return end >= min_;
    }

    //! Requires TransformFct to be a ValueType(const ValueType&) callable.
    template <typename TransformFct>
    void transform(TransformFct&& t) {
        min_ = t(min_);
    }

    //! Requires Fct to be a void(const ValueType&) callable.
    template <typename Fct>
    void forEachInOverlappingRange(const ValueType& begin, const ValueType& end, Fct&& f) const {
        for (ValueType v = std::max(min_, begin); v <= end; ++v) {
            f(v);
        }
    }

    ValueType min() const {
        return min_;
    }

private:
    ValueType min_;
};

//! Requires TransformFct to be a ValueTypeDst(const ValueTypeSrc&) callable.
template <typename ValueTypeSrc, typename TransformFct>
auto transform(const MinValue<ValueTypeSrc>& src, TransformFct&& t) {
    using ValueTypeDst = std::invoke_result_t<TransformFct, const ValueTypeSrc&>;
    return MinValue<ValueTypeDst>{t(src.min())};
}

template <typename ValueType>
struct MaxValue final {
    using value_type = ValueType;

    explicit MaxValue(ValueType max)
        : max_{std::move(max)} {
    }

    bool isWithinRange(const ValueType& v) const {
        return v <= max_;
    }

    bool isOverlappingRange(const ValueType& begin, const ValueType&) const {
        return begin <= max_;
    }

    //! Requires TransformFct to be a ValueType(const ValueType&) callable.
    template <typename TransformFct>
    void transform(TransformFct&& t) {
        max_ = t(max_);
    }

    //! Requires Fct to be a void(const ValueType&) callable.
    template <typename Fct>
    void forEachInOverlappingRange(const ValueType& begin, const ValueType& end, Fct&& f) const {
        for (ValueType v = begin; v <= std::min(max_, end); ++v) {
            f(v);
        }
    }

    ValueType max() const {
        return max_;
    }

private:
    ValueType max_;
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
    using value_type = ValueType;

    MinMaxValue(ValueType min, ValueType max)
        : min_{std::move(min)}
        , max_{std::move(max)} {
    }

    bool isWithinRange(const ValueType& v) const {
        return v >= min_ && v <= max_;
    }

    bool isOverlappingRange(const ValueType& begin, const ValueType& end) const {
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
    void forEachInOverlappingRange(const ValueType& begin, const ValueType& end, Fct&& f) const {
        for (ValueType v = std::max(min_, begin); v <= std::min(max_, end); ++v) {
            f(v);
        }
    }

    ValueType min() const {
        return min_;
    }

    ValueType max() const {
        return max_;
    }

private:
    ValueType min_;
    ValueType max_;
};

/** \tparam TransformFct required to be a ValueTypeDst(const ValueTypeSrc&) callable.*/
template <typename ValueTypeSrc, typename TransformFct>
auto transform(const MinMaxValue<ValueTypeSrc>& src, TransformFct&& t) {
    using ValueTypeDst = std::invoke_result_t<TransformFct, const ValueTypeSrc&>;
    return MinMaxValue<ValueTypeDst>{t(src.min()), t(src.max())};
}
} // namespace mb::too::math

TOO_HEADER_END

#endif
