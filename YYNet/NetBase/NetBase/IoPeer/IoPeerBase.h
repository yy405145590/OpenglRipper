#pragma once
struct IO_DATA;
class IoPeerBase
{
public:
	IoPeerBase();
	virtual ~IoPeerBase();

	virtual void DoRead();
	virtual void DoWrite();
	virtual void OnReadComplete(UINT32  nSize, IO_DATA* pOverlap);
	virtual void OnWriteComplete(UINT32 nSize, IO_DATA* pOverlap);
	virtual	void OnShutDown(){m_bShutDown = true;}
	virtual void OnDeleted() 
	{
		m_bDeleted = true;
		CloseFd();
	}
	virtual HANDLE GetFd(){return (HANDLE)m_fd;}

	virtual void Update(UINT32 dt);

	bool	IsShutDown(){return m_bShutDown;}
	bool	IsDeleted(){return m_bDeleted;}
	void	SetSession(SessionBase *pSession)
	{
		m_pSession = pSession;
	}
	void	CloseFd()
	{		
		if (m_fd != INVALID_SOCKET)
		{
			printf("close fd:%p\n", (void *)m_fd);
			shutdown(m_fd, SD_BOTH);
			CancelIoEx((HANDLE)m_fd, NULL);
			closesocket(m_fd);
			m_fd = INVALID_SOCKET;
		}
	}

	void SetFd(SOCKET fd)
	{
		m_fd = fd;
	}

private:
	bool	m_bShutDown;
	bool	m_bDeleted;

protected:
	SessionBase* m_pSession;
	SOCKET  m_fd;
};

