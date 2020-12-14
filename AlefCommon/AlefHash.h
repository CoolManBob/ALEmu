#pragma once
#include <string>

using std::string;

#include <Poco/DigestStream.h>
#include <Poco/SHA2Engine.h>

using Poco::DigestOutputStream;
using Poco::DigestEngine;
using Poco::SHA2Engine;

class AlefHash
{
public:
	AlefHash() {};
	~AlefHash() {};

	static string getHash(string input)
	{
		SHA2Engine sha2;
		DigestOutputStream ostr(sha2);

		ostr << input;
		ostr.flush();

		const DigestEngine::Digest& digest = sha2.digest();

		return DigestEngine::digestToHex(digest);
	}
};