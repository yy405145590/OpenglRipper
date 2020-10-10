#include "stdafx.h"
#include "IocpReactor.h"
#include "IocpWorkerThread.h"

IocpReactor::IocpReactor()
{
}


IocpReactor::~IocpReactor()
{
}

void IocpReactor::AddIoPeer(IoPeerBase *pIoPeer)
{
	HANDLE handle = CreateIoCompletionPort((HANDLE)pIoPeer->GetFd(), m_completionPort, (ULONG_PTR)pIoPeer, 0);
	if (handle == NULL)
	{
		printf("AddIoPeer error:%d\n", GetLastError());
		return;
	}
	pIoPeer->DoRead();
}

void IocpReactor::Start(UINT32 nThread)
{
	m_completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (m_completionPort == NULL)
	{
		printf("IocpReactor start failed:%d\n", GetLastError());
		return;
	}
	m_threadCount = nThread;
	m_iocpWorkder = new IocpWorkerThread*[m_threadCount];
	for (int i = 0; i < (int)m_threadCount; ++i)
	{
		m_iocpWorkder[i] = new IocpWorkerThread(m_completionPort);
		m_iocpWorkder[i]->Start();
	}
}

void IocpReactor::SendStopIoOverlap()
{
	IO_DATA * ovShutDown = new IO_DATA(IO_SHUTDOWN);
	PostQueuedCompletionStatus(m_completionPort, 0, (ULONG_PTR)this, (LPOVERLAPPED)ovShutDown);
}

void IocpReactor::Stop()
{
	//todo send iocp shutdown
	for (int i = 0; i < (int)m_threadCount; ++i)
	{
		SendStopIoOverlap();
	}	
	for (int i = 0; i < (int)m_threadCount; ++i)
	{
		m_iocpWorkder[i]->Stop();
		delete m_iocpWorkder[i];
	}

	delete[] m_iocpWorkder;
	CloseHandle(m_completionPort);
}
