#pragma once
//#pragma pack(push,1)

#include <Poco/SharedPtr.h>

using Poco::SharedPtr;

#include <vector>

using std::vector;

#include "AlefTypes.h"

enum FlagLength
{
	FLAG_LEN1 = 1,
	FLAG_LEN2 = 2,
	FLAG_LEN4 = 4,
};

class AlefPacket
{
public:
	AlefPacket();
	AlefPacket(int initialSize);
	AlefPacket(unsigned char* buffer, int bufSize);
	AlefPacket(UInt16 PacketType, UInt8 FlagLen);
	AlefPacket(UInt8 FlagLen);
	
	~AlefPacket();

	void Resize(int newSize);
	bool ResetFromPkt(AlefPacket* packet);

	void WriteHeader();
	void WriteMiniHeader();

	void UpdatePacket(UInt16 newSize, bool doFlagUpd = true);
	void UpdateMask(int newMask) { dwMask = newMask; }

	void ClosePacket();
	void CloseMiniPacket();

	void WriteInt8(Int8 data);
	void WriteInt16(Int16 data);
	void WriteInt32(Int32 data);
	void WriteInt64(Int64 data);

	void WriteUInt8(UInt8 data);
	void WriteUInt16(UInt16 data, bool flagUpd = true);
	void WriteUInt32(UInt32 data);
	void WriteUInt64(UInt64 data);

	void WriteFloat(float data);
	void WriteDouble(double data);

	void WriteMemoryBlock(UInt16 size, const UInt8* data, bool zero = false);
	void WriteVec3F(Alef::AlefVec3F vec3F);
	//void WriteMiniPacket(AlefPacket* packet);

	void WriteByteArray(const char* array);
	void WriteByteArray(const UInt8* array);

	void WriteArbitraryData(const void *data, int len);

	void WritePacket(AlefPacket* packet);
	void WritePacket(SharedPtr<AlefPacket> packet);

	template<typename data>
	void WriteGeneric(const data val);

	/*Public Header Operations*/

	void	acquirePacketHeader();
	UInt16	GetPacketType() { return packetType; }
	UInt16	GetPacketSize() { return packetSize; }
	UInt32	GetPacketSessionID() { return sessionID; }
	UInt32	GetPacketTimestamp() { return timestamp; }
	
	UInt8	GetFlagLength() { return flagLength; }
	UInt8	GetPacketFlag(UInt8 flagIndex) { if (flagIndex > flagLength) return 0xFF; return packetFlags[flagIndex]; }
	UInt8*	GetPacketFlags() { return packetFlags; }
	void setFlagLength(UInt8 flagLen) { flagLength = flagLen; };
	bool setAndAcquireFlags(UInt8 flagLen);

	vector<Alef::AlefPktField>& GetFieldVec() { return fields; }
	/*End Public Header Operations*/

	bool setFieldInfo(int types[], int typeSize, int sizes[], int sizeSz);
	bool setFieldInfo(vector<Alef::AlefPktField> fieldVec);
	
	Int8 GetInt8();
	Int16 GetInt16();
	Int32 GetInt32();
	Int64 GetInt64();
	UInt8 GetUInt8();
	UInt16 GetUInt16();
	UInt32 GetUInt32();
	UInt64 GetUInt64();

	float GetFloat();
	double GetDouble();

	void GetInt8(Int8& data);
	void GetInt16(Int16& data);	
	void GetInt32(Int32& data);
	void GetInt64(Int64& data);
	void GetUInt8(UInt8& data);	
	void GetUInt16(UInt16& data);	
	void GetUInt32(UInt32& data);
	void GetUInt64(UInt64& data);

	void GetFloat(float& data);
	void GetDouble(double& data);

	void GetInt8(int position, Int8 &data);
	void GetInt16(int position, Int16 &data);
	void GetInt32(int position, Int32 &data);
	void GetInt64(int position, Int64 &data);

	void GetUInt8(int position, UInt8 &data);
	void GetUInt16(int position, UInt16 &data);
	void GetUInt32(int position, UInt32 &data);
	void GetUInt64(int position, UInt64 &data);

	void GetFloat(int position, float &data);
	void GetDouble(int position, double &data);

	void GetDataBlock(UInt16 blocksize, char* data);
	void GetDataBlock(UInt16 blocksize, unsigned char* data);
	void GetVec3F(Alef::AlefVec3F& vec3F);
	Alef::AlefVec3F GetVec3F();
	
	template<typename data>
	void GetGeneric(data& val);

	template<typename data>
	void GetGeneric(data& val, int size);

	template<typename data>
	void GetGeneric(int position, data &val, int size);

	/*template<typename data>
	void SetDataAt(int position, const data val);*/

	//inline char* getBuffer() { return &buf[pos]; };
	inline unsigned char*	getBuffer() { return buf; };
	inline unsigned char*	getBufferAt(int pos) { return &buf[pos]; }
	inline unsigned char*	getBufferAtCurPos() { return &buf[pos]; }
	inline unsigned char	getSingleElement(int pos) { return buf[pos]; };
	inline int				getPosition() { return pos; };

	inline void	setPosition(int newPos) { pos = newPos; };
	inline void setSize(int newSize) { size = newSize; }

	inline int getSize() { return size; }

private:
	unsigned char * buf;
	int size;
	int pos;
	bool dynamic;

	/*Packet Info*/

	/*Header*/
	
	UInt8 guardByte;
	UInt16 packetSize;
	UInt16 packetType;
	UInt32 sessionID;
	UInt32 timestamp;
	UInt8* packetFlags; //Notes: Based on the PacketType, it will have some hardcoded "FlagLength" associated with it that defines the end of the header, the flaglength can be 1, 2, or 4 bytes.
	
	/*End Header Info*/

	Int32 numFields;
	UInt8 flagLength;
	vector<Alef::AlefPktField> fields;

	int ulFlag;
	int dwMask;
	bool headerWritten;
	bool isMini;

	/*End Packet Info*/

	/*void EnsureBufSize(int checkSize)
	{
		if (checkSize > size)
		{
			int newSize = size;
			do
			{
				newSize <<= 1;
			} while (newSize < size);

			unsigned char *tmp = new unsigned char[newSize];
			memcpy(tmp, buf, size);
			delete[] buf;
			buf = tmp;
			size = newSize;
		}
	}*/

	void EnsureBufSize(int checkSize)
	{
		if (checkSize > size)
			Resize(checkSize);
		else
			return;
	}

	inline void ValidateReadTo(int position)
	{
		if (position > size)
			throw "Buffer overread";
	}
};