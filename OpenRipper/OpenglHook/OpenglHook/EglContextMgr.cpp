#include "EglContextMgr.h"


EglContextMgr::EglContextMgr(void)
{
}


EglContextMgr::~EglContextMgr(void)
{
}

EGLContext EglContextMgr::GetShareContext(EGLContext context)
{
	EGLContext ret = NULL;
	m_lock.Lock();
	auto it = m_mapShareContext.find(context);
	if (it != m_mapShareContext.end())
	{
		ret = it->second;
	}
	m_lock.Unlock();
	return ret;
}

void EglContextMgr::CreateContext(EGLContext context,	EGLContext share_context)
{
	m_lock.Lock();
	if (share_context != NULL)
	{
		m_mapShareContext[context] = share_context;
	}
	m_lock.Unlock();
}
