#include "AlefWorldGlobal.h"
#include "AlefWorldOptimizedCharMove.h"

bool AlefWorldOptimizedCharMove::processPacket(const localInfo& local)
{
    //{	Alef::INT32, Alef::VEC3F, Alef::VEC3F, Alef::INT32, Alef::UINT16, Alef::INT8, Alef::INT8, Alef::INT8, Alef::INT32 }
    localInfo& localObj = const_cast<localInfo&>(local);
    AlefPacket* packet = localObj.packet;
    Int8 i8Operation = 0;
    //SharedPtr<AlefPacket> charMovePkt = nullptr;
    /*pktInterface->processPacket(packet, &i8Operation, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    switch (i8Operation)
    {
        default:
        {
            stringstream errorMsg;
            errorMsg << "Character processPacket Unhandled Operation: " << (int)i8Operation;
            LOG(errorMsg.str(), FATAL);
            return false;
        }
    }*/
    return true;
}