#include "AlefWorldGlobal.h"
#include "AlefWorldUIStatus.h"

bool AlefWorldUIStatus::processPacket(AlefSocket& sock, AlefPacket* packet)
{
    /*{	Alef::INT8, Alef::INT32, Alef::PACKET, Alef::INT8, Alef::PACKET, Alef::MEMORY_BLOCK, Alef::INT32, Alef::INT32, Alef::INT8,
        Alef::INT8, Alef::INT8, Alef::MEMORY_BLOCK }*/
    Int8 i8Operation = 0;
    pktInterface->processPacket(packet, &i8Operation, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    switch (i8Operation)
    {
        default:
        {
            stringstream errorMsg;
            errorMsg << "Character processPacket Unhandled Operation: " << (int)i8Operation;
            LOG(errorMsg.str(), FATAL);
            return false;
        }
    }
    return false;
}