#include "stdafx.h"
#include "ClientMessage.h"

extern void MakeAndCreateDumpFolder(const std::string& folder);
extern std::atomic<int> g_dumpCommandIdx;

MySession::MySession(IoPeerBase *pIoPeer, SessionManager* pManager):
	SessionBase(pIoPeer, pManager)	
{
	m_handlers[OpCode::SC_DUMP] = &MySession::HandleDump;
	m_handlers[OpCode::SC_SET_DRAW_RANGE] = &MySession::HandleSetDrawRange;
}

int MySession::HandlePacket(NetPacket &pck)
{
	(this->*m_handlers[pck.GetOpCode()])(pck);
	return 0;
}
void MySession::OnPeerShutDown()
{
	SessionBase::OnPeerShutDown();
	sClientMsg->OnDisconnect();
}

void MySession::SendLogMessage(const std::string &logMsg)
{
	NetPacket pck(OpCode::CS_LOG);
	pck << logMsg;
	PushSendPacket(pck);
}

void MySession::SendRegister()
{
	NetPacket pck(OpCode::CS_REGISTER);
	PushSendPacket(pck);
}

int MySession::HandleDump(NetPacket &pck)
{
	std::string dumpFolder;
	pck >> dumpFolder;
	if (dumpFolder.empty())
	{
		sConsoleLog->Write("dumpFolder is empty");
		return 0;
	}
	sConsoleLog->Write("start dump to %s", dumpFolder.c_str());		
	MakeAndCreateDumpFolder(dumpFolder);
	g_dumpCommandIdx++;
	return 0;
}
extern GLsizei g_drawMaxCount;
extern GLsizei g_drawMinCount;
int MySession::HandleSetDrawRange(NetPacket &pck)
{
	pck >> g_drawMinCount >> g_drawMaxCount;
	sConsoleLog->Write("HandleSetDrawRange (%d)~(%d)", g_drawMinCount, g_drawMaxCount);
	return 0;
}

ClientMessage::ClientMessage():
	m_bConnected(false),
	m_pSession(NULL)
{
	WSADATA dwData;
	WSAStartup(0x0202, &dwData);
	m_pSessionManager = new SessionManager();
	m_pReactor = new IocpReactor();
	m_pReactor->Start(1);
}


ClientMessage::~ClientMessage()
{
	m_pReactor->Stop();
	m_pSessionManager->Stop();
	delete m_pReactor;
	delete m_pSessionManager;
}

void ClientMessage::Update(UINT32 dt)
{
	if (!m_bConnected)
	{
		ConnectToServer();
	}
	else
	{
		std::string s;
		while (m_logQueue.try_pop(s))
		{
			m_pSession->SendLogMessage(s);
		}
	}
	m_pSessionManager->Update(dt);
}

void ClientMessage::OnDisconnect()
{
	sConsoleLog->WriteConsole("OnDisconnect");
	m_bConnected	= false;
	m_pSession		= NULL;
}

void ClientMessage::ConnectToServer()
{
	sConsoleLog->WriteConsole("ConnectToServer");
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	struct hostent * ci = gethostbyname("127.0.0.1");
	sockaddr_in addr_far;
	memset(&addr_far, 0, sizeof(struct sockaddr_in));
	addr_far.sin_family = AF_INET;
	memcpy(&addr_far.sin_addr.s_addr, ci->h_addr_list[0], ci->h_length);
	addr_far.sin_port = htons((u_short)9986);
	int error = connect(clientSocket, (struct sockaddr *)&addr_far, sizeof(struct sockaddr_in));
	if (error != 0)
	{
		sConsoleLog->WriteConsole("connect error\n");
		return;
	}
	else
	{
		sConsoleLog->WriteConsole("connect success\n");
	}
	m_bConnected = true;
	int nSendBuf;
	int len = sizeof(nSendBuf);
	int iResult = getsockopt(clientSocket, SOL_SOCKET, SO_SNDBUF, (char *)&nSendBuf, &len);
	u_long arg = 1;
	ioctlsocket(clientSocket, FIONBIO, &arg);
	arg = 1;
	setsockopt(clientSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&arg, sizeof(arg));
	IoPeerIocpTcp *pIoPeer = new IoPeerIocpTcp();
	pIoPeer->SetFd(clientSocket);
	MySession *pSession = new MySession(pIoPeer, m_pSessionManager);
	pIoPeer->SetSession(pSession);
	m_pReactor->AddIoPeer(pIoPeer);
	pSession->SendRegister();
	m_pSession = pSession;
}
