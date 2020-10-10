#include "stdafx.h"
#include "SessionManager.h"


SessionManager::SessionManager()
{
	
}

SessionManager::~SessionManager()
{
	assert(m_sessionList.size() == 0);
}

void SessionManager::AddSession(SessionBase *pSession)
{
	m_sessionList.insert(pSession);
	printf("AddSession\n");
}

void SessionManager::RemoveSession(SessionBase *pSession)
{
	m_sessionList.erase(pSession);
	delete pSession;
	printf("RemoveSession\n");
}

void SessionManager::Update(UINT32 dt)
{
	for (auto it = m_sessionList.begin(); 
		it != m_sessionList.end();)
	{
		(*it)->Update(dt);
		if ((*it)->NeedDelete())
		{
			RemoveSession(*it++);
		}
		else
		{
			it++;
		}
	}
}

void SessionManager::Stop()
{
	for (auto it = m_sessionList.begin();
		it != m_sessionList.end();)
	{
		(*it)->OnQuit();
		RemoveSession(*it++);
	}
}