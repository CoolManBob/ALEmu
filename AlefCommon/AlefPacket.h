#pragma once
#pragma pack(push,1)

#include <Poco/Foundation.h>

using namespace Poco;

struct AlefPacketHeader
{
	UInt8 GuardByte;
	UInt16 PacketSize;
	UInt8 PacketType;
	UInt64 Flag1;
	UInt8 Flag2;
	UInt8 PacketFlag;
	UInt8 PacketOperation;
};

class AlefPacket
{
public:
	AlefPacket();
	AlefPacket(int initialSize);
	AlefPacket(char* buffer, int bufSize);
	AlefPacket(Int16 packetSize, Int8 PacketType, Int8 PacketFlag, Int8 PacketOp);
	~AlefPacket();

	void Resize(int newSize);

	void WriteHeader(Int16 packetSize, Int8 PacketType, Int8 PacketFlag, Int8 PacketOp);
	void WriteFooter() { WriteInt8((Int8)0x6B); };

	void WriteInt8(Int8 data);
	void WriteInt16(Int16 data);
	void WriteInt32(Int32 data);
	void WriteInt64(Int64 data);

	void WriteUInt8(UInt8 data);
	void WriteUInt16(UInt16 data);
	void WriteUInt32(UInt32 data);
	void WriteUInt64(UInt64 data);

	void WriteByteArray(const char* array);
	void WriteByteArray(const UInt8* array);
	void WriteArbitraryData(const void *data, int len);

	template<typename data>
	void WriteGeneric(const data val);

	AlefPacketHeader GetPacketHeader(bool resetPos = false);

	void GetInt8(Int8 &data);
	void GetInt16(Int16 &data);
	void GetInt32(Int32 &data);
	void GetInt64(Int64 &data);

	void GetUInt8(UInt8 &data);
	void GetUInt16(UInt16 &data);
	void GetUInt32(UInt32 &data);
	void GetUInt64(UInt64 &data);

	void GetInt8(int position, Int8 &data);
	void GetInt16(int position, Int16 &data);
	void GetInt32(int position, Int32 &data);
	void GetInt64(int position, Int64 &data);

	void GetUInt8(int position, UInt8 &data);
	void GetUInt16(int position, UInt16 &data);
	void GetUInt32(int position, UInt32 &data);
	void GetUInt64(int position, UInt64 &data);

	template<typename data>
	void GetGeneric(int position, data &val, int size);

	/*template<typename data>
	void SetDataAt(int position, const data val);*/

	//inline char* getBuffer() { return &buf[pos]; };
	inline char* getBuffer() { return buf; };
	inline char* getBufferAt(int pos) { return &buf[pos]; }
	inline char* getBufferAtCurPos() { return &buf[pos]; }
	inline char getSingleElement(int pos) { return buf[pos]; };
	inline int getPosition() { return pos; };

	inline void setPosition(int newPos) { pos = newPos; };

	inline int getSize() { return size; }

private:
	char * buf;
	int size;
	int pos;

	void EnsureBufSize(int checkSize)
	{
		if (checkSize > size)
		{
			int newSize = size;
			do
			{
				newSize <<= 1;
			} while (newSize < size);

			char *tmp = new char[newSize];
			memcpy(tmp, buf, size);
			delete[] buf;
			buf = tmp;
			size = newSize;
		}
	}

	inline void ValidateReadTo(int position)
	{
		if (position > size)
			throw "Buffer overread";
	}
};