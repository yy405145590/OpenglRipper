#pragma once
#include "stdafx.h"
#include <map>
#include <set>
class BufferObject
{
public:
	friend class BufferObjectMgr;
	BufferObject(const char *pData, int size, uint32 id);
	~BufferObject();
	bool Write(const char *pData, int offset, int size);
	char*		m_pData;
	uint32		m_size;
	GLuint		m_id;
};

class BufferObjectMgr:public TlsInstance<BufferObjectMgr>
{
public:
	void BindBuffer(GLenum target, GLuint buffer);
	void GenBuffers(GLsizei n, GLuint *buffers);
	void BufferData(GLenum target,
		GLsizeiptr size,
		const GLvoid * data,
		GLenum usage);
	void BufferSubData(GLenum target,
		GLintptr offset,
		GLsizeiptr size,
		const GLvoid * data);
	void DeleteBuffers(GLsizei n,  const GLuint * buffers);

	GLuint GetBindBuffer(GLenum target);
	BufferObject* GetBufferObject(GLuint index);
	BufferObjectMgr(void);
	~BufferObjectMgr(void);
	void* MapPBOOffset(const void* pixels);
	void UnMapPBO();

private:	
	std::map<GLenum, uint32>		m_mapBufferBind;
	std::map<GLuint, BufferObject*> m_mapBuffers;
};

#define sBufferObjMgr BufferObjectMgr::GetInstance()

