
#include "ScopeTimer.h"



namespace Project001
{
	// public ------------------------------------------------------------------

	ScopeTimer::ScopeTimer(unsigned int timerID)
		: timerID_(timerID)
	{
		s_timeRecords_[timerID_].threadID = std::this_thread::get_id();
		s_timeRecords_[timerID_].startTimepoint = std::chrono::high_resolution_clock::now();
	}

	ScopeTimer::~ScopeTimer()
	{
		s_timeRecords_[timerID_].stopTimepoint = std::chrono::high_resolution_clock::now();
	}

	const TimeRecord& ScopeTimer::getTimeRecord(unsigned int timerID)
	{
		return s_timeRecords_[timerID];
	}

	// private -----------------------------------------------------------------

	TimeRecord ScopeTimer::s_timeRecords_[MAXIMUM_TIME_RECORDS];
}