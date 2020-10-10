#pragma once
#include <thread>


class IocpWorkerThread
{
public:
	IocpWorkerThread(HANDLE port);
	~IocpWorkerThread();	
	void Start();
	void Stop();

private:
	void Run();

private:
	std::thread m_thread;
	HANDLE		m_completionPort;
};

