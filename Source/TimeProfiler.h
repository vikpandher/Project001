#pragma once

#include <chrono>
#include <fstream>
#include <mutex>
#include <string>
#include <thread>



namespace Project001
{	
	struct TimeRecord
	{
		std::string label;
		std::thread::id threadID; // maybe replace this with a "cost char * name"
		long long start_ns;
		long long stop_ns;

		inline long long getDuration_ns() const
		{
			return stop_ns - stop_ns;
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

	class TimeProfiler
	{
	public:
		~TimeProfiler();

		TimeProfiler(const TimeProfiler&) = delete;
		void operator=(const TimeProfiler&) = delete;

		static void BeginSession(const char* outputFilePath = "Result.json");

		static void EndSession();

		static void WriteTimeRecord(const TimeRecord& timeRecord);

	protected:

	private:
		static TimeProfiler& GetInstance();

		TimeProfiler();
		//TimeProfiler(const TimeProfiler&);
		//void operator=(const TimeProfiler&);

		int entryCount_;
		bool inSession_;
		std::mutex lock_;
		std::ofstream outputStream_;
	};
	
	class ScopeTimer
	{
	public:
		ScopeTimer(const char* label);
		~ScopeTimer();

		ScopeTimer(const ScopeTimer& rhs) = delete;
		ScopeTimer& operator=(const ScopeTimer& rhs) = delete;

		const TimeRecord& getTimeRecord();

	protected:

	private:
		TimeRecord timeRecord_;
	};
}