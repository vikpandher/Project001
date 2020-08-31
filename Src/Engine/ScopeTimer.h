#pragma once
#include <chrono>
#include <thread>



namespace Project001
{	
	struct TimeRecord
	{
		std::thread::id threadID;
		std::chrono::time_point<std::chrono::high_resolution_clock> startTimepoint;
		std::chrono::time_point<std::chrono::high_resolution_clock> stopTimepoint;

		inline long long getDuration_ns() const
		{
			return (stopTimepoint - startTimepoint).count();
		}

		inline long long getDuration_us() const
		{
			return getDuration_ns() / 1000;
		}

		inline long long getDuration_ms() const
		{
			return getDuration_ns() / 1000000;
		}
	};

	const unsigned int MAXIMUM_TIME_RECORDS = 16;
	
	class ScopeTimer
	{
	public:
		ScopeTimer(unsigned int timerID);
		~ScopeTimer();

		ScopeTimer(const ScopeTimer& rhs) = delete;
		ScopeTimer& operator=(const ScopeTimer& rhs) = delete;

		static const TimeRecord& getTimeRecord(unsigned int timerID);

	protected:

	private:
		unsigned int timerID_;

		static TimeRecord s_timeRecords_[MAXIMUM_TIME_RECORDS];
	};
}