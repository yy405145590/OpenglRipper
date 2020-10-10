#include "ShaderMgr.h"

extern std::string g_dumpFolder;
ShaderObject::ShaderObject(GLenum type, GLuint name)
{
	m_type = type;
	m_name = name;
}

ShaderObject::~ShaderObject()
{
}

void ShaderObject::ClearSource()
{
	m_sourceList.clear();
}

void ShaderObject::PushSource(const std::string &source)
{
	m_sourceList.push_back(source);
}

ShaderMgr::ShaderMgr(void)
{
}


ShaderMgr::~ShaderMgr(void)
{
}

void ShaderMgr::CreateShader(GLenum type, GLuint shaderName)
{
	if (m_mapShaderObj.find(shaderName) != m_mapShaderObj.end())
	{
		sConsoleLog->DebugWrite("CreateShader:shaderName had exist[%d,%d]", type, shaderName);
		if (m_mapShaderObj[shaderName] != NULL)
		{
			delete m_mapShaderObj[shaderName];
			m_mapShaderObj[shaderName] = NULL;
		}		
	}

	ShaderObject *pObj			= new ShaderObject(type, shaderName);
 	m_mapShaderObj[shaderName]	=	pObj;
}

void ShaderMgr::DeleteShader(GLuint shader)
{
// 	if (m_mapShaderObj.find(shader) == m_mapShaderObj.end())
// 	{
// 		sConsoleLog->DebugWrite("DeleteShader:shaderName had not exist[%d]", shader);
// 	}
// 	else
// 	{
// 		if (m_mapShaderObj[shader] != NULL)
// 		{
// 			delete m_mapShaderObj[shader];			
// 		}
// 		m_mapShaderObj.erase(shader);
// 	}
}

void ShaderMgr::DetachShader(GLuint program, GLuint shader)
{
//	return;
// 	if(m_mapProgram.find(program) == m_mapProgram.end())
// 	{
// 		sConsoleLog->DebugWrite("DetachShader:programName had not exist[%d]", program);
// 	}
// 	else
// 	{
// 		ProgramObject *pObj = m_mapProgram[program];
// 		if (pObj)
// 		{
// 			auto it = m_mapShaderObj.find(shader);
// 			if (it == m_mapShaderObj.end())
// 			{
// 				sConsoleLog->DebugWrite("DetachShader:Shader Object has not exist[%d]", shader);
// 				return;
// 			}
// 			if (it->second == NULL)
// 			{
// 				sConsoleLog->DebugWrite("DetachShader:Shader Object is null[%d]", shader);
// 				return;
// 			}
// 			pObj->m_mapShaderName.erase(it->second->m_type);
// 		}
// 		else
// 		{
// 			sConsoleLog->DebugWrite("AttachShader:programName Object is Null[%d]", program);
// 		}
// 	}
}

void ShaderMgr::ShaderSource(GLuint shader, 
							 GLsizei count, 
							 const char** string, 
							 const GLint *length)
{

 	if (m_mapShaderObj.find(shader) == m_mapShaderObj.end()
 		|| m_mapShaderObj[shader] == NULL)
 	{
 		sConsoleLog->DebugWrite("ShaderSource:shaderName had not exist[%d]", shader);
 		return;
 	}
 	ShaderObject *pObj = m_mapShaderObj[shader];
 	pObj->ClearSource();
 	for (int i = 0; i < count; ++i)
 	{
 		if (length == NULL || length[i] <= 0)
 		{
 			pObj->PushSource(string[i]);
 		}
 		else
 		{			
 			std::string source(string[i], length[i]);
 			pObj->PushSource(source);
 		}
 	}
}

void ShaderMgr::CreateProgram(GLuint programName)
{
 	if(m_mapProgram.find(programName) != m_mapProgram.end())
 	{
 		sConsoleLog->DebugWrite("CreateProgram:programName had exist[%d]", programName);
 		if (m_mapProgram[programName] != NULL)
 		{
 			delete m_mapProgram[programName];
 		}
 	}
 
 	ProgramObject *pObj			= new ProgramObject(programName);
 	m_mapProgram[programName]	= pObj;
}

