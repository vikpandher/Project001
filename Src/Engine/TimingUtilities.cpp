
#include "TimingUtilities.h"



namespace Project001
{
	// public ------------------------------------------------------------------

	TimeProfiler::~TimeProfiler()
	{
		EndSession();
	}

	void TimeProfiler::BeginSession(const char* outputFilePath)
	{
		TimeProfiler& instance = GetInstance();
		std::lock_guard<std::mutex> lock(instance.lock_);

		if (!instance.inSession_)
		{
			instance.outputStream_.open(outputFilePath);
			instance.outputStream_ << "{\"otherData\": {},\"traceEvents\":[" << std::endl;
			instance.outputStream_.flush();
			instance.inSession_ = true;
		}
	}

	void TimeProfiler::EndSession()
	{
		TimeProfiler& instance = GetInstance();
		std::lock_guard<std::mutex> lock(instance.lock_);

		if (instance.inSession_)
		{
			instance.outputStream_ << std::endl;
			instance.outputStream_ << "]}" << std::endl;
			instance.outputStream_.flush();
			instance.inSession_ = false;
		}
	}

	void TimeProfiler::WriteTimeRecord(const TimeRecord& timeRecord)
	{
		TimeProfiler& instance = GetInstance();
		std::lock_guard<std::mutex> lock(instance.lock_);

		if (instance.inSession_)
		{
			if (instance.entryCount_ > 0)
			{
				instance.outputStream_ << "," << std::endl;
			}

			instance.outputStream_ << "{" << std::endl;
			instance.outputStream_ << "\"cat\":\"function\"," << std::endl;
			instance.outputStream_ << "\"dur\":" << (timeRecord.stop_ns - timeRecord.start_ns) / 1000 << ',' << std::endl;
			instance.outputStream_ << "\"name\":\"" << timeRecord.label << "\"," << std::endl;
			instance.outputStream_ << "\"ph\":\"X\"," << std::endl;
			instance.outputStream_ << "\"pid\":0," << std::endl;
			instance.outputStream_ << "\"tid\":" << timeRecord.threadID << "," << std::endl;
			instance.outputStream_ << "\"ts\":" << timeRecord.start_ns / 1000 << std::endl;
			instance.outputStream_ << "}";
			instance.outputStream_.flush();

			instance.entryCount_++;
		}
	}

	// private -----------------------------------------------------------------

	TimeProfiler& TimeProfiler::GetInstance()
	{
		static TimeProfiler instance;
		return instance;
	}

	TimeProfiler::TimeProfiler()
		: entryCount_(0)
		, inSession_(false)
	{
	}
	
	// public ------------------------------------------------------------------

	ScopeTimer::ScopeTimer(const char* label)
	{
		timeRecord_.label = label;
		timeRecord_.threadID = std::this_thread::get_id();
		timeRecord_.start_ns = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	}

	ScopeTimer::~ScopeTimer()
	{
		timeRecord_.stop_ns = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		TimeProfiler::WriteTimeRecord(timeRecord_);
	}

	const TimeRecord& ScopeTimer::getTimeRecord()
	{
		return timeRecord_;
	}

}