#include "AlefPacket.h"

AlefPacket::AlefPacket()
{
	size = 0;
	pos = 0;
	dynamic = true;
	flagLength = 0;
	packetType = 0;
	dwMask = 1;
	ulFlag = 0;
}

AlefPacket::AlefPacket(unsigned char *buffer, int bufSize) : buf(buffer, buffer + bufSize) //Incoming Packets
{
	size = bufSize;
	pos = 0;
	dynamic = false;
	flagLength = 0;
	packetType = 0;
	dwMask = 1;
	ulFlag = 0;
}

AlefPacket::AlefPacket(int PacketType) //Incoming Mini Packets
{
	size = 0;
	pos = 0;
	dynamic = true;
	flagLength = 0;
	packetType = PacketType;
	dwMask = 1;
	ulFlag = 0;
}

AlefPacket::AlefPacket(UInt16 PacketType, UInt8 FlagLen) //Outgoing Packets
{
	if (FlagLen != 1 && FlagLen != 2 && FlagLen != 4)
		throw "ERROR";
	buf.resize(13 + FlagLen); //Allocate initial space for header
	size = 13+FlagLen;
	pos = 0;
	dynamic = true;
	flagLength = FlagLen;
	packetType = PacketType;
	dwMask = 1;
	ulFlag = 0;
	guardByte = 0xD6;
	sessionID = 0;
	timestamp = 0;
	packetSize = 0;
	packetFlags = new UInt8[FlagLen];
	headerWritten = false;
	isMini = false;

	WriteHeader();
}

AlefPacket::AlefPacket(UInt8 FlagLen) //Outgoing Mini(Internal) Packets
{
	if (FlagLen != 1 && FlagLen != 2 && FlagLen != 4)
		throw "ERROR";
	buf.resize(2 + FlagLen); //Allocate initial space for header (UInt16(Size) + FlagLen)
	size = 2 + FlagLen;
	pos = 0;
	dynamic = true;
	flagLength = FlagLen;
	packetType = 0;
	dwMask = 1;
	ulFlag = 0;
	guardByte = 0xD6;
	sessionID = 0;
	timestamp = 0;
	packetSize = 0;
	packetFlags = new UInt8[FlagLen];
	headerWritten = false;
	isMini = true;

	WriteMiniHeader();
}

AlefPacket::~AlefPacket()
{		
	if (packetFlags)
		delete[] packetFlags;

	buf.clear();
	fields.clear();
}

void AlefPacket::WriteHeader()
{
	WriteUInt8(0xD6); //GuardByte
	WriteUInt16(size);
	WriteUInt16(packetType);
	WriteUInt32(sessionID);
	WriteUInt32(timestamp);

	/*Flag Setup*/
	for (int i = 0; i < flagLength; i++)
		WriteUInt8(0); //Temporary zero, will be overwritten when we close the packet.

	headerWritten = true; //Flag UpdatePacket to start building flag information
}

void AlefPacket::WriteMiniHeader()
{
	WriteUInt16(size);

	//Setup flag space for header, then flag the system to allow it to update flags
	for (int i = 0; i < flagLength; i++)
		WriteUInt8(0); //Temporary zero, will be overwritten when we close the packet.

	headerWritten = true;
}

bool AlefPacket::setFieldInfo(int types[], int typeSize, int sizes[], int sizeSz)
{
	if (typeSize != sizeSz)
		return false; //Type and Size Elements are not matched!

	for (int i = 0; i < typeSize; i++)
	{
		Alef::AlefPktField field;
		field.FieldType = types[i];
		field.FieldSize = sizes[i];
		fields.push_back(field);
	}

	return true;
}

bool AlefPacket::setFieldInfo(vector<Alef::AlefPktField> fieldVec)
{
	return false;
}

void AlefPacket::UpdatePacket(UInt16 newSize, bool doFlagUpd)
{
	if(dynamic && !isMini)
		memcpy(&buf[0x01], &newSize, sizeof(UInt16));
	else if (dynamic && isMini)
	{
		UInt16 miniSize = newSize - 2;
		memcpy(&buf[0], &miniSize, sizeof(UInt16)); //UInt16 packetSize is not counted for miniPackets
	}

	if (headerWritten && doFlagUpd)
		ulFlag |= dwMask;
}

