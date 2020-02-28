#pragma once

#include <string>
using std::string;

#include <Poco/Logger.h>
#include <Poco/AutoPtr.h>
#include <Poco/FileChannel.h>
#include <Poco/FormattingChannel.h>
#include <Poco/PatternFormatter.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/WindowsConsoleChannel.h>
#include <Poco/Message.h>
using Poco::Logger;
using Poco::AutoPtr;
using Poco::FormattingChannel;
using Poco::FileChannel;
using Poco::PatternFormatter;
using Poco::ColorConsoleChannel;
using Poco::WindowsColorConsoleChannel;
using Poco::Message;

//TODO: Further extend the logging framework to be able to log to both file and console
class AlefLog
{

public:
	AlefLog();
	AlefLog(string logName, string channelName);
	AlefLog(string logName, int logChannel);
	AlefLog(string logName, int logChannel, int logPrio);
	~AlefLog() {};

	enum class LogPrio : int
	{
		LOG_INFO = 0,
		LOG_WARNING = 1,
		LOG_FATAL = 2
	};

	Logger& AcquireLogger(string logName) {};
	void test();
	void Log(string msg, LogPrio prio = LogPrio::LOG_INFO);

	void FLog(string msg); //Logs to file
	void CLog(string msg, LogPrio prio = LogPrio::LOG_INFO); //Logs to console

private:
	Logger* internalFileLogger;
	Logger* internalConsoleLogger;
};