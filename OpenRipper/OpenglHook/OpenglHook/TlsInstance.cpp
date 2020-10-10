#include "TlsInstance.h"
#include "EglContextMgr.h"

LockSection::LockSection()
{
	InitializeCriticalSection(&m_section);
}

LockSection::~LockSection()
{
	DeleteCriticalSection(&m_section);
}

void LockSection::Lock()
{
	EnterCriticalSection(&m_section);
}

void LockSection::Unlock()
{
	LeaveCriticalSection(&m_section);
}

InstanceKey GetID()
{
	EGLContext sur = g_old_pfneglGetCurrentContext();
	EGLContext shareContext = sEglContextMgr->GetShareContext(sur);
	if (shareContext != NULL)
	{
		return shareContext;
	}
	return sur;
}


