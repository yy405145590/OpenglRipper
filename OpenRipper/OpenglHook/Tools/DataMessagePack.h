#pragma once
class DataMessagePack : public MStream
{
public:
	DataMessagePack();
	~DataMessagePack();

private:
	uint32		m_opCode;
};

