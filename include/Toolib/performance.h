// Markus Borris, 2014
// This file is part of Toolib library. Open source.

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
#include "math/number.h"
#include "math/round.h"
#include "std/std_extensions.h"


namespace too
{

	namespace implDumpAllItems { struct SKeyData; } // fwd

	//! Can only be used within one thread at the same time.
	/** Usage: */
	class CPerformanceProfiler : private non_copyable
	{
	public:
		using TTimeValStorageRep = double;
		using TSecondsDbl = TTimeValStorageRep;
		using TNestingLevel = unsigned int;

		//! \param NestingLevel is just for dump visualization
		inline explicit CPerformanceProfiler(const std::string& NewItemName, TNestingLevel NestingLevel = 0);
		inline ~CPerformanceProfiler();
		TSecondsDbl Elapsed_CurrentItem() const;

		//! New item on same hierarchy/nesting level.
		inline void StartNewItem(const std::string& NewItemName);
		inline void StopItem();

		//! Also fills DumpDataTest if \param test is true.
		static std::string DumpAllItems(bool test = false);
		static void Reset();
		static std::string ToFormattedString(const TSecondsDbl& d);
		//! Only for testing.
		struct SDumpDataset
		{
			std::string m_ItemName;
			size_t      m_Count;
			TSecondsDbl m_Total;
			TSecondsDbl m_Average;
			TSecondsDbl m_Mean;
			TSecondsDbl m_StdDev;
		};
		//! Only for testing, expect it to be filled after DumpAllItems().
		static std::vector<SDumpDataset>& DumpDataTest()
		{
			static std::vector<SDumpDataset> data;
			return data;
		}

	private:
		friend struct implDumpAllItems::SKeyData;
		using TUniqueItemStartNr = unsigned long long;
		struct SItemData
		{
			TTimeValStorageRep  m_TimeVal{};
			TNestingLevel       m_NestingLevel{};
			TUniqueItemStartNr  m_StartNr{};

			SItemData(const TTimeValStorageRep& t, TNestingLevel nl, TUniqueItemStartNr n)
				: m_TimeVal(t)
				, m_NestingLevel(nl)
				, m_StartNr(n)
			{}
		};
		using TItemNameAsKey = std::string;
		using TItems = std::multimap<TItemNameAsKey, SItemData>;
		using chrono_clock = std::chrono::high_resolution_clock;
		using chrono_duration = std::chrono::duration<TTimeValStorageRep, std::ratio<1, 1>>;	// means seconds stored with type TTimeValStorage
		using chrono_timepoint = std::chrono::time_point<chrono_clock, chrono_duration>;

		chrono_timepoint        m_StartTime;
		chrono_timepoint        m_StopTime;
		std::string             m_ItemName;
		TNestingLevel           m_NestingLevel = TNestingLevel();
		TUniqueItemStartNr      m_ItemStartNr = TUniqueItemStartNr();

