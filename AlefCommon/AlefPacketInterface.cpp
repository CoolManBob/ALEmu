#include "AlefPacketInterface.h"

AlefPacketInterface::AlefPacketInterface()
{

}

AlefPacketInterface::~AlefPacketInterface()
{

}

AlefPacket * AlefPacketInterface::buildPacket(UInt16 packetType, ...)
{
	va_list args;
	va_start(args, packetType);
	int dwMask = 1;
	//Using the packetType we get the field data for this packet. Then we create a new AlefPacket, and build it using the variadic arugment list.
	
	UInt8 flagLen = flagLookup.lookUp(packetType);
	if (flagLen == 0xFF)
		return nullptr;

	AlefPacket * response = new AlefPacket(packetType, flagLen);

	bool fieldsOkay = fieldLookup.getFieldInfo(response->GetFieldVec(), packetType);

	if (!fieldsOkay)
		return nullptr;

	for (vector<Alef::AlefPktField>::iterator itr = response->GetFieldVec().begin(); itr != response->GetFieldVec().end(); itr++) //iterate through our fields
	{
		switch (itr->FieldType)
		{
			case Alef::AlefType::CHAR: //Assumes character data is already setup with proper sizing before passing into buildPacket
			{
				char* arg = va_arg(args, char*);
				if (arg)
					response->WriteArbitraryData(arg, itr->FieldSize);
			} break;
			case Alef::AlefType::INT8:
			{
				for (int i = 0; i < itr->FieldSize; i++)
				{
					Int8* arg = va_arg(args, Int8*);
					if(arg)
						response->WriteInt8(*arg);
				}
			} break;
			case Alef::AlefType::UINT8:
			{
				for (int i = 0; i < itr->FieldSize; i++)
				{
					UInt8* arg = va_arg(args, UInt8*);
					if(arg)
						response->WriteUInt8(*arg);
				}
			} break;
			case Alef::AlefType::INT16:
			{
				for (int i = 0; i < itr->FieldSize; i++)
				{
					Int16* arg = va_arg(args, Int16*);
					if(arg)
						response->WriteInt16(*arg);
				}
			} break;
			case Alef::AlefType::UINT16:
			{
				for (int i = 0; i < itr->FieldSize; i++)
				{
					UInt16* arg = va_arg(args, UInt16*);
					if(arg)
						response->WriteUInt16(*arg);
				}
			} break;
			case Alef::AlefType::INT32:
			{
				for (int i = 0; i < itr->FieldSize; i++)
				{
					Int32* arg = va_arg(args, Int32*);
					if(arg)
						response->WriteInt32(*arg);
				}
			} break;
			case Alef::AlefType::UINT32:
			{
				for (int i = 0; i < itr->FieldSize; i++)
				{
					UInt32* arg = va_arg(args, UInt32*);
					if(arg)
						response->WriteUInt32(*arg);
				}
			} break;
			case Alef::AlefType::INT64:
			{
				for (int i = 0; i < itr->FieldSize; i++)
				{
					Int64* arg = va_arg(args, Int64*);
					if(arg)
						response->WriteInt64(*arg);
				}
			} break;
			case Alef::AlefType::UINT64:
			{
				for (int i = 0; i < itr->FieldSize; i++)
				{
					UInt64* arg = va_arg(args, UInt64*);
					if(arg)
						response->WriteUInt64(*arg);
				}
			} break;
			case Alef::AlefType::FLOAT:
			{
				for (int i = 0; i < itr->FieldSize; i++)
				{
					float* arg = va_arg(args, float*);
					if(arg)
						response->WriteFloat(*arg);
				}
			} break;
			case Alef::AlefType::VEC3F:
			{
				for (int i = 0; i < itr->FieldSize; i++)
				{
					Alef::AlefVec3F* arg = va_arg(args, Alef::AlefVec3F*);
					if(arg)
						response->WriteVec3F(*arg);
				}
			} break;
			case Alef::AlefType::MATRIX:
			{

			} break;
			case Alef::AlefType::PACKET:
			{
				for (int i = 0; i < itr->FieldSize; i++)
				{
					AlefPacket* arg = va_arg(args, AlefPacket*);

					if (arg)
						response->WritePacket(arg);
				}
			} break;
			case Alef::AlefType::MEMORY_BLOCK:
			{
				for (int i = 0; i < itr->FieldSize; i++)
				{
					UInt16* arg_sz = va_arg(args, UInt16*);
					if(arg_sz)
						response->WriteUInt16(*arg_sz, false);

				
					char* arg = va_arg(args, char*);
					if(arg && arg_sz)
						response->WriteArbitraryData(arg, *arg_sz);
				}
			} break;
			case Alef::AlefType::POS_BASEMETER:
			{

			} break;
			case Alef::AlefType::WCHAR:
			{

			} break;
		}

		dwMask *= 2;
		response->UpdateMask(dwMask);

	}
	
	response->ClosePacket();
	va_end(args);
	return response;
}

