#include "stdafx.h"
#include "BufferObjectMgr.h"


BufferObject::BufferObject(const char *pData, int size, uint32 id)
{
	m_pData		= NULL;
	m_size		= 0;
	if (size > 0)
	{
		m_pData = new char[size];
		memset(m_pData, 0x00, size);
		m_size  = (uint32)size;
	}
	if (pData && m_pData)
	{
		memcpy((void *)m_pData, pData, size);
	}
	m_id = id;
}

bool BufferObject::Write(const char *pData, int offset, int size)
{
	if (pData == NULL)
	{
		sConsoleLog->DebugWrite("BufferObject::Write:pData == null");
		return false;
	}
	if ((uint32)(size + offset) > m_size)
	{
		sConsoleLog->DebugWrite("BufferObject::Write:offset + size[%d] > m_size[%d]", size + offset, m_size);
		return false;
	}
	else
	{
		memcpy((void *)(m_pData + offset), pData, size);
	}
	return true;
}

BufferObject::~BufferObject()
{
	if (m_pData != NULL)
	{
		delete m_pData;
		m_pData = NULL;
	}
	m_size = 0;
	m_id   = 0;
}

BufferObjectMgr::BufferObjectMgr(void)
{
	m_mapBufferBind[GL_ARRAY_BUFFER]			= 0;
	m_mapBufferBind[GL_ELEMENT_ARRAY_BUFFER]	= 0;	
}


BufferObjectMgr::~BufferObjectMgr(void)
{
}

void BufferObjectMgr::BindBuffer(GLenum target, GLuint buffer)
{
// 	if (target != GL_ARRAY_BUFFER && target != GL_ELEMENT_ARRAY_BUFFER)
// 	{
// 		sConsoleLog->Write("BindBuffer:target[%d] is not recognize", target);
// 	}
	m_mapBufferBind[target] = buffer;
}

void BufferObjectMgr::GenBuffers(GLsizei n, GLuint *buffers)
{
	for (int i = 0; i < n; ++i)
	{
		if(m_mapBuffers.find(buffers[i]) != m_mapBuffers.end())
		{
			sConsoleLog->DebugWrite("GenBuffers:buffer name[%d] is exist in buffer names", buffers[i]);
			delete m_mapBuffers[buffers[i]];
		}
		m_mapBuffers[buffers[i]] = NULL;
	}
}

void BufferObjectMgr::DeleteBuffers(GLsizei n, const GLuint * buffers)
{
	for (int i = 0; i < n; ++i)
	{
		if(m_mapBuffers.find(buffers[i]) == m_mapBuffers.end())
		{
			sConsoleLog->DebugWrite("DeleteBuffers:buffer name isn't exist in buffer names");
		}
		else
		{
			if (m_mapBuffers[buffers[i]] !=	NULL)
			{
				delete m_mapBuffers[buffers[i]];
			}			
			m_mapBuffers.erase(buffers[i]);
		}
	}
}

void* BufferObjectMgr::MapPBOOffset(const void* pixels)
{
	GLuint vbo = GetBindBuffer(GL_PIXEL_UNPACK_BUFFER);
	if (vbo != 0)
	{
		BufferObject* pObj = GetBufferObject(vbo);
		if (pObj != NULL)
		{
			void *pBuffer = g_old_pfnglMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, pObj->m_size, GL_MAP_READ_BIT);
			if (pBuffer)
			{
				return (void *)((uint32)pBuffer + (uint32)pixels);
			}
		}
		
	}
	return (void *)pixels;
}

void BufferObjectMgr::UnMapPBO()
{
	if (GetBindBuffer(GL_PIXEL_UNPACK_BUFFER) != 0)
	{
		g_old_pfnglUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
	}
}

GLuint BufferObjectMgr::GetBindBuffer(GLenum target)
{
// 	if (target != GL_ARRAY_BUFFER && target != GL_ELEMENT_ARRAY_BUFFER)
// 	{
// 		sConsoleLog->Write("GetBindBuffer:target[%d] is not recognize", target);
// 	}
	if (m_mapBufferBind.find(target) == m_mapBufferBind.end())
	{
		return 0;
	}
	return m_mapBufferBind[target];
}

BufferObject* BufferObjectMgr::GetBufferObject(GLuint index)
{
	if (m_mapBuffers.find(index) == m_mapBuffers.end())
	{
		return NULL;
	}

	return m_mapBuffers[index];
}

void BufferObjectMgr::BufferData(GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage)
{
	GLuint bindBufName = GetBindBuffer(target);
	if (bindBufName == 0)
	{
		sConsoleLog->DebugWrite("BufferData:target[%d] not bind any buff name", target);
	}
	else
	{
		if(m_mapBuffers.find(bindBufName) == m_mapBuffers.end())
		{
			sConsoleLog->DebugWrite("BufferData:can't get buffName[%d]", bindBufName);
			return;
		}
		BufferObject *pBufObj = m_mapBuffers[bindBufName];
		if (pBufObj != NULL)
		{
			//sConsoleLog->Write("BufferData:buffer[%d] override", bindBufName);
			delete pBufObj;
			m_mapBuffers[bindBufName] = NULL;		
			pBufObj = NULL;
		}
		pBufObj						= new BufferObject((const char *)data, size, bindBufName);
		pBufObj->m_id				= bindBufName;
		m_mapBuffers[bindBufName]	= pBufObj;
	}
}

void BufferObjectMgr::BufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data)
{
	GLuint bindBufName = GetBindBuffer(target);
	if (bindBufName == 0)
	{
		sConsoleLog->DebugWrite("BufferSubData:target[%d] not bind any buff name", target);
	}
	else
	{
		BufferObject *pBufObj = m_mapBuffers[bindBufName];
		if (pBufObj == NULL)
		{
			sConsoleLog->DebugWrite("BufferSubData:buffer[%d] not exist", bindBufName);
		}
		else
		{
			pBufObj->Write((const char *)data, offset, size);
		}
	}
}


