#pragma once

#include <vector>
#include <string>
using std::vector;
using std::string;

#include "Poco/Util/ServerApplication.h"

using namespace Poco::Util;

#include "AlefWorldServer.h"

class AlefWorldApp : public ServerApplication
{
public:
	AlefWorldApp() {};
	~AlefWorldApp() {};

protected:
	int main(const vector<string>&);
};