AlefPacket* AlefPacketInterface::buildMiniPacket(UInt16 miniType, ...)
{
	va_list args;
	va_start(args, miniType);
	int dwMask = 1;
	//Using the packetType we get the field data for this packet. Then we create a new AlefPacket, and build it using the variadic arugment list.
	
	UInt8 flagLen = flagLookup.lookUp(miniType);
	if (flagLen == 0xFF)
		return nullptr;

	AlefPacket* response = new AlefPacket(flagLen);

	bool fieldsOkay = fieldLookup.getFieldInfo(response->GetFieldVec(), miniType);

	if (!fieldsOkay)
		return nullptr;

	for (vector<Alef::AlefPktField>::iterator itr = response->GetFieldVec().begin(); itr != response->GetFieldVec().end(); itr++) //iterate through our fields
	{
		switch (itr->FieldType)
		{
			case Alef::AlefType::CHAR: //Assumes character data is already setup with proper sizing before passing into buildPacket
			{
				char* arg = va_arg(args, char*);
				if (arg)
					response->WriteArbitraryData(arg, itr->FieldSize);
			} break;
			case Alef::AlefType::INT8:
			{
				for (int i = 0; i < itr->FieldSize; i++)
				{
					Int8* arg = va_arg(args, Int8*);
					if (arg)
						response->WriteInt8(*arg);
				}
			} break;
			case Alef::AlefType::UINT8:
			{
				for (int i = 0; i < itr->FieldSize; i++)
				{
					UInt8* arg = va_arg(args, UInt8*);
					if (arg)
						response->WriteUInt8(*arg);
				}
			} break;
			case Alef::AlefType::INT16:
			{
				for (int i = 0; i < itr->FieldSize; i++)
				{
					Int16* arg = va_arg(args, Int16*);
					if (arg)
						response->WriteInt16(*arg);
				}
			} break;
			case Alef::AlefType::UINT16:
			{
				for (int i = 0; i < itr->FieldSize; i++)
				{
					UInt16* arg = va_arg(args, UInt16*);
					if (arg)
						response->WriteUInt16(*arg);
				}
			} break;
			case Alef::AlefType::INT32:
			{
				for (int i = 0; i < itr->FieldSize; i++)
				{
					Int32* arg = va_arg(args, Int32*);
					if (arg)
						response->WriteInt32(*arg);
				}
			} break;
			case Alef::AlefType::UINT32:
			{
				for (int i = 0; i < itr->FieldSize; i++)
				{
					UInt32* arg = va_arg(args, UInt32*);
					if (arg)
						response->WriteUInt32(*arg);
				}
			} break;
			case Alef::AlefType::INT64:
			{
				for (int i = 0; i < itr->FieldSize; i++)
				{
					Int64* arg = va_arg(args, Int64*);
					if (arg)
						response->WriteInt64(*arg);
				}
			} break;
			case Alef::AlefType::UINT64:
			{
				for (int i = 0; i < itr->FieldSize; i++)
				{
					UInt64* arg = va_arg(args, UInt64*);
					if (arg)
						response->WriteUInt64(*arg);
				}
			} break;
			case Alef::AlefType::FLOAT:
			{
				for (int i = 0; i < itr->FieldSize; i++)
				{
					float* arg = va_arg(args, float*);
					if (arg)
						response->WriteFloat(*arg);
				}
			} break;
			case Alef::AlefType::VEC3F:
			{
				for (int i = 0; i < itr->FieldSize; i++)
				{
					Alef::AlefVec3F* arg = va_arg(args, Alef::AlefVec3F*);
					if (arg)
						response->WriteVec3F(*arg);
				}
			} break;
			case Alef::AlefType::MATRIX:
			{

			} break;
			case Alef::AlefType::PACKET:
			{
				for (int i = 0; i < itr->FieldSize; i++)
				{
					AlefPacket* arg = va_arg(args, AlefPacket*);

					if (arg)
						response->WritePacket(arg);
				}
			} break;
			case Alef::AlefType::MEMORY_BLOCK:
			{
				for (int i = 0; i < itr->FieldSize; i++)
				{
					UInt16* arg_sz = va_arg(args, UInt16*);
					if (arg_sz)
						response->WriteUInt16(*arg_sz, false);


					char* arg = va_arg(args, char*);
					if (arg && arg_sz)
						response->WriteArbitraryData(arg, *arg_sz);
				}
			} break;
			case Alef::AlefType::POS_BASEMETER:
			{

			} break;
			case Alef::AlefType::WCHAR:
			{

			} break;
		}

		dwMask *= 2;
		response->UpdateMask(dwMask);
	}

	response->CloseMiniPacket();
	va_end(args);
	return response;
}

