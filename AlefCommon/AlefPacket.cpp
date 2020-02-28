#include "AlefPacket.h"

AlefPacket::AlefPacket()
{
	buf = nullptr; //Default settings
	size = 0;
	pos = 0;
	dynamic = true;
	numFields = 0;
	flagLength = 0;
	packetType = 0;
	dwMask = 1;
	ulFlag = 0;
}

AlefPacket::AlefPacket(int initialSize)
{
	buf = new unsigned char[initialSize];
	size = initialSize;
	pos = 0;
	dynamic = false;
	numFields = 0;
	flagLength = 0;
	packetType = 0;
	dwMask = 1;
	ulFlag = 0;
}

AlefPacket::AlefPacket(unsigned char *buffer, int bufSize) //Incoming Packets
{
	buf = new unsigned char[bufSize];
	memcpy(buf, buffer, bufSize);
	size = bufSize;
	pos = 0;
	dynamic = false;
	numFields = 0;
	flagLength = 0;
	packetType = 0;
	dwMask = 1;
	ulFlag = 0;
}

AlefPacket::AlefPacket(UInt16 PacketType, UInt8 PacketFlag, UInt8 PacketOp) //Deprecated
{
	buf = new unsigned char[15]; //Allocate initial space for header
	size = 15;
	pos = 0;
	dynamic = true;
	numFields = 0;
	flagLength = 2;
	packetType = PacketType;
	dwMask = 1;
	ulFlag = 0;
	WriteHeader(PacketType, PacketFlag, PacketOp);
}

