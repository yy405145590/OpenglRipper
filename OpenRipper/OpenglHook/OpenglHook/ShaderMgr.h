#pragma once
#include "stdafx.h"
#include <map>
#include <set>
#include <vector>
#include <unordered_map>

struct ShaderObject
{
	ShaderObject(GLenum type, GLuint name);
	~ShaderObject();
	void ClearSource();
	void PushSource(const std::string &source);
	std::vector<std::string> m_sourceList;
	GLenum					 m_type;
	GLuint					 m_name;
};

struct ProgramObject
{
	std::unordered_map<GLenum, GLuint>	m_mapShaderName;
	GLuint						m_name;

	ProgramObject(GLuint name)
	{
		m_name = name;
	}
};



class ShaderMgr:public TlsInstance<ShaderMgr>
{
public:
	ShaderMgr(void);
	~ShaderMgr(void);	
	void	CreateShader(GLenum type, GLuint shaderName);
	void	DeleteShader(GLuint shader);
	void	ShaderSource(
				GLuint shader,  
				GLsizei count,  
				const char** string,  const GLint *length);
	void	CreateProgram(GLuint programName);
	void	AttachShader (GLuint program, GLuint shader);
	void	DetachShader(GLuint program,  GLuint shader);
	void    UseProgram (GLuint program);
	void	DumpShader(uint32 dumpIdx, const std::string &folder);
private:
	std::unordered_map<GLuint, ShaderObject*>		m_mapShaderObj;
	std::unordered_map<GLuint, ProgramObject*>		m_mapProgram;
	GLuint											m_curProgram;
};

#define sShaderMgr		ShaderMgr::GetInstance()

