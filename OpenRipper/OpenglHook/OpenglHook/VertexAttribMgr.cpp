#include "stdafx.h"
#include "VertexAttribMgr.h"
#include "TextureMgr.h"
#include "ShaderMgr.h"
#include <io.h>
#include <direct.h>

extern std::atomic<int>  g_dumpCommandIdx;
extern std::string g_dumpFolder;
VertexAttribMgr::VertexAttribMgr(void)
{
	m_dumpIdx			= 0;
	m_dumpCommandIdx	= 0;
	m_bDump				= false;
}


VertexAttribMgr::~VertexAttribMgr(void)
{
}

void VertexAttribMgr::VertexAttribPointer(
	GLuint indx, 
	GLint size, 
	GLenum type,
	GLboolean normalized, 
	GLsizei stride, 
	const void* ptr)
{
	GLuint	vbo = sBufferObjMgr->GetBindBuffer(GL_ARRAY_BUFFER);
	if (vbo == 0)
	{
		//sConsoleLog->Write("VertexAttribPointer:vbo == 0");
		return;
	}
	m_mapVertexAttrib[indx]._vbo						= vbo;
	m_mapVertexAttrib[indx]._vertexAttrib.offset		= ptr;
	m_mapVertexAttrib[indx]._vertexAttrib._indx			= indx;
	m_mapVertexAttrib[indx]._vertexAttrib._normalized	= normalized;
	m_mapVertexAttrib[indx]._vertexAttrib._size			= size;
	m_mapVertexAttrib[indx]._vertexAttrib._stride		= stride;
	m_mapVertexAttrib[indx]._vertexAttrib._type			= type;
}
int VertexAttribMgr::GetSizeFromGLType(GLenum type)
{
	switch (type)
	{
	case GL_BYTE:
		return sizeof(GLbyte);
	case GL_UNSIGNED_BYTE:
		return sizeof(GLubyte);
	case GL_SHORT:
		return sizeof(GLshort);
	case GL_UNSIGNED_SHORT:
		return sizeof(GLushort);
	case GL_INT:
		return sizeof(GLint);
	case GL_UNSIGNED_INT:
		return sizeof(GLuint);
	case GL_FLOAT:
		return sizeof(GLfloat);
	case GL_FIXED:
		return sizeof(GLfixed);
	default:
		break;
	}
	return 0;
}

