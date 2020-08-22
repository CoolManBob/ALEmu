#pragma once

#include <wincrypt.h>

#include "Poco/Foundation.h"
using namespace Poco;

class AlefMD5Crypto
{
public:
	AlefMD5Crypto() 
	{
	};
	~AlefMD5Crypto() {};

	bool encryptString(char* keyStr, unsigned char* out, int size)
	{
		/*HCRYPTKEY cryptKey;
		HCRYPTHASH cryptHash;

		if (!CryptCreateHash(cryptProv, 0x8003u, 0, 0, &cryptHash))
			return false;
		int keySize = strlen(keyStr);
		if (!CryptHashData(cryptHash, (const UInt8*)keyStr, keySize, 0))
			return false;
		if (!CryptDeriveKey(cryptProv, 0x6801u, cryptHash, 4u, &cryptKey))
			return false;
		CryptDestroyHash(cryptHash);
		cryptHash = 0;
		if (!CryptEncrypt(cryptKey, 0, 0, 0, out, (DWORD*)&size, size))
			return false;
		CryptDestroyKey(cryptKey);

		return true;*/
	}
	bool decryptString(char* out, DWORD size)
	{
		HCRYPTPROV cryptProv;
		HCRYPTKEY cryptKey;
		HCRYPTHASH cryptHash;

		CryptSetProviderA("Microsoft Base Cryptographic Provider v1.0", 1u);
		if (!CryptAcquireContextA(&cryptProv, 0, "Microsoft Base Cryptographic Provider v1.0", 1u, 0xF0000000))
			return false;
		if (!CryptCreateHash(cryptProv, 0x8003u, 0, 0, &cryptHash))
			return false;
		if (!CryptHashData(cryptHash, (const UInt8*)"12345678", 8, 0))
			return false;
		if (!CryptDeriveKey(cryptProv, 0x6801u, cryptHash, 4u, &cryptKey))
			return false;
		if (!CryptDecrypt(cryptKey, 0, 0, 0, (unsigned char*)out, &size))
			return false;
		CryptDestroyKey(cryptKey);
		CryptDestroyHash(cryptHash);

		return true;
	}
};