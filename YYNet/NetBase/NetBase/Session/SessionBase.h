#pragma once
class IoPeerBase;
template<size_t BufferSize = 8 * 1024>
//write front buffer and read backend buffer
class DoubleBuffer
{
public:
	DoubleBuffer()
	{
		m_bufferFrontIdx = 0;
	}
	~DoubleBuffer() {}	
	void Append(char *pBuffer, UINT32 nSize)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_buffer[m_bufferFrontIdx % 2].Append(pBuffer, nSize);
	}
	void GetAndSwapFrontBuffer(char *&pBuffer, UINT32 &nSize)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_buffer[(m_bufferFrontIdx) % 2].GetReadBuffer(pBuffer, nSize);

		_Swap();
	}

private:
	void _Swap()
	{
		m_bufferFrontIdx++;
		m_buffer[m_bufferFrontIdx % 2].Reset();
	}
	MemoryStream<BufferSize> m_buffer[2];
	UINT32					 m_bufferFrontIdx;

	std::mutex				 m_mutex;
};
class SessionManager;
class SessionBase
{
public:
	void PushRecvPacket(NetPacket *pPck);
	void PushSendPacket(NetPacket &pPck);
	virtual void OnPeerShutDown();
	virtual void Update(UINT32 dt);
	void UpdateRecvPacket();
	void GetSendBuffer(char* &pBuff, UINT32 &nSize);
	virtual int HandlePacket(NetPacket &pck) {return 0;}
	SessionBase(IoPeerBase *pIoPeer, SessionManager* pManager);
	virtual ~SessionBase();
	bool	NeedDelete(){return m_tobeDeleted;}
	void	OnQuit();
protected:
	IoPeerBase * m_pIoPeer;
	SessionManager* m_sessionManager;
private:
	SafeQueue<NetPacket*> m_recvPackets;
	DoubleBuffer<64 * 1024> m_doubleBuffer;
	
	bool			m_tobeDeleted;
};

