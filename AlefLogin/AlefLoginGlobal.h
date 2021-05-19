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
using Poco::Data::Keywords::in;
using Poco::Data::Keywords::out;
using Poco::Data::Keywords::into;
using Poco::Data::Keywords::bind;

#include <Poco/Timestamp.h>
using Poco::Timestamp;

#include <Poco/HMACEngine.h>
#include <Poco/SHA2Engine.h>
using Poco::HMACEngine;
using Poco::SHA2Engine;


#include "AlefLog.h"
#include "AlefLoginConfig.h"
#include "AlefPacketInterface.h"
#include "AlefDBInterface.h"

#define LOG AlefLogger->Log
#define FATAL AlefLog::LogPrio::LOG_FATAL
#define WARNING AlefLog::LogPrio::LOG_WARNING
#define INFO AlefLog::LogPrio::LOG_INFO

#define COLIDX(x) static_cast<int>(x)

#define MAX_CHARACTERS 3

//Global Interfaces
extern AlefLog* AlefLogger;
extern AlefLoginConfig* loginConfig;
extern AlefPacketInterface* pktInterface;
extern AlefDBInterface* dbInterface;
