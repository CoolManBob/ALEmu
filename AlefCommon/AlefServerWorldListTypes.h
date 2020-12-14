#pragma once

#include <vector>
#include <string>
using std::vector;
using std::string;

struct WorldListWorld
{
	int worldID;
	string worldName;
	string worldAddress;
	int worldStatus;
	int worldFlag;
};
typedef vector<SharedPtr<WorldListWorld>> worldListWorldVec;

struct WorldListGroup
{
	int groupID;
	string groupName;
	worldListWorldVec worldVec;
};
typedef vector<SharedPtr<WorldListGroup>> worldListGroupVec;

