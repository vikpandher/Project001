
#include "Logger.h"

#include <cstdio>
#include <cstdarg>
#include <cstring>


namespace Project001
{
	Logger::Logger()
	{
		memset(charBuffer_, 0, LOGGER_CHAR_BUFFER_SIZE);
	}

	Logger::~Logger()
	{

	}

	void Logger::message(const char* format, ...)
	{		
		va_list args;
		va_start(args, format);
		vsnprintf(charBuffer_, sizeof(charBuffer_), format, args);

		// ANSI escape color codes :
		// Name            BG  FG
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

		printf("\033[30;47mMESSAGE:\033[0m %s\n", charBuffer_);
	}
}