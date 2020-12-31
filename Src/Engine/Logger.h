#pragma once

#include <mutex>



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