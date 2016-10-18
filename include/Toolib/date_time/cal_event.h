// Markus Borris, 2016
// This file is part of Toolib library.

//!
/**
*/
//! \file

#ifndef CAL_EVENT_H_nbvxmcbvxmcnbxcmbierutozreoi
#define CAL_EVENT_H_nbvxmcbvxmcnbxcmbierutozreoi

#include "Toolib/ptr.h"
#include <memory>


namespace too
{
namespace date_time
{

//! Base for SingleEvent and RecurringEvent. Provides getFirst/getNext mechanism to
//! browse through events.
template <typename TimeType>
class CalEvent
{
public:
    virtual ~CalEvent() {}

    virtual too::owner<CalEvent<TimeType>*> clone() const = 0;

    virtual std::unique_ptr<TimeType> getFirstTimePoint() const = 0;
    //! If there is no further one, it \returns nullptr.
    virtual std::unique_ptr<TimeType> getNextTimePoint(const TimeType& RelativeTo) const = 0;
};

//! For events happening only once.
template <typename TimeType>
class SingleEvent : public virtual CalEvent<TimeType>
{
public:
    explicit SingleEvent(const TimeType& t) { setTimePoint(t); }
    virtual ~SingleEvent() = default;

    virtual too::owner<SingleEvent<TimeType>*> clone() const override { return new SingleEvent<TimeType>(*this); }

    virtual std::unique_ptr<TimeType> getFirstTimePoint() const override
    {
        return too::make_unique<TimeType>(m_TimePoint);
    }

    virtual std::unique_ptr<TimeType> getNextTimePoint(const TimeType&) const override { return nullptr; }

    void setTimePoint(const TimeType& t) { m_TimePoint = t; }
    TimeType getTimePoint() const { return m_TimePoint; }

private:
    TimeType m_TimePoint{};
};

//! For events starting at a certain time (or immediately, at 0, precisely TimeType()),
//! recurring in a certain period of time, and ending at a certain end time or before
//! It doesn't end at all when no end is provided.
template <typename TimeType>
class RecurringEvent : public virtual CalEvent<TimeType>
{
public:
    explicit RecurringEvent(const TimeType& period) { setTimePeriod(period); }
    virtual ~RecurringEvent() = default;
    RecurringEvent(const RecurringEvent& other);
    RecurringEvent& operator=(const RecurringEvent& other);

    virtual too::owner<RecurringEvent<TimeType>*> clone() const override { return new RecurringEvent<TimeType>(*this); }

    virtual std::unique_ptr<TimeType> getFirstTimePoint() const override;
    virtual std::unique_ptr<TimeType> getNextTimePoint(const TimeType& RelativeTo) const override;

    //! nullptr means earliest possible start, which is the default construction
    void setStart(const TimeType* t) { m_TimePointStart = t ? too::make_unique<TimeType>(*t) : nullptr; }
    const TimeType* getStart() const { return m_TimePointStart.get(); }

    //! nullptr means forever, which is default construction
    void setEnd(const TimeType* t) { m_TimePointEnd = t ? too::make_unique<TimeType>(*t) : nullptr; }
    const TimeType* getEnd() const { return m_TimePointEnd.get(); }

    void setTimePeriod(const TimeType& period) { m_TimePeriod = period; }
    TimeType getTimePeriod() const { return m_TimePeriod; }

private:
    std::unique_ptr<TimeType> m_TimePointStart;
    std::unique_ptr<TimeType> m_TimePointEnd;
    TimeType m_TimePeriod{};
};


//####################################################################################################################
// template implementation

template <typename TimeType>
RecurringEvent<TimeType>::RecurringEvent(const RecurringEvent<TimeType>& other)
{
    if (other.m_TimePointStart)
        m_TimePointStart = too::make_unique<TimeType>(*other.m_TimePointStart);
    if (other.m_TimePointEnd)
        m_TimePointEnd = too::make_unique<TimeType>(*other.m_TimePointEnd);
    m_TimePeriod       = other.m_TimePeriod;
}

template <typename TimeType>
RecurringEvent<TimeType>& RecurringEvent<TimeType>::operator=(const RecurringEvent<TimeType>& other)
{
    RecurringEvent<TimeType> tmp(other);
    *this = std::move(tmp);
    return *this;
}

template <typename TimeType>
std::unique_ptr<TimeType> RecurringEvent<TimeType>::getFirstTimePoint() const
{
    return m_TimePointStart ? too::make_unique<TimeType>(*m_TimePointStart) : too::make_unique<TimeType>(TimeType());
}

template <typename TimeType>
std::unique_ptr<TimeType> RecurringEvent<TimeType>::getNextTimePoint(const TimeType& RelativeTo) const
{
    if (m_TimePeriod == TimeType{})
        return nullptr;
    TimeType next = RelativeTo + m_TimePeriod;
    if (m_TimePointEnd && next > *m_TimePointEnd)
        return nullptr;
    else
        return too::make_unique<TimeType>(next);
}
}
}


#endif
