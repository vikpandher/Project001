#pragma once

#include "ScopeTimer.h"

#include <fstream>



namespace Project001
{	
	class Profiler
	{
	public:
		~Profiler();

		Profiler(const Profiler&) = delete;
		void operator=(const Profiler&) = delete;

		static void BeginSession(const char* outputFilePath = "Result.json");

		static void EndSession();

		static void WriteTimeRecord(const TimeRecord& timeRecord);

	protected:

	private:
		static Profiler& GetInstance();

		Profiler();
		//Profiler(const Profiler&);
		//void operator=(const Profiler&);

		int entryCount_;
		bool inSession_;
		std::mutex lock_;
		std::ofstream outputStream_;
	};
}