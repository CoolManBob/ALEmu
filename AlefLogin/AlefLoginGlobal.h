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
#include "AlefLoginConfig.h"
#include "AlefPacketInterface.h"
#include "AlefDBInterface.h"

#define LOG AlefLogger->Log
#define FATAL AlefLog::LogPrio::LOG_FATAL
#define WARNING AlefLog::LogPrio::LOG_WARNING
#define INFO AlefLog::LogPrio::LOG_INFO

//Global Interfaces
extern AlefLog* AlefLogger;
extern AlefLoginConfig* loginConfig;
extern AlefPacketInterface* pktInterface;
extern AlefDBInterface* dbInterface;

#include "AlefServerLoginSys.h"
#include "AlefServerEncryptionSys.h"
#include "AlefServerWorldListSys.h"


//Server Systems
extern AlefServerLoginSys* serverLoginSys;
extern AlefServerEncryptionSys* serverEncryptionSys;
extern AlefServerWorldListSys* serverListSys;
