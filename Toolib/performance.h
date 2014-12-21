// Markus Borris, 2014
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef PERFORMANCE_H_INCL_sdkfgnxzuwegnf93746ryng342grf
#define PERFORMANCE_H_INCL_sdkfgnxzuwegnf93746ryng342grf

#include <string>
#include <sstream>
#include <chrono>
#include <functional>
#include <iomanip>
#include <map>
#include <limits>
#include <algorithm>
#include <cmath>
#include <vector>
#include <set>
#include "class/non_copyable.h"


namespace too
{
	class CPerformanceProfiler : private non_copyable
	{
	public:
		using TTimeValStorageRep = double;
        static constexpr TTimeValStorageRep TTimeValStorageRep_zeroInit = 0.0;
		using TSecondsDbl = TTimeValStorageRep;
		inline CPerformanceProfiler(const std::string& NewItemName);
		inline ~CPerformanceProfiler();
		TSecondsDbl Elapsed_CurrentItem() const;

		//! New item on same hierarchy level (only 1 level supported by this class).
		inline void StartNewItem(const std::string& NewItemName);
        inline void StopItem();

		static std::string DumpAllItems();
		static void Reset();
		static std::string ToFormattedString(const TSecondsDbl& d);

	private:
		using TItemNameAsKey	= std::string;
        using TItems			= std::multimap<TItemNameAsKey, TTimeValStorageRep>;
        using chrono_clock		= std::chrono::high_resolution_clock;
		using chrono_duration	= std::chrono::duration<TTimeValStorageRep, std::ratio<1, 1>>;	// means seconds stored with type TTimeValStorage
        using chrono_timepoint	= std::chrono::time_point<chrono_clock, chrono_duration>;

		chrono_timepoint	m_StartTime;
		chrono_timepoint	m_StopTime;
		std::string			m_ItemName;

        inline void StartCurrentItem();
        inline void StopCurrentItem();

		static TItems& Items();

		struct match_key : public std::unary_function<TItems::value_type, bool>
		{
			match_key(const TItemNameAsKey& key) : m_key(key) {}
			bool operator()(TItems::value_type const & rhs) const
			{
				return m_key == rhs.first;
			}
			match_key& operator=(const match_key& rhs)
			{
				m_key = rhs.m_key;
				return *this;
			}
		private:
			TItemNameAsKey m_key;
		};

        struct accum_key : public std::binary_function<TTimeValStorageRep, TItems::value_type, TTimeValStorageRep>
		{
			accum_key(const TItemNameAsKey& key) : m_key(key) {}
            TTimeValStorageRep operator()(const TTimeValStorageRep& v, const TItems::value_type& rhs) const
			{
				if (m_key == rhs.first)
					return rhs.second + v;
				return v;
			}
		private:
            TItemNameAsKey m_key;
		};
	};

    inline void CPerformanceProfiler::StartCurrentItem()
	{
		m_StartTime	= chrono_clock::now();
	}
	
    inline void CPerformanceProfiler::StopCurrentItem()
	{
		Items().insert(std::make_pair(m_ItemName, Elapsed_CurrentItem()));
	}

    inline CPerformanceProfiler::CPerformanceProfiler(const std::string& NewItemName) : m_ItemName(NewItemName)
	{
		StartCurrentItem();
	}

    inline CPerformanceProfiler::TSecondsDbl CPerformanceProfiler::Elapsed_CurrentItem() const
	{
		chrono_timepoint now = chrono_clock::now();
		chrono_duration elapsed = now - m_StartTime;
		return elapsed.count();
	}

	inline CPerformanceProfiler::~CPerformanceProfiler()
	{
		StopCurrentItem();
    }

    inline void CPerformanceProfiler::StartNewItem(const std::string& NewItemName)
	{
		StopCurrentItem();
		m_ItemName = NewItemName;
		StartCurrentItem();
	}

    inline void CPerformanceProfiler::StopItem()
    {
        StopCurrentItem();
    }

