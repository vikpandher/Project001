// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

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
        TimeRecord()
            : start_ns(0)
            , stop_ns(0)
        {}

        std::string label;
        std::thread::id threadID; // maybe replace this with a "cost char * name"
        long long start_ns;
        long long stop_ns;

        long long getDuration_ns() const
        {
            return stop_ns - start_ns;
        }

        long long getDuration_us() const
        {
            return getDuration_ns() / 1000;
        }

        long long getDuration_ms() const
        {
            return getDuration_ns() / 1000000;
        }
    };

    // Output can be viewed in chrome://tracing/
    class TimeProfiler
    {
    public:
        ~TimeProfiler()
        {
            EndSession();
        }

        TimeProfiler(const TimeProfiler&) = delete;
        void operator=(const TimeProfiler&) = delete;

        static bool BeginSession(const char* outputFilePath = "Result.json");

        static bool EndSession();

        static bool WriteTimeRecord(const TimeRecord& timeRecord);

        static long long GetTimeStamp();

    protected:
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

    protected:
        TimeRecord timeRecord_;
    };

    class Timer
    {
    public:
        Timer();

        Timer(const Timer& rhs) = delete;
        Timer& operator=(const Timer& rhs) = delete;

        bool Start(const char* label);
        bool Stop();

    protected:
        bool running_;
        TimeRecord timeRecord_;
    };
}