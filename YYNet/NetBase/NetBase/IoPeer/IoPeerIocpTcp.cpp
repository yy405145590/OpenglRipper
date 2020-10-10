#include "stdafx.h"
#include "IoPeerIocpTcp.h"
#include <WinSock2.h>
#include "Util/CRC.h"

IoPeerIocpTcp::IoPeerIocpTcp()
{
	for (int i = 0; i < ARRAYSIZE(m_pIoOverlap); ++i)
	{
		m_pIoOverlap[i] = new IO_DATA((IO_OPERATION)i);
	}
}

IoPeerIocpTcp::~IoPeerIocpTcp()
{
	for (int i = 0; i < ARRAYSIZE(m_pIoOverlap); ++i)
	{
		delete m_pIoOverlap[i];
	}
}

void IoPeerIocpTcp::DoWrite()
{
	if (m_pIoOverlap[IO_WRITE]->RefCount() == 0 && !IsShutDown())
	{
		OnWriteComplete(0, m_pIoOverlap[IO_WRITE]);
	}
}

void IoPeerIocpTcp::DoRead()
{
	DWORD length = 0;
	DWORD flags = 0;
	m_pIoOverlap[IO_READ]->Reset();
	m_pIoOverlap[IO_READ]->IncrementRef();
	m_pIoOverlap[IO_READ]->_wsaBuf.len = sizeof(m_readBuffTmp);
	m_pIoOverlap[IO_READ]->_wsaBuf.buf = m_readBuffTmp;
	if (WSARecv(m_fd, &m_pIoOverlap[IO_READ]->_wsaBuf, 
		1, &length, &flags, 
		(OVERLAPPED*)m_pIoOverlap[IO_READ], 0) == SOCKET_ERROR)
	{
		int err = WSAGetLastError();
		if (err != WSA_IO_PENDING)
		{
			//error
			OnShutDown();
			m_pIoOverlap[IO_READ]->DecrementRef();			
		}
	}
}


void IoPeerIocpTcp::OnReadComplete(UINT32  nSize, IO_DATA* pOverlap)
{
	assert(pOverlap->_wsaBuf.buf != NULL);
	UINT32 nWritten = 0;
	do
	{
		nWritten = (UINT32)m_readRingBuffer.writeBuff(pOverlap->_wsaBuf.buf + nWritten, nSize);
		nSize -= nWritten;
		bool bRead = TryReadPacket();
		if (!(nSize == 0 || bRead))
		{
			assert(nSize == 0 || bRead);
		}		
	} while (nSize > 0);

	DoRead();
}

void IoPeerIocpTcp::OnWriteComplete(UINT32 nSize, IO_DATA* pOverlap)
{
	if (pOverlap->_wsaBuf.buf == NULL 
		|| pOverlap->_wsaBuf.len == nSize)
	{
		//如果当前的发送完成了，就获取新的buffer
		m_pSession->GetSendBuffer(pOverlap->_wsaBuf.buf, (UINT32 &)pOverlap->_wsaBuf.len);
		if (pOverlap->_wsaBuf.buf == NULL)
		{
			return;
		}
	}
	else
	{
		//如果当前的没有发送完成，就修正buffer和len 继续发送
		pOverlap->_wsaBuf.buf += nSize;
		pOverlap->_wsaBuf.len -= nSize;
		printf("test case 1\n");
	}
	DWORD length = 0;
	DWORD flags = 0;
	pOverlap->IncrementRef();
	DWORD nSent = 0;
	int ret = WSASend(m_fd, &pOverlap->_wsaBuf, 1, &nSent, flags, (OVERLAPPED*)pOverlap, 0);
	if (ret == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			OnShutDown();
			pOverlap->DecrementRef();			
		}
		else
		{
			printf("WSA_IO_PENDING \n");
		}
	}
}


bool IoPeerIocpTcp::TryReadPacket()
{
	bool bRet = false;
	while (true)
	{
		if (m_readRingBuffer.readAvailable() < PACKET_HEADER_SIZE)
		{
			break;
		}

		NET_PACKET_HEADER header;
		m_readRingBuffer.readBuff((char*)&header, PACKET_HEADER_SIZE);
		if (header._size > m_readRingBuffer.readAvailable())
		{
			m_readRingBuffer.skip(-(int)PACKET_HEADER_SIZE);
			break;
		}
		else
		{
			NetPacket *pPack = new NetPacket(header._opCode);
			char *pBuf = NULL;			
			pPack->GetWriteBuffer(pBuf, header._size);
			if (pBuf)
			{
				m_readRingBuffer.readBuff(pBuf, header._size);
				//push new packet to session
				//...
				//check crc
				std::uint32_t crc = CRC::Calculate(pBuf, header._size, CRC::CRC_32());
				if (header._crc != crc)
				{
					printf("error crc\n");
				}
				
			}
			//pBuf == NULL maybe only opcode no buffer
			m_pSession->PushRecvPacket(pPack);
			bRet = true;
		}
	}
	
	return bRet;
}

bool IoPeerIocpTcp::IsAllOverlapDone()
{
	for (int i = 0; i < ARRAYSIZE(m_pIoOverlap); ++i)
	{
		if (m_pIoOverlap[i]->RefCount() > 0)
		{
			return false;
		}
	}
	return true;
}

void IoPeerIocpTcp::Update(UINT32 dt)
{
	if (IsDeleted())
	{
		return;
	}
	if (IsShutDown() && IsAllOverlapDone())
	{
		OnDeleted();
	}
	else
	{
		DoWrite();
	}
}

void IoPeerIocpTcp::OnDeleted()
{
	IoPeerBase::OnDeleted();
	m_pSession->OnPeerShutDown();
	printf("IoPeerIocpTcp OnDeleted\n");
}


