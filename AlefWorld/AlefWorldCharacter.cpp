#include "AlefWorldGlobal.h"
#include "AlefWorldCharacter.h"

bool AlefWorldCharacter::processPacket(AlefSocket& sock, AlefPacket* packet)
{
	/*{	Alef::INT8, Alef::INT32, Alef::INT32, Alef::MEMORY_BLOCK, Alef::INT8, Alef::PACKET, Alef::PACKET, Alef::PACKET, 
        Alef::INT64, Alef::INT64, Alef::INT64, Alef::INT8, Alef::INT8, Alef::INT32, Alef::INT8, Alef::UINT8, Alef::UINT8,
        Alef::UINT64, Alef::INT8, Alef::CHAR, Alef::INT8, Alef::INT8, Alef::INT32, Alef::INT8, Alef::UINT16, Alef::INT32,
        Alef::INT32, Alef::CHAR, Alef::MEMORY_BLOCK, Alef::UINT32}*/
	Int8 i8Operation = 0;
    SharedPtr<AlefPacket> charMovePkt = nullptr;
	pktInterface->processPacket(packet, &i8Operation, 0, 0, 0, 0, 0, charMovePkt, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	switch (i8Operation)
    {
        case 1:
            return sendCharacterUpdate(sock, packet); break;
        case 17:
            return sendOptionFlag(sock, packet); break;
        
        default:
        {
            stringstream errorMsg;
            errorMsg << "Character processPacket Unhandled Operation: " << (int)i8Operation;
            LOG(errorMsg.str(), FATAL);
            return false;
        }
	}
	return true;
}

bool AlefWorldCharacter::sendCharacterUpdate(AlefSocket& sock, AlefPacket* packet)
{
    return true;
}

bool AlefWorldCharacter::sendOptionFlag(AlefSocket& sock, AlefPacket* packet)
{
    /*return AuPacket::MakePacket(
           &this->m_csPacket,
           1,
           pnPacketLength,
           2,
           &pcsCharacter,
           v4,
           0,
           0,
           0,
           0,
           0,
           0,
           0,
           0,
           0,
           0,
           0,
           0,
           0,
           0,
           0,
           0,
           0,
           0,
           0,
           0,
           &lOptionFlag,
           0,
           0,
           0,
           0,
           0,
           0,
           0);*/
    Int8 i8Operation = 17;
    Int32 i32CID = 1012;
    


    return true;
}