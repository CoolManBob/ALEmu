#pragma once
//File used for global variables if any
#include <iostream>
#include <string>
#include <sstream>
using std::cout;
using std::endl;
using std::string;
using std::stringstream;

#include <Poco/SharedPtr.h>
using Poco::SharedPtr;

#include "AlefPacketInterface.h"
#include "AlefLog.h"

#define LOG AlefLogger->Log
#define FATAL AlefLog::LogPrio::LOG_FATAL
#define WARNING AlefLog::LogPrio::LOG_WARNING
#define INFO AlefLog::LogPrio::LOG_INFO

//namespace AlefGlobal
//{
	extern AlefLog* AlefLogger;
	extern AlefPacketInterface* pktInterface;
//}