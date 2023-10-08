#include "Logger.h"

#include <cstdio>
#include <cstdarg>
#include <cstring>



namespace Project001
{
    void Logger::DestroyLogger()
    {
        delete s_instance_;
        s_instance_ = nullptr;
    }

    // NOTES:
    // ANSI escape color codes :
    // *these don't work on windows (only in visual studio console)
    // ---------------------------------------------------------------------------
    // https://en.wikipedia.org/wiki/ANSI_escape_code
    // Name            FG  BG
    // Black           30  40
    // Red             31  41
    // Green           32  42
    // Yellow          33  43
    // Blue            34  44
    // Magenta         35  45
    // Cyan            36  46
    // White           37  47
    // Bright Black    90  100
    // Bright Red      91  101
    // Bright Green    92  102
    // Bright Yellow   93  103
    // Bright Blue     94  104
    // Bright Magenta  95  105
    // Bright Cyan     96  106
    // Bright White    97  107
    // Reset                    0
    //
    // ex:
    // printf("\033[30;42mERROR:\033[32;40m %s\033[0m\n", s_charBuffer_);
    // printf("\033[30;47mMESSAGE:\033[0m %s\n", s_charBuffer_);

    // public ------------------------------------------------------------------

    void Logger::Error(const char* format, ...)
    {
        Logger* instance = GetInstance();
        std::lock_guard<std::mutex> lock(s_lock_);

        va_list args;
        va_start(args, format);
        memset(instance->charBuffer_, 0, sizeof(char) * s_charBufferCapacity_);
        vsnprintf(instance->charBuffer_, sizeof(char) * s_charBufferCapacity_, format, args);

        printf("ERROR:      %s\n", instance->charBuffer_);
        fflush(stdout);
    }

    void Logger::Message(const char* format, ...)
    {
        Logger* instance = GetInstance();
        std::lock_guard<std::mutex> lock(s_lock_);

        va_list args;
        va_start(args, format);
        memset(instance->charBuffer_, 0, sizeof(char) * s_charBufferCapacity_);
        vsnprintf(instance->charBuffer_, sizeof(char) * s_charBufferCapacity_, format, args);

        printf("MESSAGE:    %s\n", instance->charBuffer_);
        fflush(stdout);
    }

    Logger::~Logger()
    {
        std::lock_guard<std::mutex> lock(s_lock_);
        delete[] charBuffer_;
    }

    // private -----------------------------------------------------------------

    Logger* Logger::s_instance_ = nullptr;
    std::mutex Logger::s_lock_;

    Logger* Logger::GetInstance()
    {
        std::lock_guard<std::mutex> lock(s_lock_);
        if (s_instance_ == nullptr)
        {
            s_instance_ = new Logger();
        }
        return s_instance_;
    }

    Logger::Logger()
    {
        charBuffer_ = new char[s_charBufferCapacity_];
    }
}