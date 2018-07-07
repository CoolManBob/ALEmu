#include <iostream>
using std::cout;
using std::endl;

#include "AlefLoginClientLogin.h"

#include "AlefMD5Crypto.h"

bool AlefLoginClientLogin::processPacket(AlefSocket& sock, AlefPacket * packet)
{
	switch (packet->GetPacketHeader(true).PacketFlag)
	{
	case 1: //Initial Packet Opcode 0x0D 0x01
		return processInitialLoginPacket(sock, packet); break;
	case 0x3D: //User Login Packet Opcode 0x0D 0x3D
		return processUserLoginPacket(sock, packet); break;
	default:
		return false;
	}
	return false;
}

bool AlefLoginClientLogin::processInitialLoginPacket(AlefSocket& sock, AlefPacket * packet)
{
	AlefPacket response(0x1A, 0x0D, 0x03, 0x00);

	unsigned char hashKey[] = "12345678";
	response.WriteUInt8(0);
	response.WriteByteArray(hashKey);
	response.WriteUInt8(0);
	response.WriteFooter();

	sock.sendPacket(&response);
	return true;
}

bool AlefLoginClientLogin::processUserLoginPacket(AlefSocket& sock, AlefPacket* packet)
{
	AlefMD5Crypto md5Crypt;

	uint32_t acctLen = packet->getBuffer()[0x41]; //Make sure size isn't too large here!
	unsigned char* md5account = new unsigned char[acctLen];
	memcpy(md5account, packet->getBufferAt(0x10), acctLen+1);

	uint32_t pwLen = packet->getBuffer()[0x63]; //Make sure size isn't too large here!
	unsigned char* md5password = new unsigned char[pwLen];
	memcpy(md5password, packet->getBufferAt(0x42), pwLen+1);

	md5Crypt.decryptString(md5account, acctLen);
	md5Crypt.decryptString(md5password, pwLen);

	cout << "Account: " << md5account << " " << "Password: " << md5password << endl;

	AlefPacket signOnResponse(0x4A, 0x0D, 0x05, 0xC0);
	signOnResponse.WriteUInt8(0x01);
	signOnResponse.WriteByteArray("test");
	for (int i = 0; i < 53; i++)
		signOnResponse.WriteUInt8(0);
	signOnResponse.WriteFooter();

	sock.sendPacket(&signOnResponse);

	return true;
}