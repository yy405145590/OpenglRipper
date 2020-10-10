#include "stdafx.h"
#include "SessionBase.h"
#include "Util/CRC.h"

SessionBase::SessionBase(IoPeerBase *pIoPeer, SessionManager* pManager) :
	m_pIoPeer(pIoPeer),
	m_sessionManager(pManager),
	m_tobeDeleted(false)
{
	m_sessionManager->AddSession(this);
}

SessionBase::~SessionBase()
{
	assert(m_pIoPeer == NULL);
	NetPacket* p;
	while (m_recvPackets.try_pop(p))
	{
		delete p;
	}
}


void SessionBase::PushRecvPacket(NetPacket * pPck)
{
	m_recvPackets.push(pPck);
}

void SessionBase::PushSendPacket(NetPacket &pPck)
{
	char	*pBuf = NULL;
	UINT32	nSize = 0;
	NET_PACKET_HEADER header;
	header._opCode = pPck.GetOpCode();
	pPck.GetReadBuffer(pBuf, nSize);
	if (nSize > std::numeric_limits<decltype(header._size)>::max())
	{
		assert(false && "PushSendPacket overflow");
	}
	else
	{
		header._size = nSize;
		std::uint32_t crc = CRC::Calculate(pBuf, nSize, CRC::CRC_32());
		header._crc = crc;
		m_doubleBuffer.Append((char *)&header, PACKET_HEADER_SIZE);
		m_doubleBuffer.Append(pBuf, nSize);
	}
}

void SessionBase::OnPeerShutDown()
{
	assert(m_pIoPeer != NULL);
	delete m_pIoPeer;
	m_pIoPeer = NULL;
	m_tobeDeleted = true;
	printf("OnPeerShutDown\n");
}

void SessionBase::OnQuit()
{
	delete m_pIoPeer;
	m_pIoPeer = NULL;
	m_tobeDeleted = true;
	printf("OnQuit\n");
}

void SessionBase::Update(UINT32 dt)
{
	UpdateRecvPacket();
	if (m_pIoPeer)
	{
		m_pIoPeer->Update(dt);
	}

}

void SessionBase::UpdateRecvPacket()
{
	NetPacket* pPck = NULL;
	while (m_recvPackets.try_pop(pPck))
	{
		HandlePacket(*pPck);
		delete pPck;
	}
}

void SessionBase::GetSendBuffer(char *& pBuff, UINT32 & nSize)
{
	m_doubleBuffer.GetAndSwapFrontBuffer(pBuff, nSize);
}

