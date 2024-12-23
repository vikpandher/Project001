// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#pragma once

namespace Project001
{
    typedef enum LogLevelType
    {
        LOG_LEVEL_NONE,
        LOG_LEVEL_ERROR,
        LOG_LEVEL_WARNING,
        LOG_LEVEL_INFO
    } LogLevelType;
}

#ifndef NDEBUG

#include <iostream>



#define FILE_LINE_FUNC __FILE__ << " (" << __LINE__ << ") " << __func__
#define INDENT "         "

#define LOG_ERROR(x)   Project001::Logger(Project001::LOG_LEVEL_ERROR)   << "ERROR:   " x << Project001::Logger::endl
#define LOG_WARNING(x) Project001::Logger(Project001::LOG_LEVEL_WARNING) << "WARNING: " x << Project001::Logger::endl
#define LOG_INFO(x)    Project001::Logger(Project001::LOG_LEVEL_INFO)    << "INFO:    " x << Project001::Logger::endl

#define LOG_ERROR_F(x)   LOG_ERROR(FILE_LINE_FUNC << ":\n" << INDENT << x)
#define LOG_WARNING_F(x) LOG_WARNING(FILE_LINE_FUNC << ":\n" << INDENT << x)
#define LOG_INFO_F(x)    LOG_INFO(FILE_LINE_FUNC << ":\n" << INDENT << x)

#define FAIL_CHECK(x) if (!(x)) {LOG_ERROR_F(#x);} do {} while(0)

#else

#include <string>



#define FILE_LINE_FUNC ""
#define INDENT ""

#define LOG_ERROR(x)   do {} while(0)
#define LOG_WARNING(x) do {} while(0)
#define LOG_INFO(x)    do {} while(0)

#define LOG_ERROR_F(x)   do {} while(0)
#define LOG_WARNING_F(x) do {} while(0)
#define LOG_INFO_F(x)    do {} while(0)

#define FAIL_CHECK(x) x

#endif

namespace Project001
{
    // This class is a simple logger utility that wraps an output stream. It's
    // enabled via a call to EnableConsole(). If compiled with the NDEBUG
    // preprocessor macro, then its interfaces are compiled as no-ops.
    //
    // The basic intended usage via macro (prefered):
    //
    // LOG_DEBUG("foo" << bar);
    //
    // or, directly instantiating:
    //
    // Logger(LOG_LEVEL_INFO) << "foo" << bar << Logger::endl;
    //
    // An intance of the logger can be kept around and used repeatedly:
    //
    // Logger myLog(LOG_LEVEL_INFO);
    //
    // myLog << "foo" << bar << Logger::endl;
    // myLog << baz;
    // myLog << Logger::endl;
    //
    class Logger
    {
    public:
        // This empty struct is a distinct type denoting the end of a line.
        struct LineEndMarkerType {};

        // Static end of line marker
        static const LineEndMarkerType endl;

        // Static default log level
        static const LogLevelType DEFAULT_LOG_LEVEL;

#ifndef NDEBUG
        // Constructor
        Logger(LogLevelType level)
            : outStream_(s_outStream)
            , level_(level)
        {}

        static void SetLogLevel(LogLevelType level)
        {
            s_enabledDownThroughLevel = level;
        }

        // Enable streaming to the given stream
        static void SetOutStream(std::ostream* outStream)
        {
            s_outStream = outStream;
        }

        // Enable streaming to std::cout
        static void EnableConsole()
        {
            s_outStream = &std::cout;
        }

        // Ends a line and flushes the stream, if the stream has been enabled
        void Flush()
        {
            if (outStream_ && level_ <= s_enabledDownThroughLevel)
            {
                *outStream_ << std::endl;
            }
        }

        // Stream the given parameter to the output stream if enabled
        template <typename T>
        Logger& operator<<(const T& parameter)
        {
            if (outStream_ && level_ <= s_enabledDownThroughLevel)
            {
                *outStream_ << parameter;
            }

            return *this;
        }

        // Streams the end of line marker to the stream, flushing iter_swap
        Logger& operator<<(const LineEndMarkerType)
        {
            Flush();

            return *this;
        }

#else

        Logger(LogLevelType) {}

        static void SetLogLevel(LogLevelType) {}

        static void SetOutStream(void*) {}

        static void EnableConsole() {}

        void Flush() {}

        template <typename T>
        Logger& operator<<(const T&)
        {
            return *this;
        }

#endif

        static LogLevelType SeverityStringToEnumType(std::string logLevel)
        {
            for (char& character : logLevel)
            {
                if (character >= 'a' && character <= 'z')
                {
                    character -= 32;
                }
            }

            LogLevelType result = LOG_LEVEL_NONE;

            if (logLevel.compare("ERROR") == 0)
            {
                result = LOG_LEVEL_ERROR;
            }
            else if (logLevel.compare("WARNING") == 0)
            {
                result = LOG_LEVEL_WARNING;
            }
            else if (logLevel.compare("INFO") == 0)
            {
                result = LOG_LEVEL_INFO;
            }

            return result;
        }

    private:

#ifndef NDEBUG
        // The statically configured output stream which new logger instances copy
        // on construction
        static std::ostream* s_outStream;

        // The level through which log messagesare currently enabled.
        static LogLevelType s_enabledDownThroughLevel;

        // Non-owning pointer to an output stream for the logger instance
        std::ostream* outStream_;

        // The severity level at which this instance logs
        LogLevelType level_;

#endif
    };
}