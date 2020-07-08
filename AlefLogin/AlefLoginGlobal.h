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

#include <Poco/Data/Statement.h>
using Poco::Data::Keywords::now;
using Poco::Data::Keywords::use;
using Poco::Data::Keywords::into;
using Poco::Data::Keywords::bind;

#include "AlefLog.h"
#include "AlefPacketInterface.h"
#include "AlefLoginConfig.h"
#include "AlefDBInterface.h"

#define LOG AlefLogger->Log
#define FATAL AlefLog::LogPrio::LOG_FATAL
#define WARNING AlefLog::LogPrio::LOG_WARNING
#define INFO AlefLog::LogPrio::LOG_INFO

#define LOGINDB AlefDBInterface::dbType::loginDB
#define WORLDDB AlefDBInterface::dbType::worldDB
#define DATADB	AlefDBInterface::dbType::dataDB

//namespace AlefGlobal
//{
	extern AlefLog* AlefLogger;
	extern AlefPacketInterface* pktInterface;
	extern AlefLoginConfig* loginConfig;
	extern AlefDBInterface * dbInterface;
//}

/*namespace AlefLoginConfig
{
	static std::string g_WorldAddress;
}*/