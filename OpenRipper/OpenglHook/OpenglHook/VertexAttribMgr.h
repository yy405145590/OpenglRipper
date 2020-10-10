#pragma once
#include <map>
#include "DetoursGlDef.h"
#include "BufferObjectMgr.h"
struct VertexAttrib
{
	GLuint				_vbo;
	bool				_enabled;
	VertexAttribParam	_vertexAttrib;

	VertexAttrib()
	{
		_vbo = 0;
		_enabled = false;
	}
};
class VertexAttribMgr:public TlsInstance<VertexAttribMgr>
{
public:
	VertexAttribMgr(void);
	~VertexAttribMgr(void);
	void VertexAttribPointer(
		GLuint indx, 
		GLint size, 
		GLenum type, 
		GLboolean normalized, 
		GLsizei stride, 
		const void* ptr);


	int GetSizeFromGLType(GLenum type);
	void EnableVertexAttribArray(GLuint index);
	void DisableVertexAttribArray(GLuint index);

	void DumpVertex(const std::string &folder);
	void DumpIndexBuffer(GLenum mode, GLsizei count, GLenum type, const void* indices, const std::string &folder);

	void DrawElements (GLenum mode, GLsizei count, GLenum type, const void* indices);

	void Finish();

private:
	std::map<GLuint, VertexAttrib>	m_mapVertexAttrib;
	uint32							m_dumpIdx;
	int32							m_dumpCommandIdx;
	bool							m_bDump;
	std::string						m_folder;
};

#define sVertexAttribMgr VertexAttribMgr::GetInstance()

