#include "AlefPacket.h"

AlefPacket::AlefPacket()
{
	buf = new unsigned char[1024]; //Default settings
	size = 1024;
	pos = 0;
}

AlefPacket::AlefPacket(int initialSize)
{
	buf = new unsigned char[initialSize];
	size = initialSize;
	pos = 0;
}

AlefPacket::AlefPacket(unsigned char *buffer, int bufSize)
{
	buf = new unsigned char[bufSize];
	memcpy(buf, buffer, bufSize);
	size = bufSize;
	pos = 0;
}

AlefPacket::AlefPacket(UInt16 PacketSize, UInt8 PacketType, UInt8 PacketOp, UInt8 UnkOp)
{
	buf = new unsigned char[PacketSize];
	size = PacketSize;
	pos = 0;
	WriteHeader(PacketSize, PacketType, PacketOp, UnkOp);
}

AlefPacket::~AlefPacket()
{
	delete[] buf;
}

void AlefPacket::WriteHeader(UInt16 packetSize, UInt8 PacketType, UInt8 PacketFlag, UInt8 PacketOp)
{
	/*Header
	guardByte - 1 Byte (Osy emu has d6 as the guardbyte here every time, is this intentional?)
	packetSize - 2 Bytes
	PacketType - 1 Byte
	Unk - 9 Bytes (IDA says these are flags, Osy server emu never sends anything here initially, client however has data in this field)
	Opcode2 (Packet Flag) - 1 Byte
	Opcode3 (Packet Operation) - 1 Byte
	Total : 15 Bytes(edited)*/
	WriteUInt8(0xD6); //GuardByte - 1Byte D6 is International guard byte
	WriteUInt16(packetSize); //Packetsize
	WriteUInt8(PacketType); //PacketType (Opcode1) - 1Byte
	WriteUInt64(0); //Unk - Flag? - 8Bytes
	WriteUInt8(0); //Unk - Flag? - 1Byte
	WriteUInt8(PacketFlag); //Packet Flag (Opcode2) - 1Byte
	WriteUInt8(PacketOp); //Packet Operation(Opcode3) - 1Byte

}

AlefPacketHeader AlefPacket::GetPacketHeader(bool resetPos)
{
	AlefPacketHeader header;
	GetUInt8(header.GuardByte);
	GetUInt16(header.PacketSize);
	GetUInt8(header.PacketType);
	GetUInt64(header.Flag1);
	GetUInt8(header.Flag2);
	GetUInt8(header.PacketFlag);
	GetUInt8(header.PacketOperation);

	if (resetPos)
		pos -= (sizeof(UInt8) + sizeof(UInt16) + sizeof(UInt8) + sizeof(UInt64) + sizeof(UInt8) + sizeof(UInt8) + sizeof(UInt8));

	return header;
}

void AlefPacket::Resize(int newSize)
{
	if (size == newSize)
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
	}
}

void AlefPacket::WriteInt8(Int8 data)
{
	EnsureBufSize(pos + sizeof(Int8));
	*(Int8*)&buf[pos] = data;
	pos += sizeof(Int8);
}

void AlefPacket::WriteInt16(Int16 data)
{
	EnsureBufSize(pos + sizeof(Int16));
	*(Int16*)&buf[pos] = data;
	pos += sizeof(Int16);
}

void AlefPacket::WriteInt32(Int32 data)
{
	EnsureBufSize(pos + sizeof(Int32));
	*(Int32*)&buf[pos] = data;
	pos += sizeof(Int32);
}

void AlefPacket::WriteInt64(Int64 data)
{
	EnsureBufSize(pos + sizeof(Int64));
	*(Int64*)&buf[pos] = data;
	pos += sizeof(Int64);
}

void AlefPacket::WriteUInt8(UInt8 data)
{
	EnsureBufSize(pos + sizeof(UInt8));
	*(UInt8*)&buf[pos] = data;
	pos += sizeof(UInt8);
}

void AlefPacket::WriteUInt16(UInt16 data)
{
	EnsureBufSize(pos + sizeof(UInt16));
	*(UInt16*)&buf[pos] = data;
	pos += sizeof(UInt16);
}

void AlefPacket::WriteUInt32(UInt32 data)
{
	EnsureBufSize(pos + sizeof(UInt32));
	*(UInt32*)&buf[pos] = data;
	pos += sizeof(UInt32);
}

void AlefPacket::WriteUInt64(UInt64 data)
{
	EnsureBufSize(pos + sizeof(UInt64));
	*(UInt64*)&buf[pos] = data;
	pos += sizeof(UInt64);
}

void AlefPacket::WriteArbitraryData(const void *data, int len)
{
	EnsureBufSize(pos + len);
	memcpy(&buf[pos], data, len);
	pos += len;
}

void AlefPacket::WriteByteArray(const char* array)
{
	WriteArbitraryData(array, (int)(strlen(array)));
}

void AlefPacket::WriteByteArray(const UInt8* array)
{
	WriteArbitraryData(array, (int)(strlen((char*)array)));
}

void AlefPacket::GetInt8(Int8 &data)
{
	ValidateReadTo(pos + sizeof(Int8));
	data = *(Int8*)&buf[pos];
	pos += sizeof(Int8);
}

void AlefPacket::GetInt16(Int16 &data)
{
	ValidateReadTo(pos + sizeof(Int16));
	data = *(Int16*)&buf[pos];
	pos += sizeof(Int16);
}

void AlefPacket::GetInt32(Int32 &data)
{
	ValidateReadTo(pos + sizeof(Int32));
	data = *(Int32*)&buf[pos];
	pos += sizeof(Int32);
}

void AlefPacket::GetInt64(Int64 &data)
{
	ValidateReadTo(pos + sizeof(Int64));
	data = *(Int64*)&buf[pos];
	pos += sizeof(Int64);
}

void AlefPacket::GetUInt8(UInt8 &data)
{
	ValidateReadTo(pos + sizeof(UInt8));
	data = *(UInt8*)&buf[pos];
	pos += sizeof(UInt8);
}

void AlefPacket::GetUInt16(UInt16 &data)
{
	ValidateReadTo(pos + sizeof(UInt16));
	data = *(UInt16*)&buf[pos];
	pos += sizeof(UInt16);
}

void AlefPacket::GetUInt32(UInt32 &data)
{
	ValidateReadTo(pos + sizeof(UInt32));
	data = *(UInt32*)&buf[pos];
	pos += sizeof(UInt32);
}

void AlefPacket::GetUInt64(UInt64 &data)
{
	ValidateReadTo(pos + sizeof(UInt64));
	data = *(UInt64*)&buf[pos];
	pos += sizeof(UInt64);
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

template <typename data>
void AlefPacket::WriteGeneric(const data val)
{
	EnsureBufSize(pos + sizeof(data));
	*(data*)&buf[pos] = val;
	pos += sizeof(data);
}

template <typename data>
void AlefPacket::GetGeneric(int position, data &val, int size)
{
	ValidateReadTo(position + sizeof(data));
	val = *(data*)&buf[position];
}