void AlefPacket::ClosePacket()
{
	//Write packetflags
	if(flagLength == 1)
		memcpy(&buf[0x0D], &ulFlag, sizeof(UInt8));
	else if(flagLength == 2)
		memcpy(&buf[0x0D], &ulFlag, sizeof(UInt16));
	else if(flagLength == 4)
		memcpy(&buf[0x0D], &ulFlag, sizeof(UInt32));
	//Write footer
	WriteUInt8(0x6B);
}

void AlefPacket::CloseMiniPacket()
{
	//Write packetflags
	if (flagLength == 1)
		memcpy(&buf[2], &ulFlag, sizeof(UInt8));
	else if (flagLength == 2)
		memcpy(&buf[2], &ulFlag, sizeof(UInt16));
	else if (flagLength == 4)
		memcpy(&buf[2], &ulFlag, sizeof(UInt32));
}

void AlefPacket::acquirePacketHeader()
{
	GetUInt8(guardByte);
	GetUInt16(packetSize);
	GetUInt16(packetType);
	GetUInt32(sessionID);
	GetUInt32(timestamp);
}

bool AlefPacket::setAndAcquireFlags(UInt8 flagLen)
{
	if (flagLen != 1 && flagLen != 2 && flagLen != 4)
		return false;

	flagLength = flagLen;
	
	packetFlags = new UInt8[flagLen];

	for (int i = 0; i < flagLen; i++)
		GetUInt8(packetFlags[i]);

	return true;
}

bool AlefPacket::ResetFromPkt(AlefPacket* pkt)
{
	buf.clear();

	if (packetFlags)
		delete[] packetFlags;

	if (pkt->buf.size() != 0)
	{
		buf.resize(pkt->size);
		buf = pkt->buf;
		size = pkt->size;
		pos = pkt->pos;
		flagLength = pkt->flagLength;
		packetFlags = new UInt8[flagLength];
		memcpy(packetFlags, pkt->packetFlags, flagLength);
		fields = pkt->fields;
		return true;
	}
	else
		return false;
}

void AlefPacket::Resize(int newSize)
{
	if (size == newSize)
		return;
	
	buf.resize(newSize);
	size = newSize;
}

void AlefPacket::WriteInt8(Int8 data)
{
	EnsureBufSize(pos + sizeof(Int8));
	memcpy(&buf[pos], &data, sizeof(Int8));
	pos += sizeof(Int8);
	UpdatePacket(size);
}

void AlefPacket::WriteInt16(Int16 data)
{
	EnsureBufSize(pos + sizeof(Int16));
	memcpy(&buf[pos], &data, sizeof(Int16));
	pos += sizeof(Int16);
	UpdatePacket(size);
}

void AlefPacket::WriteInt32(Int32 data)
{
	EnsureBufSize(pos + sizeof(Int32));
	memcpy(&buf[pos], &data, sizeof(Int32));
	pos += sizeof(Int32);
	UpdatePacket(size);
}

void AlefPacket::WriteInt64(Int64 data)
{
	EnsureBufSize(pos + sizeof(Int64));
	memcpy(&buf[pos], &data, sizeof(Int64));
	pos += sizeof(Int64);
	UpdatePacket(size);
}

void AlefPacket::WriteUInt8(UInt8 data)
{
	EnsureBufSize(pos + sizeof(UInt8));
	memcpy(&buf[pos], &data, sizeof(UInt8));
	pos += sizeof(UInt8);
	UpdatePacket(size);
}

void AlefPacket::WriteUInt16(UInt16 data, bool flagUpd)
{
	EnsureBufSize(pos + sizeof(UInt16));
	memcpy(&buf[pos], &data, sizeof(UInt16));
	pos += sizeof(UInt16);
	UpdatePacket(size, flagUpd);
}

