#pragma once



namespace Project001
{	
	const unsigned int LOGGER_CHAR_BUFFER_SIZE = 512;
	
	class Logger
	{
	public:
		Logger();
		~Logger();

		void message(const char* format, ...);

	protected:

	private:
		Logger(const Logger& rhs);
		Logger& operator=(const Logger& rhs) { return *this; }

		char charBuffer_[LOGGER_CHAR_BUFFER_SIZE];
	};
}