
#include "Profiler.h"



namespace Project001
{
	// public ------------------------------------------------------------------

	Profiler::~Profiler()
	{
		EndSession();
	}

	void Profiler::BeginSession(const char* outputFilePath)
	{
		Profiler& instance = GetInstance();
		std::lock_guard<std::mutex> lock(instance.lock_);

		if (!instance.inSession_)
		{
			instance.outputStream_.open(outputFilePath);
			instance.outputStream_ << "{\"otherData\": {},\"traceEvents\":[";
			instance.outputStream_.flush();
			instance.inSession_ = true;
		}
	}

	void Profiler::EndSession()
	{
		Profiler& instance = GetInstance();
		std::lock_guard<std::mutex> lock(instance.lock_);

		if (instance.inSession_)
		{
			instance.outputStream_ << "]}";
			instance.outputStream_.flush();
			instance.inSession_ = false;
		}
	}

	void Profiler::WriteTimeRecord(const TimeRecord& timeRecord)
	{
		Profiler& instance = GetInstance();
		std::lock_guard<std::mutex> lock(instance.lock_);

		if (instance.inSession_)
		{
			if (instance.entryCount_ > 0)
			{
				instance.entryCount_++;
				instance.outputStream_ << ",";
			}

			instance.outputStream_ << "{";
			instance.outputStream_ << "\"cat\":\"function\",";
			instance.outputStream_ << "\"dur\":" << (timeRecord.start_ns - timeRecord.stop_ns) / 1000 << ',';
			instance.outputStream_ << "\"name\":\"" << timeRecord.label << "\",";
			instance.outputStream_ << "\"ph\":\"X\",";
			instance.outputStream_ << "\"pid\":0,";
			instance.outputStream_ << "\"tid\":" << timeRecord.threadID << ",";
			instance.outputStream_ << "\"ts\":" << timeRecord.start_ns / 1000;
			instance.outputStream_ << "}";
			instance.outputStream_.flush();
		}
	}

	// private -----------------------------------------------------------------

	Profiler& Profiler::GetInstance()
	{
		static Profiler instance;
		return instance;
	}

	Profiler::Profiler()
		: entryCount_(0)
		, inSession_(false)
	{
	}
}