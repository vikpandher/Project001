// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#include "Logger.h"



namespace Project001
{
    const Logger::LineEndMarkerType Logger::endl = {};
    const LogLevelType Logger::DEFAULT_LOG_LEVEL = LOG_LEVEL_INFO;

#ifndef NDEBUG

    std::ostream* Logger::s_outStream = 0;
    LogLevelType Logger::s_enabledDownThroughLevel = Logger::DEFAULT_LOG_LEVEL;

#endif
}