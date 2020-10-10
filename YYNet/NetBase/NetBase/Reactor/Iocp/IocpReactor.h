#pragma once
#include "../ReactorBase.h"
#include "windows.h"
enum IO_OPERATION 
{ 
	IO_READ = 0, 
	IO_WRITE, 
	IO_ACCEPT,
	IO_SHUTDOWN,
	IO_COUNT
};

#define IO_FD_OP_COUNT (IO_WRITE - IO_READ + 1)
struct IO_DATA {
	OVERLAPPED                  _Overlapped;
	IO_OPERATION                _opCode;
	WSABUF						_wsaBuf;
	IO_DATA(IO_OPERATION opCode)
	{
		_opCode = opCode;
		_wsaBuf.len = 0;
		_wsaBuf.buf = NULL;
		m_ref = 0;
		Reset();
	}
	std::atomic<long> m_ref;
	void IncrementRef()
	{		
		m_ref.fetch_add(1);
	}

	void DecrementRef()
	{
		m_ref.fetch_add(-1);
	}

	long RefCount()
	{
		return m_ref.load();
	}

	void Reset()
	{
		memset(&_Overlapped, 0x00, sizeof(_Overlapped));
	}
};

struct IO_DATA_ACCEPT : public IO_DATA
{
	IO_DATA_ACCEPT(IO_OPERATION opCode):
		IO_DATA(opCode)
	{
		_wsaBuf.buf = _buf;
		_wsaBuf.len = sizeof(_buf);
	}
	SOCKET _fd;
	char   _buf[(sizeof(SOCKADDR_IN) + 16) * 2];
};
class IocpWorkerThread;
class IocpReactor :
	public ReactorBase
{
public:
	IocpReactor();
	virtual ~IocpReactor();
	virtual void AddIoPeer(IoPeerBase *pIoPeer);
	virtual void Start(UINT32 nThread);
	virtual void Stop();
	void SendStopIoOverlap();

private:
	HANDLE m_completionPort;
	IocpWorkerThread** m_iocpWorkder;
	UINT32			   m_threadCount;
};