void ShaderMgr::AttachShader(GLuint program, GLuint shader)
{
 	if(m_mapProgram.find(program) == m_mapProgram.end())
 	{
 		sConsoleLog->DebugWrite("AttachShader:programName had not exist[%d]", program);
 	}
 	else
 	{
 		ProgramObject *pObj = m_mapProgram[program];
 		if (pObj)
 		{
 			auto it = m_mapShaderObj.find(shader);
 			if (it == m_mapShaderObj.end())
 			{
 				sConsoleLog->DebugWrite("AttachShader:Shader Object has not exist[%d]", shader);
 				return;
 			}
 			if (it->second == NULL)
 			{
 				sConsoleLog->DebugWrite("AttachShader:Shader Object is null[%d]", shader);
 				return;
 			}
 			pObj->m_mapShaderName[it->second->m_type] = it->second->m_name;
 		}
 		else
 		{
 			sConsoleLog->DebugWrite("AttachShader:programName Object is Null[%d]", program);
 		}
 	}
}

void ShaderMgr::UseProgram(GLuint program)
{
	m_curProgram	= program;
}

void ShaderMgr::DumpShader(uint32 dumpIdx, const std::string &folder)
{
	auto it = m_mapProgram.find(m_curProgram);
	if (it == m_mapProgram.end())
	{
		sConsoleLog->DebugWrite("DumpShader:curProgram had not exist[%d]", m_curProgram);
		return;
	}

	if (it->second == NULL)
	{
		sConsoleLog->DebugWrite("DumpShader:curProgram is null[%d]", m_curProgram);
		return;
	}

	ProgramObject* pProgrameObj = it->second;
	for (auto itShader = pProgrameObj->m_mapShaderName.begin(); 
		itShader != pProgrameObj->m_mapShaderName.end(); ++itShader)
	{
		auto itObj = m_mapShaderObj.find(itShader->second);
		if (itObj == m_mapShaderObj.end() || itObj->second == NULL)
		{
			sConsoleLog->DebugWrite("DumpShader:Shader Object is null[%d][%d]", m_curProgram, itObj->second);
			continue;
		}
		char fileName[128] = "";
		const char *shaderType = "";
		if (itObj->second->m_type == GL_VERTEX_SHADER)
		{
			shaderType = "vs";
		}
		else if (itObj->second->m_type == GL_FRAGMENT_SHADER)
		{
			shaderType = "ps";
		}
		sprintf(fileName, "%s/%d.%s.txt", folder.c_str(), dumpIdx, shaderType);
		FILE* fp = fopen(fileName, "wb+");
		char newLine[] = "-------new shader line----------\r\n";
		if (fp)
		{
			for (auto itSource = itObj->second->m_sourceList.begin();
				itSource != itObj->second->m_sourceList.end(); ++itSource)
			{
				fwrite(itSource->c_str(),  itSource->length(), 1, fp);
				fwrite(newLine, sizeof(newLine), 1, fp);
			}

			fclose(fp);
		}
	}
}
// void ShaderMgr::DumpShader(uint32 dumpIdx, const std::string &folder)
// {
// 	GLint p = 0;
// 	g_old_pfnglGetIntegerv(GL_CURRENT_PROGRAM, &p);
// 	if (p == 0)
// 	{
// 		return;
// 	}
// 	GLuint		shaders[10];
// 	GLsizei		nCount = 0;
// 	g_old_pfnglGetAttachedShaders(p, sizeof(shaders)/sizeof(shaders[0]), &nCount, shaders);
// 	for (int i = 0; i < nCount; ++i)
// 	{
// 		GLint sType = 0;
// 		g_old_pfnglGetShaderiv(shaders[i], GL_SHADER_TYPE, &sType);
// 		GLint slength = 0;
// 		g_old_pfnglGetShaderiv(shaders[i], GL_SHADER_SOURCE_LENGTH, &slength);
// 		char *source = new char[slength];
// 		g_old_pfnglGetShaderSource(shaders[i], slength, &slength, source);
// 		char fileName[128] = "";
// 		const char *shaderType = "";
// 		if (sType == GL_VERTEX_SHADER)
// 		{
// 			shaderType = "vs";
// 		}
// 		else if (sType == GL_FRAGMENT_SHADER)
// 		{
// 			shaderType = "ps";
// 		}
// 		else
// 		{
// 			shaderType = "uk";
// 		}
// 		sprintf(fileName, "%s/%d.%s.txt", folder.c_str(), dumpIdx, shaderType);
// 		FILE* fp = fopen(fileName, "wb+");
// 		if (fp)
// 		{
// 			fwrite(source, slength, 1, fp);
// 			fclose(fp);
// 		}
// 		delete[] source;
// 	}
// }
