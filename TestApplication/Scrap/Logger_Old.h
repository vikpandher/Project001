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
#define _LOG_ERROR(...) Project001::Logger_Old::Error("%s %s %d", __FILENAME__ , __FUNCTION__, __LINE__); Project001::Logger_Old::Error(__VA_ARGS__)
#define _LOG_MESSAGE(...) Project001::Logger_Old::Message(__VA_ARGS__)
#define _FAIL_CHECK(x) if (!(x)) {_LOG_ERROR(#x);} static_assert(true, "")
#define _DESTROY_LOGGER() Project001::Logger_Old::DestroyLogger()
#else
#define _LOG_ERROR(...)
#define _LOG_MESSAGE(...)
#define _FAIL_CHECK(x) x
#define _DESTROY_LOGGER()
#endif



namespace Project001
{
    class Logger_Old
    {
    public:
        static void DestroyLogger();

        static void Error(const char* format, ...);

        static void Message(const char* format, ...);

        ~Logger_Old();

        Logger_Old(const Logger_Old&) = delete;
        void operator=(const Logger_Old&) = delete;

    protected:

    private:
        static Logger_Old* GetInstance();

        Logger_Old();

        static Logger_Old* s_instance_;
        static std::mutex s_lock_;

        static const unsigned int s_charBufferCapacity_ = 1024;
        char* charBuffer_;
    };
}