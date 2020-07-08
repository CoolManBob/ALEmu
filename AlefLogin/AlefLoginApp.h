#pragma once

#include <vector>
#include <string>
using std::vector;
using std::string;

#include "Poco/Util/ServerApplication.h"

using namespace Poco::Util;

#include "AlefLoginServer.h"

class AlefLoginApp : public ServerApplication
{
public:
	AlefLoginApp() {};
	~AlefLoginApp() {};

protected:
	int main(const vector<string>&);

	void initDatabase();
};