	inline std::string CPerformanceProfiler::DumpAllItems()
	{
		std::stringstream ret;
		if (Items().empty())
		{
            ret << "No performance measurement data." << std::endl;
			return ret.str();
		}

        using TKeySet = std::set<std::string>;
		TKeySet keys;
        std::transform(Items().begin(), Items().end(), std::inserter(keys, keys.begin()),
                       [] (decltype(*Items().begin())& i) { return i.first; });

        static const size_t ColumnWidth = 10;
        static const size_t ColumnWidthLarger = 12;
        static const size_t ColumnWidthHuge = 26;

        ret << std::left;
        ret << std::setfill('-') << std::setw(ColumnWidthHuge + ColumnWidthLarger + ColumnWidth * 4) << '-' << std::endl;
        ret << std::setfill(' ');
        ret     << std::setw(ColumnWidthHuge) << std::setprecision(ColumnWidthHuge) << "Item"
                << std::setw(ColumnWidthLarger) << std::setprecision(ColumnWidthLarger) << "Count"
                << std::setw(ColumnWidth) << std::setprecision(ColumnWidth) << "Total"
                << std::setw(ColumnWidth) << std::setprecision(ColumnWidth) << "Average"
                << std::setw(ColumnWidth) << std::setprecision(ColumnWidth) << "Mean"
                << std::setw(ColumnWidth) << std::setprecision(ColumnWidth) << "StdDev"
                << std::endl;
        ret << std::setfill('-') << std::setw(ColumnWidthHuge + ColumnWidthLarger + ColumnWidth * 4) << '-' << std::endl;
        ret << std::setfill(' ');

        for (const auto& key : keys)
        {
            TTimeValStorageRep totalT = std::accumulate(Items().begin(), Items().end(),
                                                        TTimeValStorageRep_zeroInit, accum_key(key));
            size_t count = std::count_if(Items().begin(), Items().end(), match_key(key));
            TTimeValStorageRep avgT = 0.0;
            if (count)
                avgT = totalT / static_cast<TTimeValStorageRep>(count);
            else
                avgT = std::numeric_limits<double>::infinity();

            std::vector<TTimeValStorageRep> sortedItems;
            for (const auto& item : Items())
            {
                if (key == item.first)
                    sortedItems.push_back(item.second);
            }
            std::sort(sortedItems.begin(), sortedItems.end());
            size_t mid = static_cast<size_t>(floor(static_cast<double>(count) / 2.0));
            double meanT = (count > 1 && count % 2) ? (sortedItems[mid] + sortedItems[mid + 1]) / 2.0 : sortedItems[mid];

            double variance = 0.0;
            if (count > 1)
			{
                for (const auto& time : sortedItems)
                    variance+= pow(time - meanT, 2.0);
			}

            double stddev = count > 1 ? sqrt(variance / (static_cast<double>(count) - 1.0)) : 0.0;

            ret     << std::setw(ColumnWidthHuge) << std::setprecision(ColumnWidthHuge) << key
                    << std::setw(ColumnWidthLarger) << std::setprecision(ColumnWidthLarger) << count
                    << std::setw(ColumnWidth) << std::setprecision(ColumnWidth) << ToFormattedString(totalT)
                    << std::setw(ColumnWidth) << std::setprecision(ColumnWidth) << ToFormattedString(avgT)
                    << std::setw(ColumnWidth) << std::setprecision(ColumnWidth) << ToFormattedString(meanT)
                    << std::setw(ColumnWidth) << std::setprecision(ColumnWidth) << ToFormattedString(stddev)
                    << std::endl;
        }
        ret << std::setfill('-') << std::setw(ColumnWidthHuge + ColumnWidthLarger + ColumnWidth * 4) << '-' << std::endl;
        ret << std::setfill(' ');
        return ret.str();
	}

    inline std::string CPerformanceProfiler::ToFormattedString(const TSecondsDbl& d)
	{
        std::stringstream ret;
        if (d < 0.0000000001)
            ret << std::setprecision(4) << std::fixed << d * 1000000000000.0 << " ps";
        else if (d < 0.0000001)
            ret << std::setprecision(4) << d * 1000000000.0 << " ns";
        else if (d < 0.0001)
            ret << std::setprecision(4) << d * 1000000.0 << " " << u8"\xC2\xB5" << "s";
        else if (d < 0.1)
            ret << std::setprecision(4) << d * 1000.0 << " ms";
        else if (d < 60.0)
            ret << std::setprecision(2) << std::fixed << d << " s";
        else if (d < 3600.0)
            ret << std::setw(2) << std::setfill('0') << floor(d / 60.0) << ':'
                << std::setw(4) << std::setprecision(2) << std::setfill('0') << fmod(d, 60.0);
        else
            ret << std::setw(2) << std::setfill('0') << floor(d / 3600.0) << ':'
                << std::setw(2) << std::setfill('0') << floor(fmod(d, 3600.0) / 60.0)
                << std::setw(4) << std::setprecision(2) << std::setfill('0') << fmod(d, 60.0);
        return ret.str();
	}

	inline void CPerformanceProfiler::Reset()
	{
		Items().clear();
	}

	inline CPerformanceProfiler::TItems& CPerformanceProfiler::Items()
	{
        static TItems instance;
        return instance;
	}
}

#endif
