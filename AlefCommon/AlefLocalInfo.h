#pragma once

#define _localSock local.sock
#define _localPkt local.packet
#define _localSys local.localSys
#define _curChar local.localSys->_localChar()

#include "AlefPacket.h"
#include "AlefSocket.h"
#include "AlefLocalSys.h"

struct localInfo
{
	localInfo(AlefSocket& socket) : sock(socket), packet(nullptr), localSys(nullptr) {};
	localInfo(AlefLocalSys* local, AlefSocket& socket) : localSys(local), sock(socket), packet(nullptr) {};
	AlefLocalSys* localSys;
	AlefPacket* packet;
	AlefSocket& sock;
};