bool AlefPacketInterface::processPacket(AlefPacket* packet, ...)
{
	//Packet should've already been setup at this point. Retrieve data using already defined field type data, and place into variadic argument list
	UInt32	pktFlag = 0;
	UInt32	readFlag = 1U << 0;
	va_list args;

	va_start(args, packet);
	switch (packet->GetFlagLength())
	{
		case 1:
			pktFlag = *(UInt8*)packet->GetPacketFlags(); break;
		case 2:
			pktFlag = *(UInt16*)packet->GetPacketFlags(); break;
		case 4:
			pktFlag = *(UInt32*)packet->GetPacketFlags(); break;
		default:
			pktFlag = 0; break;
	}

	for (vector<Alef::AlefPktField>::iterator itr = packet->GetFieldVec().begin(); itr != packet->GetFieldVec().end(); itr++, readFlag <<= 1) //iterate through our fields
	{
		if (readFlag > pktFlag)
			break;

		void* arg = va_arg(args, void*);

		if (pktFlag & readFlag)
		{
			switch (itr->FieldType)
			{
				case Alef::AlefType::CHAR:
				{
					//char * arg = va_arg(args, char*);
					if (arg)
					{
						UInt32 charSize = itr->FieldSize; //Is the size ALWAYS static?

						char* temp = new char[charSize];
						packet->GetDataBlock(charSize, temp);

						/*UInt8* arg_sz = va_arg(args, UInt8*);
						UInt8  size = 0;
						if (arg_sz)
						{
							packet->GetUInt8(*arg_sz);
							size = *arg_sz;
						}*/

						//if (arg)
							memcpy(arg, temp, charSize);

						delete[] temp;
					}

					
				} break;
				case Alef::AlefType::INT8:
				{
					for (int i = 0; i < itr->FieldSize; i++)
					{
						//Int8* arg = va_arg(args, Int8*);
						if (arg)
							*(((Int8*)arg)) = packet->GetInt8();
					}
				} break;
				case Alef::AlefType::UINT8:
				{
					for (int i = 0; i < itr->FieldSize; i++)
					{
						//UInt8* arg = va_arg(args, UInt8*);
						if(arg)
							*(((UInt8*)arg)) = packet->GetUInt8();
					}
				} break;
				case Alef::AlefType::INT16:
				{
					for (int i = 0; i < itr->FieldSize; i++)
					{
						//Int16* arg = va_arg(args, Int16*);
						if(arg)
							*(((Int16*)arg)) = packet->GetInt16();
					}
				} break;
				case Alef::AlefType::UINT16:
				{
					for (int i = 0; i < itr->FieldSize; i++)
					{
						//UInt16* arg = va_arg(args, UInt16*);
						if(arg)
							*(((UInt16*)arg)) = packet->GetUInt16();
					}
				} break;
				case Alef::AlefType::INT32:
				{
					for (int i = 0; i < itr->FieldSize; i++)
					{
						//Int32* arg = va_arg(args, Int32*);
						if(arg)
							*(((Int32*)arg)) = packet->GetInt32();
					}
				} break;
				case Alef::AlefType::UINT32:
				{
					for (int i = 0; i < itr->FieldSize; i++)
					{
						//UInt32* arg = va_arg(args, UInt32*);
						if(arg)
							*(((UInt32*)arg)) = packet->GetUInt32();
					}
				} break;
				case Alef::AlefType::INT64:
				{
					for (int i = 0; i < itr->FieldSize; i++)
					{
						//Int64* arg = va_arg(args, Int64*);
						if(arg)
							*(((Int64*)arg)) = packet->GetInt64();
					}
				} break;
				case Alef::AlefType::UINT64:
				{
					for (int i = 0; i < itr->FieldSize; i++)
					{
						UInt64* arg = va_arg(args, UInt64*);
						if(arg)
							*(((UInt64*)arg)) = packet->GetUInt64();
					}
				} break;
				case Alef::AlefType::FLOAT:
				{
					for (int i = 0; i < itr->FieldSize; i++)
					{
						//float* arg = va_arg(args, float*);
						if(arg)
							*(((float*)arg)) = packet->GetFloat();
					}
				} break;
				case Alef::AlefType::VEC3F:
				{
					for (int i = 0; i < itr->FieldSize; i++)
					{
						Alef::AlefVec3F* arg = va_arg(args, Alef::AlefVec3F*);
						if(arg)
							packet->GetVec3F(*arg);
					}
				} break;
				case Alef::AlefType::MATRIX:
				{
				
				} break;
				case Alef::AlefType::PACKET:
				{
					for (int i = 0; i < itr->FieldSize; i++)
					{
						UInt16* arg_sz = va_arg(args, UInt16*);
						UInt16	size = 0;
						if (arg_sz)
						{
							packet->GetUInt16(*arg_sz);
							size = *arg_sz;
						}

						AlefPacket* arg = va_arg(args, AlefPacket*);
						
						if (arg && arg_sz)
						{
							char* pktData = new char[size];

							packet->GetDataBlock(size, pktData);

							AlefPacket* temp = new AlefPacket((unsigned char*)pktData, (int)size);

							delete[] pktData;

							arg = temp;
						}
					}

					//TODO: Extend for more than one packet within the field
				} break;
				case Alef::AlefType::MEMORY_BLOCK:
				{
					for (int i = 0; i < itr->FieldSize; i++)
					{
						//UInt16* arg_sz = va_arg(args, UInt16*);
						UInt16  blocksize;
						packet->GetUInt16(blocksize);;

						/*if (arg_sz)
							*arg_sz = blocksize;*/

						//char* arg = va_arg(args, char*);
						char* temp = new char[blocksize];

						packet->GetDataBlock(blocksize, temp);
						if (arg)
							memcpy(arg, temp, blocksize);
					}
				} break;
				case Alef::AlefType::POS_BASEMETER:
				{

				} break;
				case Alef::AlefType::WCHAR:
				{

				} break;
			}
		}
		else
		{
			switch (itr->FieldType)
			{
				case Alef::AlefType::PACKET:
				{
					AlefPacket* arg = va_arg(args, AlefPacket*);
					if(arg)
						arg = nullptr;
				} break;
				case Alef::AlefType::MEMORY_BLOCK:
				{
					UInt16* arg = va_arg(args, UInt16*);
				} break;
			}
		}
	}

	va_end(args);
	return true;
}

bool AlefPacketInterface::setupPkt(AlefPacket* packet)
{
	//call acquireHeader() from packet pointer,then use packettype to set the flaglength, and field type data.
	packet->acquirePacketHeader();
	bool flagsOkay = packet->setAndAcquireFlags(flagLookup.lookUp(packet->GetPacketType())); //This needs to be simplified somehow

	if (!flagsOkay)
		return false;

	//Get FieldInfo
	bool fieldsOkay = fieldLookup.getFieldInfo(packet->GetFieldVec(), packet->GetPacketType());

	if (!fieldsOkay)
		return false;

	return true;
}