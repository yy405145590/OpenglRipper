#pragma once

#include "../Reactor/Iocp/IocpReactor.h"
#include "IoPeerBase.h"
#define READ_RING_BUFFER_SIZE 32 * 1024
class IoPeerIocpTcp:public IoPeerBase
{
public:
	IoPeerIocpTcp();
	virtual ~IoPeerIocpTcp();

	virtual void DoRead();
	virtual void DoWrite();
	virtual void OnReadComplete(UINT32  nSize, IO_DATA* pOverlap);
	virtual void OnWriteComplete(UINT32  nSize, IO_DATA* pOverlap);
	virtual void Update(UINT32 dt);
	virtual void OnDeleted();

	bool	IsAllOverlapDone();
	bool 	TryReadPacket();

private:
	IO_DATA* m_pIoOverlap[IO_FD_OP_COUNT];

	Ringbuffer<char, READ_RING_BUFFER_SIZE, UINT32, false> m_readRingBuffer;
	char	m_readBuffTmp[READ_RING_BUFFER_SIZE / 2];	
};

