#pragma once
struct IO_DATA;
class IoPeerIocpAccept:public IoPeerBase
{
public:
	IoPeerIocpAccept();
	virtual ~IoPeerIocpAccept();

	virtual void DoRead();
	virtual void OnReadComplete(UINT32  nSize, IO_DATA* pOverlap);
	virtual void OnDeleted();
	virtual void Update(UINT32 dt);
	virtual void OnNewClientConnect(SOCKET fd) = 0;

	bool Init(const std::string& ip, UINT16 port);
	void InitServerSocket(SOCKET fd);
	bool IsAllOverlapDone();
private:
	IO_DATA_ACCEPT* m_pIoOverlap;

	LPFN_ACCEPTEX				m_pfnAcceptEx = NULL;                   //AcceptEx函数指针  
	LPFN_GETACCEPTEXSOCKADDRS	m_pfnGetAcceptExSockaddrs;  //加载GetAcceptExSockaddrs函数指针  	
};

