#pragma once

typedef struct NET_PACKET_HEADER
{
	UINT16	_size;
	UINT16	_opCode;
	UINT32	_crc;
}NetPacketHeader;

#define PACKET_HEADER_SIZE sizeof(NET_PACKET_HEADER)
class NetPacket : public MemoryStream<8192>
{
public:
	NetPacket(UINT16 opCode);
	~NetPacket();
	void AppendBuffer(const char* buf, UINT32 nSize);
	UINT16 GetOpCode()
	{
		return m_opCode;
	}
private:
	UINT16	m_opCode;
};