void AlefPacket::WriteUInt32(UInt32 data)
{
	EnsureBufSize(pos + sizeof(UInt32));
	memcpy(&buf[pos], &data, sizeof(UInt32));
	pos += sizeof(UInt32);
	UpdatePacket(size);
}

void AlefPacket::WriteUInt64(UInt64 data)
{
	EnsureBufSize(pos + sizeof(UInt64));
	memcpy(&buf[pos], &data, sizeof(UInt64));
	pos += sizeof(UInt64);
	UpdatePacket(size);
}

void AlefPacket::WriteFloat(float data)
{
	EnsureBufSize(pos + sizeof(float));
	memcpy(&buf[pos], &data, sizeof(float));
	pos += sizeof(float);
	UpdatePacket(size);
}

void AlefPacket::WriteDouble(double data)
{
	EnsureBufSize(pos + sizeof(double));
	memcpy(&buf[pos], &data, sizeof(double));
	pos += sizeof(double);
	UpdatePacket(size);
}

void AlefPacket::WriteMemoryBlock(UInt16 blocksize, const UInt8* data, bool zero)
{
	EnsureBufSize(pos + sizeof(UInt16));
	memcpy(&buf[pos], &blocksize, sizeof(UInt16));
	pos += sizeof(UInt16);

	if (zero)
	{
		for (int i = 0; i < blocksize; i++)
		{
			EnsureBufSize(pos + sizeof(UInt8));
			UInt8 aZero = 0;
			memcpy(&buf[pos], &aZero, sizeof(UInt8));
			pos += sizeof(UInt8);
		}
	}
	else
	{
		int datalen = (int)(strlen((char*)data));
		EnsureBufSize(pos + datalen);
		memcpy(&buf[pos], data, datalen);
		pos += datalen;
	}

	UpdatePacket(size);
}

void AlefPacket::WriteVec3F(Alef::AlefVec3F vec3F)
{
	WriteFloat(vec3F.x);
	WriteFloat(vec3F.y);
	WriteFloat(vec3F.z);
}

void AlefPacket::WriteArbitraryData(const void *data, int len)
{
	EnsureBufSize(pos + len);
	memset(&buf[pos], 0, len);
	memcpy(&buf[pos], data, (int)(strlen((const char*)data)));
	pos += len;
	UpdatePacket(size);
}

void AlefPacket::WritePacket(AlefPacket* packet)
{
	int pktSize = packet->getSize();
	unsigned char* pktData = packet->getBuffer();
	EnsureBufSize(pos + pktSize);
	memset(&buf[pos], 0, pktSize);
	memcpy(&buf[pos], pktData, pktSize);
	pos += pktSize;
	UpdatePacket(size);
}

void AlefPacket::WritePacket(SharedPtr<AlefPacket> packet)
{
	int pktSize = packet->getSize();
	unsigned char* pktData = packet->getBuffer();
	EnsureBufSize(pos + pktSize);
	memset(&buf[pos], 0, pktSize);
	memcpy(&buf[pos], pktData, pktSize);
	pos += pktSize;
	UpdatePacket(size);
}

void AlefPacket::WriteByteArray(const char* array)
{
	WriteArbitraryData(array, (int)(strlen(array)));
}

void AlefPacket::WriteByteArray(const UInt8* array)
{
	WriteArbitraryData(array, (int)(strlen((char*)array)));
}

void AlefPacket::GetInt8(Int8& data)
{
	ValidateReadTo(pos + sizeof(Int8));
	memcpy(&data, &buf[pos], sizeof(Int8));
	pos += sizeof(Int8);
}

void AlefPacket::GetInt16(Int16& data)
{
	ValidateReadTo(pos + sizeof(Int16));
	memcpy(&data, &buf[pos], sizeof(Int16));
	pos += sizeof(Int16);
}

void AlefPacket::GetInt32(Int32& data)
{
	ValidateReadTo(pos + sizeof(Int32));
	memcpy(&data, &buf[pos], sizeof(Int32));
	pos += sizeof(Int32);
}

