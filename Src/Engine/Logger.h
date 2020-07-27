#pragma once



namespace Project001
{	
	const unsigned int LOGGER_CHAR_BUFFER_SIZE = 512;
	
	class Logger
	{
	public:
		Logger() = delete;
		Logger(const Logger& rhs) = delete;
		Logger& operator=(const Logger& rhs) = delete;

		static void Error(const char* format, ...);

		static void Message(const char* format, ...);

	protected:

	private:
		static char s_charBuffer_[LOGGER_CHAR_BUFFER_SIZE];
	};
}