void VertexAttribMgr::DumpIndexBuffer(GLenum mode, GLsizei count, GLenum type, const void* indices, const std::string &folder)
{
#pragma warning(push)
#pragma warning(disable:4311 4302)
	UINT32 nIndices = reinterpret_cast<UINT32>(indices);
#pragma warning(pop)
	GLuint ibo = sBufferObjMgr->GetBindBuffer(GL_ELEMENT_ARRAY_BUFFER);
	if (ibo == 0)
	{
		sConsoleLog->DebugWrite("DumpIndexBuffer: ibo == 0");
		return;
	}
	BufferObject *pBufObj = sBufferObjMgr->GetBufferObject(ibo);
	if (pBufObj == NULL || pBufObj->m_pData == NULL)
	{
		sConsoleLog->DebugWrite("DumpIndexBuffer: ibo = %d,BufferObject == NULL", ibo);
		return;
	}

	char indexFileName[128] = "";
	sprintf(indexFileName, "%s/%d.indx",folder.c_str(), m_dumpIdx);
	FILE* fp = fopen(indexFileName, "wb+");
	if(fp)
 	{
 		fwrite(&mode, sizeof(mode), 1, fp);
 		fwrite(&count, sizeof(count), 1, fp);
 		fwrite(&type, sizeof(type), 1, fp);
 		fwrite(&nIndices, sizeof(nIndices), 1, fp);
 		int sizeType = GetSizeFromGLType(type);
 		if (sizeType == 0)
 		{
			sConsoleLog->DebugWrite("DumpIndexBuffer:type[%d] not DumpIndexBuffer", type);
 			fwrite(pBufObj->m_pData, pBufObj->m_size, 1, fp);
 		}
 		else
 		{
 			if ((nIndices + count * sizeType) <= pBufObj->m_size)
 			{
 				fwrite(pBufObj->m_pData + nIndices, count * sizeType, 1, fp);
 			}							
 			else
 			{
 				sConsoleLog->DebugWrite("DumpIndexBuffer: Error %d > %d", 
 					(nIndices + count * sizeType) * sizeType, pBufObj->m_size);
 			}
 		}
 		fflush(fp);
 		fclose(fp);
 
 	}				
}
void VertexAttribMgr::DumpVertex(const std::string &folder)
{
	GLint program = 0;
	g_old_pfnglGetIntegerv(GL_CURRENT_PROGRAM, &program);
	if (program != 0)
	{
		
		GLint attrCount = -1;
		g_old_pfnglGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &attrCount);
		for (int i = 0; i < attrCount; ++i)
		{
			char name[128];
			memset(name, 0x00, sizeof(name));
			GLint	size	= 0;
			GLenum	type	= 0;
			g_old_pfnglGetActiveAttrib(program, i, sizeof(name), NULL, &size, &type, name);
			int locIdx = g_old_pfnglGetAttribLocation(program, name);
			if (m_mapVertexAttrib[locIdx]._vbo != 0 && m_mapVertexAttrib[locIdx]._enabled)
			{
				BufferObject *pBufObj = sBufferObjMgr->GetBufferObject(m_mapVertexAttrib[locIdx]._vbo);
				if (pBufObj == NULL || pBufObj->m_pData == NULL)
				{
					sConsoleLog->DebugWrite("DumpVertex: vbo = %d,BufferObject == NULL", m_mapVertexAttrib[locIdx]._vbo);
					return;
				}
				char fileName[128] = "";
				sprintf(fileName, "%s/%d.%s", folder.c_str(), m_dumpIdx, name);
				void *offset = NULL;
// 				g_old_pfnglGetVertexAttribPointerv(locIdx, GL_VERTEX_ATTRIB_ARRAY_POINTER, &offset);
// 				int32 stride = 0;
// 				g_old_pfnglGetVertexAttribiv(locIdx, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
// 				int32 vbo = 0;
// 				g_old_pfnglGetVertexAttribiv(locIdx, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &vbo);
// 				if (offset != m_mapVertexAttrib[locIdx]._vertexAttrib.offset)
// 				{
// 					sConsoleLog->Write("DumpVertex:offset %d:%d", offset, m_mapVertexAttrib[locIdx]._vertexAttrib.offset);
// 				}
// 				if (stride != m_mapVertexAttrib[locIdx]._vertexAttrib._stride)
// 				{
// 					sConsoleLog->Write("DumpVertex:stride %d:%d", stride, m_mapVertexAttrib[locIdx]._vertexAttrib._stride);
// 				}
// 				if (vbo != m_mapVertexAttrib[locIdx]._vbo)
// 				{
// 					sConsoleLog->Write("DumpVertex:vbo %d:%d", vbo, m_mapVertexAttrib[locIdx]._vbo);
// 				}
				FILE* fp = fopen(fileName, "wb+");
				if (fp)
				{
					fwrite(&m_mapVertexAttrib[locIdx]._vertexAttrib, sizeof(m_mapVertexAttrib[locIdx]._vertexAttrib), 1, fp);
					fwrite(pBufObj->m_pData,  pBufObj->m_size, 1, fp);
					fclose(fp);
				}
			}
			else
			{
				sConsoleLog->DebugWrite("DumpVertex:locIdx's vbo == 0 or enabled == false");
			}
		}
	}

}

void VertexAttribMgr::DrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices)
{
	if (m_bDump)
	{
		DumpIndexBuffer(mode, count, type, indices, m_folder);
		DumpVertex(m_folder);
		sTextureMgr->DumpTexture(m_dumpIdx, m_folder);		
		sShaderMgr->DumpShader(m_dumpIdx, m_folder);
		m_dumpIdx ++;
	}	
}

void VertexAttribMgr::EnableVertexAttribArray(GLuint index)
{
	m_mapVertexAttrib[index]._enabled = true;
}

void VertexAttribMgr::DisableVertexAttribArray(GLuint index)
{
	//m_mapVertexAttrib[index]._enabled = false;
}

void VertexAttribMgr::Finish()
{
	if (m_bDump)
	{		
		sConsoleLog->Write("Finish:%p surface dump finish", GetID());
		m_bDump = false;
	}
	if (m_dumpCommandIdx != g_dumpCommandIdx)
	{
		char folder[128];
		sprintf(folder, "%s/%p", g_dumpFolder.c_str(), GetID());
		m_folder = folder;
		if (_access(m_folder.c_str(), 0) == -1)  
		{
			_mkdir(m_folder.c_str());
			sConsoleLog->Write("Finish folder:%s", m_folder.c_str());
		}
		m_bDump = true;
		m_dumpCommandIdx = g_dumpCommandIdx;
		sConsoleLog->Write("Finish:%p surface dump begin", GetID());
		sTextureMgr->DumpAllTexture(m_folder);
	}
}