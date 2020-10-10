#pragma once
#include "stdafx.h"
#include "TlsInstance.h"
#include <unordered_map>

class EglContextMgr
{
public:
	void CreateContext( EGLContext context,	EGLContext share_context);
	EGLContext GetShareContext(EGLContext context);
	static EglContextMgr* GetInstance()
	{
		static EglContextMgr sInstance;
		return &sInstance;
	}
private:
	EglContextMgr(void);
	~EglContextMgr(void);

	LockSection	m_lock;
	std::unordered_map<EGLContext, EGLContext> m_mapShareContext;
};

#define  sEglContextMgr EglContextMgr::GetInstance()

