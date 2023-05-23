//! \file

#ifndef CAL_EVENT_H_NBVXMCBVXMCNBXCMBIERUTOZREOI
#define CAL_EVENT_H_NBVXMCBVXMCNBXCMBIERUTOZREOI

#include "../config.h"
#include "ul/ptr.h"
#include <memory>

namespace mb::too::date_time {
//! Base for SingleEvent and RecurringEvent. Provides getFirst/getNext mechanism to browse through events.
template <typename TimeType>
class CalEvent {
public:
    virtual ~CalEvent() = default;

    virtual ul::Owner<CalEvent<TimeType>*> clone() const = 0;

    //! Has to ensure to never return nullptr.
    virtual std::unique_ptr<TimeType> get_first_time_point() const = 0;
    //! If there is no further one, it returns nullptr.
    virtual std::unique_ptr<TimeType> get_next_time_point(const TimeType& relative_to) const = 0;

    //! Convenience function.
    [[nodiscard]] bool is_single() const {
        return !get_next_time_point(*get_first_time_point());
    }
};

//! For events happening only once.
template <typename TimeType>
class SingleEvent : public virtual CalEvent<TimeType> {
public:
    explicit SingleEvent(const TimeType& t) {
        set_time_point(t);
    }

    ~SingleEvent() override = default;

    ul::Owner<SingleEvent<TimeType>*> clone() const override {
        return new SingleEvent<TimeType>(*this);
    }

    std::unique_ptr<TimeType> get_first_time_point() const override {
        return std::make_unique<TimeType>(m_time_point_);
    }

    std::unique_ptr<TimeType> get_next_time_point(const TimeType&) const override {
        return nullptr;
    }

    void set_time_point(const TimeType& t) {
        m_time_point_ = t;
    }

    TimeType get_time_point() const {
        return m_time_point_;
    }

private:
    TimeType m_time_point_{};
};

/** For events starting at a certain time (or immediately, at 0, precisely TimeType()),
    recurring in a certain period of time, and ending at a certain end time or before
    It doesn't end at all when no end is provided.*/
template <typename TimeType>
class RecurringEvent : public virtual CalEvent<TimeType> {
public:
    explicit RecurringEvent(const TimeType& period) {
        set_time_period(period);
    }

    ~RecurringEvent() override = default;
    RecurringEvent(const RecurringEvent& other);
    RecurringEvent& operator=(const RecurringEvent& other);

    ul::Owner<RecurringEvent<TimeType>*> clone() const override {
        return new RecurringEvent<TimeType>(*this);
    }

    std::unique_ptr<TimeType> get_first_time_point() const override;
    std::unique_ptr<TimeType> get_next_time_point(const TimeType& RelativeTo) const override;

    //! nullptr means earliest possible start, which is the default construction
    void set_start(const TimeType* t) {
        m_time_point_start_ = t ? std::make_unique<TimeType>(*t) : nullptr;
    }

    const TimeType* get_start() const {
        return m_time_point_start_.get();
    }

    TimeType* get_start() {
        return m_time_point_start_.get();
    }

    void backup_start_move() {
        m_backup_start_ = std::move(m_time_point_start_);
    }

    [[nodiscard]] bool has_start_backup() const {
        return static_cast<bool>(m_backup_start_);
    }

    void restore_start_move() {
        m_time_point_start_ = std::move(m_backup_start_);
    }

    //! nullptr means forever, which is default construction
    void set_end(const TimeType* t) {
        m_time_point_end_ = t ? std::make_unique<TimeType>(*t) : nullptr;
    }

    const TimeType* get_end() const {
        return m_time_point_end_.get();
    }

    TimeType* get_end() {
        return m_time_point_end_.get();
    }

    void backup_end_move() {
        m_backup_end_ = std::move(m_time_point_end_);
    }

    [[nodiscard]] bool has_end_backup() const {
        return static_cast<bool>(m_backup_end_);
    }

    void restore_end_move() {
        m_time_point_end_ = std::move(m_backup_end_);
    }

    void set_time_period(const TimeType& period) {
        m_time_period_ = period;
    }

    TimeType get_time_period() const {
        return m_time_period_;
    }

private:
    std::unique_ptr<TimeType> m_time_point_start_;
    std::unique_ptr<TimeType> m_time_point_end_;
    std::unique_ptr<TimeType> m_backup_start_;
    std::unique_ptr<TimeType> m_backup_end_;
    TimeType m_time_period_{};
};

//####################################################################################################################
// template implementation

template <typename TimeType>
RecurringEvent<TimeType>::RecurringEvent(const RecurringEvent<TimeType>& other) {
    if (other.m_time_point_start_)
        m_time_point_start_ = std::make_unique<TimeType>(*other.m_time_point_start_);
    if (other.m_time_point_end_)
        m_time_point_end_ = std::make_unique<TimeType>(*other.m_time_point_end_);
    m_time_period_ = other.m_time_period_;
}

template <typename TimeType>
RecurringEvent<TimeType>& RecurringEvent<TimeType>::operator=(const RecurringEvent<TimeType>& other) {
    RecurringEvent<TimeType> tmp(other);
    *this = std::move(tmp);
    return *this;
}

template <typename TimeType>
std::unique_ptr<TimeType> RecurringEvent<TimeType>::get_first_time_point() const {
    return m_time_point_start_ ? std::make_unique<TimeType>(*m_time_point_start_)
                               : std::make_unique<TimeType>(TimeType());
}

template <typename TimeType>
std::unique_ptr<TimeType> RecurringEvent<TimeType>::get_next_time_point(const TimeType& RelativeTo) const {
    if (m_time_period_ == TimeType{})
        return nullptr;
    TimeType next = RelativeTo + m_time_period_;
    if (m_time_point_end_ && next > *m_time_point_end_)
        return nullptr;
    else
        return std::make_unique<TimeType>(next);
}
} // namespace mb::too::date_time

TOO_HEADER_END

#endif
