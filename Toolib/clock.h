// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/** To use Windows specific high precision clock_hp "#define TOO_CLOCK_USE_WINDOWS_HP 1" in front of
	header inclusion.
*/
//! \file

#pragma once
#ifndef CLOCK_H_INCL_ncf3z78g8on1x7fnm
#define CLOCK_H_INCL_ncf3z78g8on1x7fnm

#include <ctime>
#include "PPDEFS.H"
#if TOO_WINDOWS && TOO_CLOCK_USE_WINDOWS_HP
	#include <windows.h>
#endif
//#include <sys/time.h>

namespace too
{
	//##############################################################################################################
	//! Represents a clock. Has the additional feature of adjusting a speed of time ("zoom").
	//todo get higher precision (microsecs) with gettimeofday, sys\time.h
	class clock
	{
	public:
		//!
		enum EPrecision {
			PREC_MICROSEC,
			PREC_MILLISEC
		};

		//! Sets up an object containing a measuring point with the current time stamp (not necessarily the system time).
		/**
		\param fSpeedFactor adjusts a "time zoom" affecting calls of the elapsed() member.
		\param precision sets up the desired precision of the clock. Besides it influences the technical nature
		of the time stamp. This stamp will really be seen just as a technical value for most applications. But it can
		be requested by calls of now() or gettime() and here are the deteils:
		PREC_MILLISEC leads to the time stamp being the process time used since its start. This is currently the
		only supported choice.*/
		clock(double fSpeedFactor = 1.0, EPrecision precision = PREC_MILLISEC)
			: m_last_measurement(gettime(precision)), m_prec(precision), m_SpeedFactor(fSpeedFactor){}
		/**
		\returns the current time stamp (some technical value, \see clock()). It does not take into account
		the speed factor.
		\param bMemorizeMeasurement when set to true, now() starts a new time measuring interval
		by 0 regarding the next call of the elapsed() member. Otherwise the internal clock of the class
		keeps running, no measuring point is stored.*/
		clock_t now(bool bMemorizeMeasurement = true) {
			clock_t t = gettime(m_prec);
			if (bMemorizeMeasurement)
				m_last_measurement = t;
			return t;
		}
		//! Restarts the time measurement. The next call of elapsed() would return the elpased time since restart().
		void restart()
		{
			now();
		}
		//! Just returns some current time stamp (technical value, not necessarily the system time).
		/** \returns the current calling process time in milliseconds.
		\param Currently only milliseconds are supported, which leads to the stated return value.*/
		static clock_t gettime(EPrecision precision = PREC_MILLISEC) {
			switch (precision)
			{
			case PREC_MICROSEC:
			case PREC_MILLISEC:
			default:
				return ::clock();
			}
		}
		/**
		\returns the elapsed time since the last stored measuring point.
		\param bRestartClockImmediately starts the clock from zero again immediately, when true. Otherwise the next
		call of elapsed just adds up to the result of this one.*/
		clock_t elapsed(bool bRestartClockImmediately = true)
		{
			clock_t t = gettime(m_prec);
			clock_t ret = static_cast<clock_t>(m_SpeedFactor * (t - m_last_measurement));
			if (bRestartClockImmediately)
				m_last_measurement = t;
			return ret;
		}
		//! Getting and setting the "time zoom", affecting calls of the elapsed member.
		/** \returns a reference to the internally stored factor. A factor of 1.0 is normal
		system clock speed, 0.5 is half slow-motion, 2.0 would be twice the normal speed.*/
		double& SpeedFactor() { return m_SpeedFactor; }
	private:
		clock_t m_last_measurement; // stores the last measuring point
		double m_SpeedFactor; // stores the "time zoom" factor
		EPrecision m_prec; // precision used by gettime()
	};


	//##############################################################################################################
	//! High precision clock, Windows only.
#if TOO_WINDOWS && TOO_CLOCK_USE_WINDOWS_HP
	//! Usage: clock_hp c; /* do sth. ... */ double dMillisecTaken = c.elapsed();
	class clock_hp
	{
	public:
		//! Initializes processor tick frequency and a first absolute tick count.
		clock_hp() : m_bIsBroken(false), m_Freq(initFreq()), m_LastCount(getCount())  {}
		//! If strange things happen, you could ask whether the clock is broken.
		bool IsBroken() const
		{
			return m_bIsBroken;
		}
		//! \returns difference in seconds to last call of elapsed() or constructor.
		/** \param bRestartClockImmediately false would accumulate times without resetting.*/
		double elapsed(bool bRestartClockImmediately = true)
		{
			LONGLONG count = getCount();
			LONGLONG diff = count - m_LastCount;
			if (bRestartClockImmediately)
				m_LastCount = count;
			return static_cast<double>(diff)/static_cast<double>(m_Freq); // m_Freq != 0 by construction
		}
		void restart()
		{
			m_LastCount = getCount();
		}
	private:
		bool m_bIsBroken;
		LONGLONG m_Freq; // count per second
		LONGLONG m_LastCount;

		clock_hp(const clock_hp&);
		clock_hp& operator=(const clock_hp&);
		LONGLONG initFreq()
		{
			LONGLONG f;
			if (!QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&f)) || !f)
				m_bIsBroken = true;
			return f;
		}
		LONGLONG getCount()
		{
			LONGLONG c;
			if (!QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&c)))
				m_bIsBroken = true;
			return c;
		}
	};
#endif

} // too

#endif