AlefPacket::AlefPacket(UInt16 PacketType, UInt8 FlagLen) //Outgoing Packets
{
	if (FlagLen != 1 && FlagLen != 2 && FlagLen != 4)
		throw "ERROR";
	buf = new unsigned char[13+FlagLen]; //Allocate initial space for header
	size = 13+FlagLen;
	pos = 0;
	dynamic = true;
	numFields = 0;
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

AlefPacket::AlefPacket(UInt8 FlagLen) //Mini(Internal) Packets
{
	if (FlagLen != 1 && FlagLen != 2 && FlagLen != 4)
		throw "ERROR";
	buf = new unsigned char[2 + FlagLen]; //Allocate initial space for header (UInt16(Size) + FlagLen)
	size = 2 + FlagLen;
	pos = 0;
	dynamic = true;
	numFields = 0;
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
	if(buf)
		delete[] buf;
}

void AlefPacket::WriteHeader(UInt16 PacketType, UInt8 PacketFlag, UInt8 PacketOp) //Deprecated
{
	WriteUInt8(0xD6); //GuardByte - 1Byte D6 is International guard byte
	WriteUInt16(size); //Packetsize
	WriteUInt16(PacketType); //PacketType (Opcode1) - 1Byte
	WriteUInt32(0); //Unk - Flag? - 8Bytes
	WriteUInt32(0); //Unk - Flag? - 1Byte
	WriteUInt8(PacketFlag); //Packet Flag (Opcode2) - 1Byte
	WriteUInt8(PacketOp); //Packet Operation(Opcode3) - 1Byte
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

void AlefPacket::UpdatePacket(UInt16 newSize)
{
	if(dynamic && !isMini)
		*(UInt16*)&buf[0x01] = newSize;
	else if(dynamic && isMini)
		*(UInt16*)&buf[0] = newSize;

	if (headerWritten)
	{
		ulFlag |= dwMask;
		dwMask *= 2;
	}
}

void AlefPacket::ClosePacket()
{
	//Write packetflags
	if(flagLength == 1)
		*(UInt8*)&buf[0x0D] = ulFlag;
	else if(flagLength == 2)
		*(UInt16*)&buf[0x0D] = ulFlag;
	else if(flagLength == 4)
		*(UInt32*)&buf[0x0D] = ulFlag;
	//Write footer
	WriteUInt8(0x6B);
}

void AlefPacket::CloseMiniPacket()
{
	//Write packetflags
	if (flagLength == 1)
		*(UInt8*)&buf[2] = ulFlag;
	else if (flagLength == 2)
		*(UInt16*)&buf[2] = ulFlag;
	else if (flagLength == 4)
		*(UInt32*)&buf[2] = ulFlag;
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

void AlefPacket::Resize(int newSize)
{
	/*if (size == newSize)
		return;
	else if (size < newSize)
		EnsureBufSize(newSize);
	else
	{
		unsigned char *tmp = new unsigned char[newSize];
		memcpy(tmp, buf, newSize);
		delete[] buf;
		buf = tmp;
		size = newSize;
	}*/
	if (size == newSize)
		return;
	else if (size == 0 && buf == nullptr)
	{
		unsigned char* tmp = new unsigned char[newSize];
		memset(tmp, 0, newSize);
		buf = tmp;
		size = newSize;
	}
	else
	{
		unsigned char * tmp = new unsigned char[newSize];
		//memset(tmp, 0, newSize);
		memcpy(tmp, buf, newSize);
		delete[] buf;
		buf = tmp;
		size = newSize;
	}
}

void AlefPacket::WriteInt8(Int8 data)
{
	EnsureBufSize(pos + sizeof(Int8));
	*(Int8*)&buf[pos] = data;
	pos += sizeof(Int8);
	UpdatePacket(size);
}

void AlefPacket::WriteInt16(Int16 data)
{
	EnsureBufSize(pos + sizeof(Int16));
	*(Int16*)&buf[pos] = data;
	pos += sizeof(Int16);
	UpdatePacket(size);
}

void AlefPacket::WriteInt32(Int32 data)
{
	EnsureBufSize(pos + sizeof(Int32));
	*(Int32*)&buf[pos] = data;
	pos += sizeof(Int32);
	UpdatePacket(size);
}

void AlefPacket::WriteInt64(Int64 data)
{
	EnsureBufSize(pos + sizeof(Int64));
	*(Int64*)&buf[pos] = data;
	pos += sizeof(Int64);
	UpdatePacket(size);
}

void AlefPacket::WriteUInt8(UInt8 data)
{
	EnsureBufSize(pos + sizeof(UInt8));
	*(UInt8*)&buf[pos] = data;
	pos += sizeof(UInt8);
	UpdatePacket(size);
}

void AlefPacket::WriteUInt16(UInt16 data)
{
	EnsureBufSize(pos + sizeof(UInt16));
	*(UInt16*)&buf[pos] = data;
	pos += sizeof(UInt16);
	UpdatePacket(size);
}

void AlefPacket::WriteUInt32(UInt32 data)
{
	EnsureBufSize(pos + sizeof(UInt32));
	*(UInt32*)&buf[pos] = data;
	pos += sizeof(UInt32);
	UpdatePacket(size);
}

void AlefPacket::WriteUInt64(UInt64 data)
{
	EnsureBufSize(pos + sizeof(UInt64));
	*(UInt64*)&buf[pos] = data;
	pos += sizeof(UInt64);
	UpdatePacket(size);
}

void AlefPacket::WriteFloat(float data)
{
	EnsureBufSize(pos + sizeof(float));
	*(float*)&buf[pos] = data;
	pos += sizeof(float);
	UpdatePacket(size);
}

void AlefPacket::WriteDouble(double data)
{
	EnsureBufSize(pos + sizeof(double));
	*(double*)&buf[pos] = data;
	pos += sizeof(double);
	UpdatePacket(size);
}

void AlefPacket::WriteMemoryBlock(UInt16 blocksize, const UInt8* data, bool zero)
{
	EnsureBufSize(pos + sizeof(UInt16));
	*(UInt16*)&buf[pos] = blocksize;
	pos += sizeof(UInt16);

	if (zero)
	{
		for (int i = 0; i < blocksize; i++)
		{
			EnsureBufSize(pos + sizeof(UInt8));
			*(UInt8*)&buf[pos] = 0;
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
	memcpy(&buf[pos], data, len);
	pos += len;
	UpdatePacket(size);
}

void AlefPacket::WriteByteArray(const char* array)
{
	WriteArbitraryData(array, (int)(strlen(array)));
	//WriteUInt8((UInt16)(strlen(array)));
}

void AlefPacket::WriteByteArray(const UInt8* array)
{
	WriteArbitraryData(array, (int)(strlen((char*)array)));
	//WriteUInt16((UInt16)(strlen((char*)array)));
}

void AlefPacket::GetInt8(Int8& data)
{
	ValidateReadTo(pos + sizeof(Int8));
	data = *(Int8*)&buf[pos];
	pos += sizeof(Int8);
}

void AlefPacket::GetInt16(Int16& data)
{
	ValidateReadTo(pos + sizeof(Int16));
	data = *(Int16*)&buf[pos];
	pos += sizeof(Int16);
}

void AlefPacket::GetInt32(Int32& data)
{
	ValidateReadTo(pos + sizeof(Int32));
	data = *(Int32*)&buf[pos];
	pos += sizeof(Int32);
}

void AlefPacket::GetInt64(Int64& data)
{
	ValidateReadTo(pos + sizeof(Int64));
	data = *(Int64*)&buf[pos];
	pos += sizeof(Int64);
}

void AlefPacket::GetUInt8(UInt8& data)
{
	ValidateReadTo(pos + sizeof(UInt8));
	data = *(UInt8*)&buf[pos];
	pos += sizeof(UInt8);
}

void AlefPacket::GetUInt16(UInt16& data)
{
	ValidateReadTo(pos + sizeof(UInt16));
	data = *(UInt16*)&buf[pos];
	pos += sizeof(UInt16);
}

void AlefPacket::GetUInt32(UInt32& data)
{
	ValidateReadTo(pos + sizeof(UInt32));
	data = *(UInt32*)&buf[pos];
	pos += sizeof(UInt32);
}

void AlefPacket::GetUInt64(UInt64& data)
{
	ValidateReadTo(pos + sizeof(UInt64));
	data = *(UInt64*)&buf[pos];
	pos += sizeof(UInt64);
}

void AlefPacket::GetFloat(float& data)
{
	ValidateReadTo(pos + sizeof(float));
	data = *(float*)&buf[pos];
	pos += sizeof(float);
}

void AlefPacket::GetDouble(double& data)
{
	ValidateReadTo(pos + sizeof(double));
	data = *(double*)&buf[pos];
	pos += sizeof(double);
}

Int8 AlefPacket::GetInt8()
{
	Int8 temp = 0;
	ValidateReadTo(pos + sizeof(Int8));
	temp = *(Int8*)&buf[pos];
	pos += sizeof(Int8);
	return temp;
}

Int16 AlefPacket::GetInt16()
{
	Int16 temp = 0;
	ValidateReadTo(pos + sizeof(Int16));
	temp = *(Int16*)&buf[pos];
	pos += sizeof(Int16);
	return temp;
}

Int32 AlefPacket::GetInt32()
{
	Int32 temp = 0;
	ValidateReadTo(pos + sizeof(Int32));
	temp = *(Int32*)&buf[pos];
	pos += sizeof(Int32);
	return temp;
}

Int64 AlefPacket::GetInt64()
{
	Int64 temp = 0;
	ValidateReadTo(pos + sizeof(Int64));
	temp = *(Int64*)&buf[pos];
	pos += sizeof(Int64);
	return temp;
}

UInt8 AlefPacket::GetUInt8()
{
	UInt8 temp = 0;
	ValidateReadTo(pos + sizeof(UInt8));
	temp = *(UInt8*)&buf[pos];
	pos += sizeof(UInt8);
	return temp;
}

UInt16 AlefPacket::GetUInt16()
{
	UInt16 temp = 0;
	ValidateReadTo(pos + sizeof(UInt16));
	temp = *(UInt16*)&buf[pos];
	pos += sizeof(UInt16);
	return temp;
}

UInt32 AlefPacket::GetUInt32()
{
	UInt32 temp = 0;
	ValidateReadTo(pos + sizeof(UInt32));
	temp = *(UInt32*)&buf[pos];
	pos += sizeof(UInt32);
	return temp;
}

UInt64 AlefPacket::GetUInt64()
{
	UInt64 temp = 0;
	ValidateReadTo(pos + sizeof(UInt64));
	temp = *(UInt64*)&buf[pos];
	pos += sizeof(UInt64);
	return temp;
}

float AlefPacket::GetFloat()
{
	float temp = 0;
	ValidateReadTo(pos + sizeof(float));
	temp = *(float*)&buf[pos];
	pos += sizeof(float);
	return temp;
}

double AlefPacket::GetDouble()
{
	double temp = 0;
	ValidateReadTo(pos + sizeof(double));
	temp = *(double*)&buf[pos];
	pos += sizeof(double);
	return temp;
}

void AlefPacket::GetInt8(int position, Int8 &data)
{
	ValidateReadTo(position + sizeof(Int8));
	data = *(Int8*)&buf[position];
}

void AlefPacket::GetInt16(int position, Int16 &data)
{
	ValidateReadTo(position + sizeof(Int16));
	data = *(Int16*)&buf[position];
}

void AlefPacket::GetInt32(int position, Int32 &data)
{
	ValidateReadTo(position + sizeof(Int32));
	data = *(Int32*)&buf[position];
}

void AlefPacket::GetInt64(int position, Int64 &data)
{
	ValidateReadTo(position + sizeof(Int64));
	data = *(Int64*)&buf[position];
}

void AlefPacket::GetUInt8(int position, UInt8 &data)
{
	ValidateReadTo(position + sizeof(UInt8));
	data = *(UInt8*)&buf[position];
}

void AlefPacket::GetUInt16(int position, UInt16 &data)
{
	ValidateReadTo(position + sizeof(UInt16));
	data = *(UInt16*)&buf[position];
}

void AlefPacket::GetUInt32(int position, UInt32 &data)
{
	ValidateReadTo(position + sizeof(UInt32));
	data = *(UInt32*)&buf[position];
}

void AlefPacket::GetUInt64(int position, UInt64 &data)
{
	ValidateReadTo(position + sizeof(UInt64));
	data = *(UInt64*)&buf[position];
}

void AlefPacket::GetFloat(int position, float& data)
{
	ValidateReadTo(position + sizeof(float));
	data = *(float*)&buf[position];
}

void AlefPacket::GetDouble(int position, double& data)
{
	ValidateReadTo(position + sizeof(double));
	data = *(double*)&buf[position];
}

void AlefPacket::GetDataBlock(UInt16 blockSize, char* data)
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

template <typename data>
void AlefPacket::WriteGeneric(const data val)
{
	EnsureBufSize(pos + sizeof(data));
	*(data*)&buf[pos] = val;
	pos += sizeof(data);
	UpdatePacket(size);
}

template <typename data>
void AlefPacket::GetGeneric(data& val)
{
	ValidateReadTo(pos + sizeof(data));
	data = *(data*)&buf[pos];
	pos += sizeof(data);
}

template <typename data>
void AlefPacket::GetGeneric(data& val, int size)
{
	ValidateReadTo(pos + size);
	data = *(data*)&buf[pos];
	pos += size;
}

template <typename data>
void AlefPacket::GetGeneric(int position, data &val, int size)
{
	ValidateReadTo(position + sizeof(data));
	val = *(data*)&buf[position];
}