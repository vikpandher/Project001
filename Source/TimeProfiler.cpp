#include "TimeProfiler.h"



namespace Project001
{
    // TimeProfiler ============================================================

    // public ------------------------------------------------------------------

    bool TimeProfiler::BeginSession(const char* outputFilePath)
    {
        TimeProfiler& instance = GetInstance();
        std::lock_guard<std::mutex> lock(instance.lock_);

        if (!instance.inSession_)
        {
            instance.outputStream_.open(outputFilePath);
            instance.outputStream_ << "{\"otherData\": {},\"traceEvents\":[" << "\n";
            instance.outputStream_.flush();
            instance.inSession_ = true;

            return true;
        }
        else
        {
            return false;
        }
    }

    bool TimeProfiler::EndSession()
    {
        TimeProfiler& instance = GetInstance();
        std::lock_guard<std::mutex> lock(instance.lock_);

        if (instance.inSession_)
        {
            instance.outputStream_ << "\n";
            instance.outputStream_ << "]}" << "\n";
            instance.outputStream_.flush();
            instance.outputStream_.close();
            instance.entryCount_ = 0;
            instance.inSession_ = false;

            return true;
        }
        else
        {
            return false;
        }
    }

    bool TimeProfiler::WriteTimeRecord(const TimeRecord& timeRecord)
    {
        TimeProfiler& instance = GetInstance();
        std::lock_guard<std::mutex> lock(instance.lock_);

        if (instance.inSession_)
        {
            if (instance.entryCount_ > 0)
            {
                instance.outputStream_ << "," << "\n";
            }

            instance.outputStream_ << "{" << "\n";
            instance.outputStream_ << "\"cat\":\"function\"," << "\n";
            instance.outputStream_ << "\"dur\":" << (timeRecord.stop_ns - timeRecord.start_ns) / 1000 << ',' << "\n";
            instance.outputStream_ << "\"name\":\"" << timeRecord.label << "\"," << "\n";
            instance.outputStream_ << "\"ph\":\"X\"," << "\n";
            instance.outputStream_ << "\"pid\":0," << "\n";
            instance.outputStream_ << "\"tid\":" << timeRecord.threadID << "," << "\n";
            instance.outputStream_ << "\"ts\":" << timeRecord.start_ns / 1000 << "\n";
            instance.outputStream_ << "}";
            instance.outputStream_.flush();

            instance.entryCount_++;

            return true;
        }
        else
        {
            return false;
        }
    }

    long long TimeProfiler::GetTimeStamp()
    {
        std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
        std::chrono::nanoseconds duration = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch());
        return duration.count();

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

    // ScopeTimer ==============================================================
    
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

    // Timer ===================================================================

    // public ------------------------------------------------------------------

    Timer::Timer()
        : running_(false)
    {}

    bool Timer::Start(const char* label)
    {
        if (!running_)
        {
            running_ = true;

            timeRecord_.label = label;
            timeRecord_.threadID = std::this_thread::get_id();
            timeRecord_.start_ns = std::chrono::high_resolution_clock::now().time_since_epoch().count();

            return true;
        }
        else
        {
            return false;
        }
    }

    bool Timer::Stop()
    {
        if (running_)
        {
            running_ = false;

            timeRecord_.stop_ns = std::chrono::high_resolution_clock::now().time_since_epoch().count();
            TimeProfiler::WriteTimeRecord(timeRecord_);

            return true;
        }
        else
        {
            return false;
        }
    }
}