#pragma once

#include <mutex>



#ifdef _DEBUG
#define LogError(...) Project001::Logger::Error(__VA_ARGS__)
#define LogMessage(...) Project001::Logger::Message(__VA_ARGS__)
#else
#define LogError(...)
#define LogMessage(...)
#endif



namespace Project001
{
    class Logger
    {
    public:
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

        static const unsigned int s_charBufferCapacity_ = 128;
        char* charBuffer_;
    };
}