#include "AlefWorldGlobal.h"
#include "AlefWorldCharacter.h"

bool AlefWorldCharacter::processPacket(const localInfo& local)
{
    /*AuPacket::GetField(
        &this->m_csPacket,
        1,
        pvPacket,
        nSize,
        &pvFirst,
        &cOperation,
        &lOptionFlag,
        &unGGLength,
        &cBankSize,
        &szNickName,
        &llBankMoney,
        &lRemainedMurdererTime,
        &pvActionPacket,
        &llMoney,
        &v36,
        &llCash,
        &cFaceIndex,
        &cIsNewChar,
        &ucSocialAnimationIndex,
        &unEventStatusFlag,
        &cCriminalStatus,
        &lLastKilledTimeInBattleSquare,
        &ulSpecialStatus,
        &cActionStatus,
        &unMessageLength,
        &szSkillInit,
        &cIsTransform,
        &pvFactorPacket,
        &llMoneyCount,
        &cHairIndex,
        &cStatus,
        &lRemainedCriminalTime,
        &pvMovePacket,
        &szMessage,
        &ePenalty,
        &pggAuthData);*/
	/*{	Alef::INT8, Alef::INT32, Alef::INT32, Alef::MEMORY_BLOCK, Alef::INT8, Alef::PACKET, Alef::PACKET, Alef::PACKET, 
        Alef::INT64, Alef::INT64, Alef::INT64, Alef::INT8, Alef::INT8, Alef::INT32, Alef::INT8, Alef::UINT8, Alef::UINT8,
        Alef::UINT64, Alef::INT8, Alef::CHAR, Alef::INT8, Alef::INT8, Alef::INT32, Alef::INT8, Alef::UINT16, Alef::INT32,
        Alef::INT32, Alef::CHAR, Alef::MEMORY_BLOCK, Alef::UINT32}*/
    localInfo& localObj = const_cast<localInfo&>(local);
    AlefPacket* packet = localObj.packet;
	Int8 i8Operation = 0;
    SharedPtr<AlefPacket> charMovePkt = nullptr;
	pktInterface->processPacket(packet, &i8Operation, 0, 0, 0, 0, 0, charMovePkt, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	switch (i8Operation)
    {
        case 1:
            return sendCharacterUpdate(localObj); break;
        case 17:
            return sendOptionFlag(localObj); break;
        
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

bool AlefWorldCharacter::sendCharacterUpdate(localInfo& local)
{
    return true;
}

bool AlefWorldCharacter::sendOptionFlag(localInfo& local)
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