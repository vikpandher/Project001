#pragma once

#include <mutex>



namespace Project001
{	
	const unsigned int LOGGER_CHAR_BUFFER_SIZE = 512;
	
	class Logger
	{
	public:
		~Logger();

		Logger(const Logger&) = delete;
		void operator=(const Logger&) = delete;

		static void Error(const char* format, ...);

		static void Message(const char* format, ...);

	protected:

	private:
		static Logger& GetInstance();

		Logger();
		//Logger(const Logger&);
		//void operator=(const Logger&);

		std::mutex lock_;
		char charBuffer_[LOGGER_CHAR_BUFFER_SIZE];
	};
}