		inline void StartCurrentItem();
		inline void StopCurrentItem();
		static TUniqueItemStartNr& UniqueItemStartNr()
		{
			static TUniqueItemStartNr n = TUniqueItemStartNr();
			return n;
		}

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
					return rhs.second.m_TimeVal + v;
				return v;
			}
		private:
			TItemNameAsKey m_key;
		};
	};

	inline void CPerformanceProfiler::StartCurrentItem()
	{
		m_ItemStartNr = UniqueItemStartNr()++;
		m_StartTime = chrono_clock::now();
	}

	inline void CPerformanceProfiler::StopCurrentItem()
	{
		SItemData d(Elapsed_CurrentItem(), m_NestingLevel, m_ItemStartNr);
		const auto& it = Items().find(m_ItemName);
		// is the same item started and stopped a second time at least?
		if (it != Items().end())
		{
			// if on the same nesting level, we want to keep the order of a single run through this level;
			// we don't know which representative will be picked later to generate a data record
			if (m_NestingLevel == it->second.m_NestingLevel)
				d.m_StartNr = it->second.m_StartNr;
		}
		Items().insert(std::make_pair(m_ItemName, d));
	}

	inline CPerformanceProfiler::CPerformanceProfiler(const std::string& NewItemName, TNestingLevel NestingLevel)
		: m_ItemName(NewItemName)
		, m_NestingLevel(NestingLevel)
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

	namespace implDumpAllItems
	{
		struct SKeyData
		{
			using TNestingLevel = CPerformanceProfiler::TNestingLevel;
			using TUniqueItemStartNr = CPerformanceProfiler::TUniqueItemStartNr;
			using SItemData = CPerformanceProfiler::SItemData;

			TNestingLevel m_NestingLevel{};
			TUniqueItemStartNr m_StartNr{};

			explicit SKeyData(const SItemData& d)
				: m_NestingLevel(d.m_NestingLevel)
				, m_StartNr(d.m_StartNr)
			{}
		};
	}

	inline std::string CPerformanceProfiler::DumpAllItems(bool test)
	{
		if (test)
			DumpDataTest().clear();
		std::stringstream ret;
		if (Items().empty())
		{
			ret << "No performance measurement data." << std::endl;
			return ret.str();
		}

		using TKeySet_unsorted = std::map<TItemNameAsKey, implDumpAllItems::SKeyData>;
		TKeySet_unsorted keys_unsorted;
		std::transform(Items().begin(), Items().end(), std::inserter(keys_unsorted, keys_unsorted.begin()),
			[](decltype(*Items().begin())& i)
		{
			return std::make_pair(i.first, implDumpAllItems::SKeyData(i.second));
		}
		);
		using TKeyNameAndData = std::pair<TItemNameAsKey, implDumpAllItems::SKeyData>;
		using TKeySet = std::vector<TKeyNameAndData>;
		TKeySet keys(keys_unsorted.begin(), keys_unsorted.end());
		std::sort(keys.begin(), keys.end(),
			[](const TKeySet::value_type& k1, const TKeySet::value_type& k2) -> bool
		{
			return k1.second.m_StartNr < k2.second.m_StartNr;
		}
		);

		static const size_t COLUMN_WIDTH = 10;
		static const size_t COLUMN_WIDTH_HUGE = 29;

		ret << std::left;
		ret << std::setfill('-') << std::setw(COLUMN_WIDTH_HUGE + COLUMN_WIDTH * 5) << '-' << std::endl;
		ret << std::setfill(' ');
		ret << std::setw(COLUMN_WIDTH_HUGE) << std::setprecision(COLUMN_WIDTH_HUGE) << "Item"
			<< std::setw(COLUMN_WIDTH) << std::setprecision(COLUMN_WIDTH) << "Count"
			<< std::setw(COLUMN_WIDTH) << std::setprecision(COLUMN_WIDTH) << "Total"
			<< std::setw(COLUMN_WIDTH) << std::setprecision(COLUMN_WIDTH) << "Average"
			<< std::setw(COLUMN_WIDTH) << std::setprecision(COLUMN_WIDTH) << "Mean"
			<< std::setw(COLUMN_WIDTH) << std::setprecision(COLUMN_WIDTH) << "StdDev"
			<< std::endl;
		ret << std::setfill('-') << std::setw(COLUMN_WIDTH_HUGE + COLUMN_WIDTH * 5) << '-' << std::endl;
		ret << std::setfill(' ');

		for (const auto& key : keys)
		{
			TTimeValStorageRep totalT = std::accumulate(Items().begin(), Items().end(),
				TTimeValStorageRep(), accum_key(key.first));
			size_t count = std::count_if(Items().begin(), Items().end(), match_key(key.first));
			TTimeValStorageRep avgT = 0.0;
			if (count)
				avgT = totalT / static_cast<TTimeValStorageRep>(count);
			else
				avgT = std::numeric_limits<double>::infinity();

			std::vector<TTimeValStorageRep> sortedItems;
			for (const auto& item : Items())
			{
				if (key.first == item.first)
					sortedItems.push_back(item.second.m_TimeVal);
			}
			std::sort(sortedItems.begin(), sortedItems.end());
			size_t mid = static_cast<size_t>(floor(static_cast<double>(count) / 2.0));
			double meanT = (count > 1 && count % 2) ? (sortedItems[mid] + sortedItems[mid + 1]) / 2.0 : sortedItems[mid];

			double variance = 0.0;
			if (count > 1)
			{
				for (const auto& time : sortedItems)
					variance += pow(time - meanT, 2.0);
			}

			double stddev = count > 1 ? sqrt(variance / (static_cast<double>(count)-1.0)) : 0.0;

			std::stringstream ssItemNameWithSymbolizedNestingLevel;
			const TNestingLevel NESTING_LEVELS_SYMBOLIZED_BY_SPACES = 20;
			for (TNestingLevel nl = 1; nl <= key.second.m_NestingLevel && nl <= NESTING_LEVELS_SYMBOLIZED_BY_SPACES; ++nl)
				ssItemNameWithSymbolizedNestingLevel << ' ';
			ssItemNameWithSymbolizedNestingLevel << key.first;

			std::string ItemNameWithSymbolizedNestingLevel(ssItemNameWithSymbolizedNestingLevel.str());
			if (ItemNameWithSymbolizedNestingLevel.length() >= COLUMN_WIDTH_HUGE)
				ItemNameWithSymbolizedNestingLevel.resize(COLUMN_WIDTH_HUGE - 1);
			ret << std::setw(COLUMN_WIDTH_HUGE) << std::setprecision(COLUMN_WIDTH_HUGE) << ItemNameWithSymbolizedNestingLevel
				<< std::setw(COLUMN_WIDTH) << std::setprecision(COLUMN_WIDTH) << count
				<< std::setw(COLUMN_WIDTH) << std::setprecision(COLUMN_WIDTH) << ToFormattedString(totalT)
				<< std::setw(COLUMN_WIDTH) << std::setprecision(COLUMN_WIDTH) << ToFormattedString(avgT)
				<< std::setw(COLUMN_WIDTH) << std::setprecision(COLUMN_WIDTH) << ToFormattedString(meanT)
				<< std::setw(COLUMN_WIDTH) << std::setprecision(COLUMN_WIDTH) << ToFormattedString(stddev)
				<< std::endl;
			if (test)
				DumpDataTest().push_back({ ItemNameWithSymbolizedNestingLevel, count, totalT, avgT, meanT, stddev });
		}
		ret << std::setfill('-') << std::setw(COLUMN_WIDTH_HUGE + COLUMN_WIDTH * 5) << '-' << std::endl;
		ret << std::setfill(' ');
		return ret.str();
	}

	inline std::string CPerformanceProfiler::ToFormattedString(const TSecondsDbl& d)
	{
		std::stringstream ret;
		TSecondsDbl d_(d);
		if (d_ < 0.0)
		{
			d_ = -d_;
			ret << '-';
		}
		if (d_ == 0.0 || d_ < 0.000000000099995) // 0.0000000001
			ret << std::setprecision(2) << std::fixed << too::math::round(d_ * 1000000000000.0, 2) << " ps";
		else if (d_ < 0.000000099995) // 0.0000001
			ret << std::setprecision(2) << std::fixed << too::math::round(d_ * 1000000000.0, 2) << " ns";
		else if (d_ < 0.000099995) // 0.0001
			ret << std::setprecision(2) << std::fixed << too::math::round(d_ * 1000000.0, 2) << " " << "\xC2\xB5" << "s";
		else if (d_ < 0.099995) // 0.1
			ret << std::setprecision(2) << std::fixed << too::math::round(d_ * 1000.0, 2) << " ms";
		else if (d_ < 59.995) // 60.0
			ret << std::setprecision(2) << std::fixed << too::math::round(d_, 2) << " s";
		else if (d_ < 3600.0) // 3600.0
			ret << std::setw(2) << std::setfill('0') << floor(d_ / 60.0) << ':'
			<< std::setw(5) << std::fixed << std::setprecision(2) << std::setfill('0') << fmod(d_, 60.0);
		else if (d_ < 359999.0)
			ret << std::setw(2) << std::setfill('0') << floor(d_ / 3600.0) << ':'
			<< std::setw(2) << std::setfill('0') << floor(fmod(d_, 3600.0) / 60.0) << ':'
			<< std::setw(2) << std::setfill('0') << std::setprecision(0) << std::fixed << floor(fmod(d_, 60.0));
		else
			ret << ">= 100 h";
		return ret.str();
	}

	inline void CPerformanceProfiler::Reset()
	{
		Items().clear();
		UniqueItemStartNr() = TUniqueItemStartNr();
	}

	inline CPerformanceProfiler::TItems& CPerformanceProfiler::Items()
	{
		static TItems instance;
		return instance;
	}

}

#endif
