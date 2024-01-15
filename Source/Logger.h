#pragma once

#include <mutex>


// static_assert(true, "") is at the end of macros so they require a semi-colon
// maybe add exit(1); to _FAIL_CHECK(x)

#ifdef _MSC_VER 
#define __FILENAME__ strrchr("\\" __FILE__, '\\') + 1
#elif
#define __FILENAME__ strrchr("/" __FILE__, '/') + 1
#endif

#ifndef NDEBUG
#define _LOG_ERROR(...) Project001::Logger::Error("%s %s %d", __FILENAME__ , __FUNCTION__, __LINE__); Project001::Logger::Error(__VA_ARGS__)
#define _LOG_MESSAGE(...) Project001::Logger::Message(__VA_ARGS__)
#define _FAIL_CHECK(x) if (!(x)) {_LOG_ERROR(#x);} static_assert(true, "")
#define _DESTROY_LOGGER() Project001::Logger::DestroyLogger()
#else
#define _LOG_ERROR(...)
#define _LOG_MESSAGE(...)
#define _FAIL_CHECK(x) x
#define _DESTROY_LOGGER()
#endif



namespace Project001
{
    class Logger
    {
    public:
        static void DestroyLogger();

        static void Error(const char* format, ...);

        static void Message(const char* format, ...);

        ~Logger();

        Logger(const Logger&) = delete;
        void operator=(const Logger&) = delete;

    protected:

    private:
        static Logger* GetInstance();

        Logger();

        static Logger* s_instance_;
        static std::mutex s_lock_;

        static const unsigned int s_charBufferCapacity_ = 1024;
        char* charBuffer_;
    };
}