#include "stdafx.h"
#include "IoPeerIocpAccept.h"

IoPeerIocpAccept::IoPeerIocpAccept()
{
	m_pIoOverlap = new IO_DATA_ACCEPT(IO_ACCEPT);	
}

bool IoPeerIocpAccept::Init(const std::string& ip, UINT16 port)
{
	SOCKET sListen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	m_fd = sListen;
	//监听套接字绑定监听  
	SOCKADDR_IN serAdd;
	serAdd.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	serAdd.sin_family = AF_INET;
	serAdd.sin_port = htons(port);
	InitServerSocket(m_fd);
	if (bind(sListen, (SOCKADDR*)&serAdd, sizeof(SOCKADDR)) < 0)
	{
		return false;
	}
	if (listen(sListen, SOMAXCONN) < 0)
	{
		return false;
	}
	DWORD dwbytes = 0;
	//Accept function GUID  
	GUID guidAcceptEx = WSAID_ACCEPTEX;

	if (0 != WSAIoctl(sListen, SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guidAcceptEx, sizeof(guidAcceptEx),
		&m_pfnAcceptEx, sizeof(m_pfnAcceptEx),
		&dwbytes, NULL, NULL))
	{
		return false;
	}


	// 获取GetAcceptExSockAddrs函数指针，也是同理  
	GUID guidGetAcceptExSockaddrs = WSAID_GETACCEPTEXSOCKADDRS;
	if (0 != WSAIoctl(sListen, SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guidGetAcceptExSockaddrs,
		sizeof(guidGetAcceptExSockaddrs),
		&m_pfnGetAcceptExSockaddrs,
		sizeof(m_pfnGetAcceptExSockaddrs),
		&dwbytes, NULL, NULL))
	{
		return false;
	}
	return true;
}

IoPeerIocpAccept::~IoPeerIocpAccept()
{
	delete m_pIoOverlap;
	m_pIoOverlap = NULL;
}

void IoPeerIocpAccept::InitServerSocket(SOCKET fd)
{
	u_long arg = 1;
	ioctlsocket(fd, FIONBIO, &arg);
	arg = 1;
	setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (const char*)&arg, sizeof(arg));
	arg = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&arg, sizeof(arg));
}

void IoPeerIocpAccept::DoRead()
{
	DWORD dwBytes;
	m_pIoOverlap->Reset();
	m_pIoOverlap->_fd = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	InitServerSocket(m_pIoOverlap->_fd);
	m_pIoOverlap->IncrementRef();
	BOOL rc = m_pfnAcceptEx(m_fd, m_pIoOverlap->_fd,
		m_pIoOverlap->_wsaBuf.buf, 0,
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
		&dwBytes, (OVERLAPPED*)m_pIoOverlap);
	if (FALSE == rc)
	{
		if (WSAGetLastError() != ERROR_IO_PENDING)
		{
			printf("%d", WSAGetLastError());
			m_pIoOverlap->DecrementRef();
			OnShutDown();
		}
	}
}


void IoPeerIocpAccept::OnReadComplete(UINT32  nSize, IO_DATA* pOverlap)
{
	IO_DATA_ACCEPT* pIoAccept = (IO_DATA_ACCEPT *)pOverlap;
	setsockopt(pIoAccept->_fd, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&(pIoAccept->_fd), sizeof(pIoAccept->_fd));

	SOCKADDR_IN *addrClient = NULL, *addrLocal = NULL;
	int nClientLen = sizeof(SOCKADDR_IN), nLocalLen = sizeof(SOCKADDR_IN);

	m_pfnGetAcceptExSockaddrs(pIoAccept->_wsaBuf.buf, 0,
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
		(LPSOCKADDR*)&addrLocal, &nLocalLen,
		(LPSOCKADDR*)&addrClient, &nClientLen);

	OnNewClientConnect(pIoAccept->_fd);
	//sprintf(pClientComKey->sIP, "%d", addrClient->sin_port); //cliAdd.sin_port );  
	//printf(pClientComKey->sIP);
	DoRead();
}

bool IoPeerIocpAccept::IsAllOverlapDone()
{
	return m_pIoOverlap->RefCount() == 0;
}

void IoPeerIocpAccept::Update(UINT32 dt)
{
	if (IsDeleted())
	{
		return;
	}
	if (IsShutDown() && IsAllOverlapDone())
	{
		OnDeleted();
	}
}

void IoPeerIocpAccept::OnDeleted()
{
	IoPeerBase::OnDeleted();
	m_pSession->OnPeerShutDown();
}
