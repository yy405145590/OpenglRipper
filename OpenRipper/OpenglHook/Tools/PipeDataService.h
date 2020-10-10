#pragma once
#include "RingBuffer.hpp"
class PipeDataService;
class Session;

enum IOType
{
	IO_READ = 0,
	IO_WRITE,
};

typedef struct IoOverlap
{
	OVERLAPPED	_oOverlap;
	IOType		_ioType;
	Session*	_pSession;
	IoOverlap(IOType type, Session* pSession)
	{
		_ioType = type;
		_pSession = pSession;
	}
}IoOverlap, *PIoOverlap;

class Session
{
public:
	virtual void OnConnectSuccess() {};
	virtual void OnDisConnect() {};
	virtual void OnReadData() {};
	virtual void OnWriteData() {};

	Session();
	~Session();
	void PushSendPacket() {};
	void PushRecvPacket() {};
	//void 
private:
	//ring buffer
	Ringbuffer<char, 1024> m_ringReadBuffer;
	Ringbuffer<char, 1024> m_ringWriteBuffer;
	Ringbuffer<DataMessagePack*, 1024>	m_recvPacket;

	IoOverlap *m_pReadOverlap;
	IoOverlap *m_pWriteOverlap;
	HANDLE	   m_pPipe;
};


class PipeDataService
{
public:
	PipeDataService();
	~PipeDataService();
	void		StartServer();
	void		Connect();

	void		ServerLoop();
	void		ClientLoop();
	BOOL		CreateAndConnectInstance(LPOVERLAPPED lpoOverlap, HANDLE &hPipe);
	BOOL		ConnectToNewClient(HANDLE hPipe, LPOVERLAPPED lpo);
	void		OnClientConnect(HANDLE hPipe);

private:
	//HANDLE m_hPipe;
};

