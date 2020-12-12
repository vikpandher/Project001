#pragma once

#include <mutex>



namespace Project001
{		
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

		static const unsigned int s_charBufferCapacity_ = 128;
		char* charBuffer_;
	};
}