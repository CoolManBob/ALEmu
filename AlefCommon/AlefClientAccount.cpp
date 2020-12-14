#include <Windows.h>
#include <wincrypt.h>

#include "Poco/RandomStream.h"
using Poco::RandomInputStream;

#include "AlefClientAccount.h"

AlefClientAccount::AlefClientAccount()
{
	data.acctID = -1;
	data.acctUser = "LOCALRESERVED";
	data.acctPw = "LOCALRESERVED";
	data.acctStatus = -1;
}

AlefClientAccount::~AlefClientAccount()
{

}

string AlefClientAccount::generateHashKey()
{
	RandomInputStream ri;
	std::string rs;
	ri >> rs;
	rs.resize(8);
	//For some reason using a size other than 8 will yield unusable results. Data remains garbled.
	//It's not really important to fix this issue, since the protocol is fundamentally broken anyways.

	setHashKey(rs);

	return rs;
}

bool AlefClientAccount::decryptUserInfo(string& user, Int32 userLen, string& pw, Int32 pwLen)
{
	if (!decrypt((UInt8*)user.c_str(), userLen))
		return false;
	if (!decrypt((UInt8*)pw.c_str(), pwLen))
		return false;

	data.acctUser = user;
	data.acctPw = pw;

	return true;
}

bool AlefClientAccount::decrypt(UInt8* out, DWORD len)
{
	string key = getHashKey();
	HCRYPTPROV cryptProv;
	HCRYPTKEY cryptKey;
	HCRYPTHASH cryptHash;

	CryptSetProviderA("Microsoft Base Cryptographic Provider v1.0", 1u);
	if (!CryptAcquireContextA(&cryptProv, 0, "Microsoft Base Cryptographic Provider v1.0", 1u, 0xF0000000))
		return false;
	if (!CryptCreateHash(cryptProv, 0x8003u, 0, 0, &cryptHash))
		return false;
	if (!CryptHashData(cryptHash, (const UInt8*)key.c_str(), 8, 0))
		return false;
	if (!CryptDeriveKey(cryptProv, 0x6801u, cryptHash, 4u, &cryptKey))
		return false;
	if (!CryptDecrypt(cryptKey, 0, 0, 0, out, &len))
		return false;
	CryptDestroyKey(cryptKey);
	CryptDestroyHash(cryptHash);

	return true;
}