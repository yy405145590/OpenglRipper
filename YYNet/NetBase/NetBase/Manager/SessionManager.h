#pragma once
#include <set>
class SessionManager
{
public:
	SessionManager();
	~SessionManager();

	void AddSession(SessionBase *pSession);
	void RemoveSession(SessionBase *pSession);

	void Update(UINT32 dt);

	void Stop();
	UINT32 SessionCount(){return (UINT32)m_sessionList.size();}
	SessionBase *GetSession(UINT32 sessId)
	{
		if (m_sessionList.size() > 0)
		{
			return *m_sessionList.begin();
		}
		return NULL;
	}
private:
	std::set<SessionBase*> m_sessionList;
};
