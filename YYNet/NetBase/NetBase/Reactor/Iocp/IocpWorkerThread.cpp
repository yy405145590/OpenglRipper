#include "stdafx.h"
#include "IocpWorkerThread.h"
#include "IocpReactor.h"
#include "../../IoPeer/IoPeerBase.h"

IocpWorkerThread::IocpWorkerThread(HANDLE port):
	m_completionPort(port)
{

}

IocpWorkerThread::~IocpWorkerThread()
{

}

void IocpWorkerThread::Start()
{
	m_thread = std::thread(&IocpWorkerThread::Run, this);
}

void IocpWorkerThread::Stop()
{
	m_thread.join();
}

void IocpWorkerThread::Run()
{
	IO_DATA *pIOContext = NULL;
	DWORD nBytes = 0;
	DWORD dwFlags = 0;
	int nRet = 0;

	DWORD dwIoSize = 0;
	IoPeerBase* pCompletionKey = NULL;

	while (1) 
	{
		BOOL nRet = GetQueuedCompletionStatus(m_completionPort, 
			&dwIoSize, 
			(PULONG_PTR)&pCompletionKey, (LPOVERLAPPED *)&pIOContext, INFINITE);

		if (nRet)
		{
			if (pIOContext->_opCode == IO_SHUTDOWN)
			{
				delete pIOContext;
				break;
			}
			//failed
			if (dwIoSize == 0 && (pIOContext->_opCode == IO_READ || pIOContext->_opCode == IO_WRITE))
			{
				//disconnect
				printf("pCompletionKey->disconnect:%d\n", pIOContext->_opCode);				
				pCompletionKey->OnShutDown();				
				pIOContext->DecrementRef();
				continue;
			}
			else if (pCompletionKey->IsShutDown())
			{
				pIOContext->DecrementRef();
			}
			else
			{
				//success
				switch (pIOContext->_opCode)
				{
				case IO_ACCEPT:
				case IO_READ:
					assert(pCompletionKey != NULL);
					pCompletionKey->OnReadComplete(dwIoSize, pIOContext);
					//printf("pCompletionKey->OnReadComplete:%d\n", pIOContext->_opCode);
					break;
				case IO_WRITE:
					assert(pCompletionKey != NULL);
					pCompletionKey->OnWriteComplete(dwIoSize, pIOContext);
					//printf("pCompletionKey->OnWriteComplete:%d\n", pIOContext->_opCode);
					break;
				default:
					assert(false && "GetQueuedCompletionStatus case error");
					break;
				}
				pIOContext->DecrementRef();
			}
		}
		else
		{
			UINT32 nErr = GetLastError();
			printf("GetQueuedCompletionStatus error:%d, %d, %p, %p\n", nErr, dwIoSize, pCompletionKey, pIOContext);
			//内部错误，没有取出任何的完成事件
			if (pIOContext == NULL)
			{
				continue;
			}
			else
			{
				//disconnect
				printf("pCompletionKey->disconnect err:%d\n", pIOContext->_opCode);

				pCompletionKey->OnShutDown();
				pIOContext->DecrementRef();
			}
		}
	}
}