void AlefPacket::GetInt64(Int64& data)
{
	ValidateReadTo(pos + sizeof(Int64));
	memcpy(&data, &buf[pos], sizeof(Int64));
	pos += sizeof(Int64);
}

void AlefPacket::GetUInt8(UInt8& data)
{
	ValidateReadTo(pos + sizeof(UInt8));
	memcpy(&data, &buf[pos], sizeof(UInt8));
	pos += sizeof(UInt8);
}

void AlefPacket::GetUInt16(UInt16& data)
{
	ValidateReadTo(pos + sizeof(UInt16));
	memcpy(&data, &buf[pos], sizeof(UInt16));
	pos += sizeof(UInt16);
}

void AlefPacket::GetUInt32(UInt32& data)
{
	ValidateReadTo(pos + sizeof(UInt32));
	memcpy(&data, &buf[pos], sizeof(UInt32));
	pos += sizeof(UInt32);
}

void AlefPacket::GetUInt64(UInt64& data)
{
	ValidateReadTo(pos + sizeof(UInt64));
	memcpy(&data, &buf[pos], sizeof(UInt64));
	pos += sizeof(UInt64);
}

void AlefPacket::GetFloat(float& data)
{
	ValidateReadTo(pos + sizeof(float));
	memcpy(&data, &buf[pos], sizeof(float));
	pos += sizeof(float);
}

void AlefPacket::GetDouble(double& data)
{
	ValidateReadTo(pos + sizeof(double));
	memcpy(&data, &buf[pos], sizeof(double));
	pos += sizeof(double);
}

Int8 AlefPacket::GetInt8()
{
	Int8 temp = 0;
	ValidateReadTo(pos + sizeof(Int8));
	memcpy(&temp, &buf[pos], sizeof(Int8));
	pos += sizeof(Int8);
	return temp;
}

Int16 AlefPacket::GetInt16()
{
	Int16 temp = 0;
	ValidateReadTo(pos + sizeof(Int16));
	memcpy(&temp, &buf[pos], sizeof(Int16));
	pos += sizeof(Int16);
	return temp;
}

Int32 AlefPacket::GetInt32()
{
	Int32 temp = 0;
	ValidateReadTo(pos + sizeof(Int32));
	memcpy(&temp, &buf[pos], sizeof(Int32));
	pos += sizeof(Int32);
	return temp;
}

Int64 AlefPacket::GetInt64()
{
	Int64 temp = 0;
	ValidateReadTo(pos + sizeof(Int64));
	memcpy(&temp, &buf[pos], sizeof(Int64));
	pos += sizeof(Int64);
	return temp;
}

UInt8 AlefPacket::GetUInt8()
{
	UInt8 temp = 0;
	ValidateReadTo(pos + sizeof(UInt8));
	memcpy(&temp, &buf[pos], sizeof(UInt8));
	pos += sizeof(UInt8);
	return temp;
}

UInt16 AlefPacket::GetUInt16()
{
	UInt16 temp = 0;
	ValidateReadTo(pos + sizeof(UInt16));
	memcpy(&temp, &buf[pos], sizeof(UInt16));
	pos += sizeof(UInt16);
	return temp;
}

UInt32 AlefPacket::GetUInt32()
{
	UInt32 temp = 0;
	ValidateReadTo(pos + sizeof(UInt32));
	memcpy(&temp, &buf[pos], sizeof(UInt32));
	pos += sizeof(UInt32);
	return temp;
}

UInt64 AlefPacket::GetUInt64()
{
	UInt64 temp = 0;
	ValidateReadTo(pos + sizeof(UInt64));
	memcpy(&temp, &buf[pos], sizeof(UInt64));
	pos += sizeof(UInt64);
	return temp;
}

float AlefPacket::GetFloat()
{
	float temp = 0;
	ValidateReadTo(pos + sizeof(float));
	memcpy(&temp, &buf[pos], sizeof(float));
	pos += sizeof(float);
	return temp;
}

double AlefPacket::GetDouble()
{
	double temp = 0;
	ValidateReadTo(pos + sizeof(double));
	memcpy(&temp, &buf[pos], sizeof(double));
	pos += sizeof(double);
	return temp;
}

