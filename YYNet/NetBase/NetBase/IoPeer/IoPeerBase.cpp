#include "stdafx.h"
#include "IoPeerBase.h"

IoPeerBase::IoPeerBase():
	m_bShutDown(false),
	m_bDeleted(false),	
	m_pSession(NULL),
	m_fd(INVALID_SOCKET)
{
}

IoPeerBase::~IoPeerBase()
{
	CloseFd();
}

void IoPeerBase::DoRead()
{
}

void IoPeerBase::DoWrite()
{
}

void IoPeerBase::OnReadComplete(UINT32  nSize, IO_DATA* pOverlap)
{
}

void IoPeerBase::OnWriteComplete(UINT32  nSize, IO_DATA* pOverlap)
{
}


void IoPeerBase::Update(UINT32 dt)
{

}
