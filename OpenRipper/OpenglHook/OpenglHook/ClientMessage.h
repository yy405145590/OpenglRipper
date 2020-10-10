#pragma once
class MySession :public SessionBase
{
public:
	using HandlerType = int (MySession::*) (NetPacket &pck);
	MySession(IoPeerBase *pIoPeer, SessionManager* pManager);
	virtual int HandlePacket(NetPacket &pck);
	virtual void OnPeerShutDown();

	void SendLogMessage(const std::string &msg);
	void SendRegister();

	int HandleDump(NetPacket &pck);
	int HandleSetDrawRange(NetPacket &pck);
private:
	HandlerType	m_handlers[OpCode::OpCodeCount];
};

class ClientMessage:public SafeSingleton<ClientMessage>
{
public:
	ClientMessage();
	~ClientMessage();

	void Update(UINT32 dt);
	void ConnectToServer();

	void OnDisconnect();


	void PushLog(const std::string &&log)
	{
		m_logQueue.push(std::forward<const std::string>(log));
	}

	void PushLog(const std::string &log)
	{
		m_logQueue.push(std::forward<const std::string>(log));
	}

private:
	bool m_bConnected;
	SessionManager	*m_pSessionManager;
	IocpReactor		*m_pReactor		;
	MySession		*m_pSession		;
	SafeQueue<std::string>	m_logQueue;
};

#define sClientMsg	ClientMessage::getInstance()