void AlefPacket::GetInt8(int position, Int8 &data)
{
	ValidateReadTo(position + sizeof(Int8));
	memcpy(&data, &buf[position], sizeof(Int8));
}

void AlefPacket::GetInt16(int position, Int16 &data)
{
	ValidateReadTo(position + sizeof(Int16));
	memcpy(&data, &buf[position], sizeof(Int16));
}

void AlefPacket::GetInt32(int position, Int32 &data)
{
	ValidateReadTo(position + sizeof(Int32));
	memcpy(&data, &buf[position], sizeof(Int32));
}

void AlefPacket::GetInt64(int position, Int64 &data)
{
	ValidateReadTo(position + sizeof(Int64));
	memcpy(&data, &buf[position], sizeof(Int64));
}

void AlefPacket::GetUInt8(int position, UInt8 &data)
{
	ValidateReadTo(position + sizeof(UInt8));
	memcpy(&data, &buf[position], sizeof(UInt8));
}

void AlefPacket::GetUInt16(int position, UInt16 &data)
{
	ValidateReadTo(position + sizeof(UInt16));
	memcpy(&data, &buf[position], sizeof(UInt16));
}

void AlefPacket::GetUInt32(int position, UInt32 &data)
{
	ValidateReadTo(position + sizeof(UInt32));
	memcpy(&data, &buf[position], sizeof(UInt32));
}

void AlefPacket::GetUInt64(int position, UInt64 &data)
{
	ValidateReadTo(position + sizeof(UInt64));
	memcpy(&data, &buf[position], sizeof(UInt64));
}

void AlefPacket::GetFloat(int position, float& data)
{
	ValidateReadTo(position + sizeof(float));
	memcpy(&data, &buf[position], sizeof(float));
}

void AlefPacket::GetDouble(int position, double& data)
{
	ValidateReadTo(position + sizeof(double));
	memcpy(&data, &buf[position], sizeof(double));
}

void AlefPacket::GetDataBlock(UInt16 blockSize, char* data)
{
	ValidateReadTo(pos + blockSize);
	memcpy(data, &buf[pos], blockSize);
	pos += blockSize;
}

void AlefPacket::GetDataBlock(UInt16 blockSize, unsigned char* data)
{
	ValidateReadTo(pos + blockSize);
	memcpy(data, &buf[pos], blockSize);
	pos += blockSize;
}

void AlefPacket::GetVec3F(Alef::AlefVec3F& vec3F)
{
	GetFloat(vec3F.x);
	GetFloat(vec3F.y);
	GetFloat(vec3F.z);
}

Alef::AlefVec3F AlefPacket::GetVec3F()
{
	Alef::AlefVec3F vec3F;
	GetFloat(vec3F.x);
	GetFloat(vec3F.y);
	GetFloat(vec3F.z);
	return vec3F;
}

template <typename data>
void AlefPacket::WriteGeneric(const data val)
{
	EnsureBufSize(pos + sizeof(data));
	memcpy(&buf[pos], &val, sizeof(data));
	pos += sizeof(data);
	UpdatePacket(size);
}

template <typename data>
void AlefPacket::GetGeneric(data& val)
{
	ValidateReadTo(pos + sizeof(data));
	memcpy(&val, &buf[pos], sizeof(data));
	pos += sizeof(data);
}

template <typename data>
void AlefPacket::GetGeneric(data& val, int size)
{
	ValidateReadTo(pos + size);
	memcpy(&val, &buf[pos], size);
	pos += size;
}

template <typename data>
void AlefPacket::GetGeneric(int position, data &val, int size)
{
	ValidateReadTo(position + sizeof(data));
	memcpy(&val, &buf[position], sizeof(data));
}

void AlefPacket::dumpPacket()
{
	ofstream out("packetDump.txt", std::ios::app);

	out << std::hex << std::endl;

	for (vector<UInt8>::iterator itr = buf.begin(); itr != buf.end(); itr++)
	{
		out  << (int)*itr << " ";
	}
}