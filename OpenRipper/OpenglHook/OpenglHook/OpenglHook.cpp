// OpenglHook.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <stdlib.h>

#include "DetoursGlDef.h"
#include "detours/detours.h"
#include "Log/Log.h"
#include "BufferObjectMgr.h"
#include "VertexAttribMgr.h"
#include "TextureMgr.h"
#include "ShaderMgr.h"
#include "EglContextMgr.h"
#include <conio.h>
#define  GLESV2 "libGLES_V2_translator.dll"
#define  EGL	"libEGl_translator.dll"
#include <unordered_map>
#include <vector>
#include <time.h>
#include <io.h>
#include <direct.h>
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
std::vector<std::string> SplitString(const std::string &src, const std::string &sep)
{
	std::vector<std::string> r;
	std::string s;
	for (std::string::const_iterator i = src.begin(); i != src.end(); i++) {
		if (sep.find(*i) != std::string::npos) {
			if (s.length()) 
				r.push_back(s);
			s = "";
		} else {
			s += *i;
		}
	}
	if (s.length()) r.push_back(s);
	return r;
}
bool g_bHooked		= false;
int  g_dumpIdx		= 0;
std::atomic<int>  g_dumpCommandIdx	= 0;
bool g_errReturn    = false;
bool g_bDumpTex		= false;
bool g_bDebug		= false;
GLsizei g_drawMaxCount = 1000000;
GLsizei g_drawMinCount = 0;
enum VertexAttribIndex
{
	VertexPosition = 0,
	VertexUv,
	VertexCount
};

std::unordered_map<GLenum, GLuint>		  m_slotBuffObj;

GLuint GetBindingTexture2D()
{
	GLint ret = 0;
	g_old_pfnglGetIntegerv(GL_TEXTURE_BINDING_2D, &ret);
	return ret;
}

void GL_APIENTRY DetourglBindBuffer(GLenum target, GLuint buffer)
{
	sBufferObjMgr->BindBuffer(target, buffer);
	g_old_pfnglBindBuffer(target, buffer);
}

void GL_APIENTRY DetourglGenBuffers(GLsizei n, GLuint *buffers)
{	
	g_old_pfnglGenBuffers(n, buffers);
	sBufferObjMgr->GenBuffers(n, buffers);
}

void GL_APIENTRY DetourglDeleteBuffers(GLsizei n, const GLuint *buffers)
{	
	sBufferObjMgr->DeleteBuffers(n, buffers);
	g_old_pfnglDeleteBuffers(n, buffers);
}

void  GL_APIENTRY DetourglBufferData (GLenum target, GLsizeiptr size, const void* data, GLenum usage)
{
	sBufferObjMgr->BufferData(target, size, data, usage);
	return g_old_pfnglBufferData(target, size, data, usage);
}

void GL_APIENTRY DetourglBufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, const void* data)
{
	sBufferObjMgr->BufferSubData(target, offset, size, data);
	return g_old_pfnglBufferSubData(target, offset, size, data);
}
GLuint gl_lastPointer = 0;
GLuint gl_lastUvPointer = 0;
void  GL_APIENTRY DetourglVertexAttribPointer (GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr)
{
	sVertexAttribMgr->VertexAttribPointer(indx, size, type, normalized, stride, ptr);
	return g_old_pfnglVertexAttribPointer(indx, size, type, normalized, stride, ptr);
}

void  GL_APIENTRY DetourglEnableVertexAttribArray(GLuint index)
{
	sVertexAttribMgr->EnableVertexAttribArray(index);
	g_old_pfnglEnableVertexAttribArray(index);
}

void  GL_APIENTRY DetourglDisableVertexAttribArray(GLuint index)
{
	sVertexAttribMgr->DisableVertexAttribArray(index);
	g_old_pfnglDisableVertexAttribArray(index);
}


void   GL_APIENTRY DetourglDrawElements (GLenum mode, GLsizei count, GLenum type, const void* indices)
{
	if(count > g_drawMaxCount || count < g_drawMinCount)
	{
		return;
	}
	sVertexAttribMgr->DrawElements(mode, count, type, indices);
	return g_old_pfnglDrawElements(mode, count, type, indices);
}

void   GL_APIENTRY DetourglDrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount)
{
	if (count > g_drawMaxCount || count < g_drawMinCount)
	{
		return;
	}
	sVertexAttribMgr->DrawElements(mode, count, type, indices);
	return g_old_pfnglDrawElements(mode, count, type, indices);
}

void GL_APIENTRY DetourglGenTextures (GLsizei n, GLuint* textures)
{	
	g_old_pfnglGenTextures(n, textures);
	sTextureMgr->GenTextures(n, textures);
}

void  GL_APIENTRY DetourglBindTexture (GLenum target, GLuint texture)
{	
	sTextureMgr->BindTexture(target, texture);
	return g_old_pfnglBindTexture(target, texture);
}

void  GL_APIENTRY DetourglActiveTexture (GLenum slot)
{
	sTextureMgr->ActiveTexture(slot);
	return g_old_pfnglActiveTexture(slot);
}
void GL_APIENTRY DetourglCompressedTexImage2D (GLenum target, 
	 GLint level, 
	 GLenum internalformat, 
	 GLsizei width, 
	 GLsizei height, 
	 GLint border, 
	 GLsizei imageSize, 
	 const void* data)
{
	sTextureMgr->CompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
	g_old_pfnglCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
}


void GL_APIENTRY DetourglTexImage2D (GLenum target, 
							   GLint level, 
							   GLenum internalformat, 
							   GLsizei width, 
							   GLsizei height, 
							   GLint border, 
							   GLenum format, 
							   GLenum type, 
							   const void* pixels)
{
	sTextureMgr->TexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
	g_old_pfnglTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
}

void GL_APIENTRY DetourglDeleteTextures (GLsizei n, const GLuint* textures)
{
	sTextureMgr->DeleteTextures(n, textures);
	g_old_pfnglDeleteTextures(n, textures);
}

void  GL_APIENTRY DetourglCompressedTexSubImage2D (
	GLenum target, 
	GLint level, 
	GLint xoffset, 
	GLint yoffset, 
	GLsizei width, 
	GLsizei height, 
	GLenum format, 
	GLsizei imageSize, 
	const void* data)
{
	sTextureMgr->CompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);
	return g_old_pfnglCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);
}

void  GL_APIENTRY DetourglCopyTexImage2D (
	GLenum target,
	GLint level, 
	GLenum internalformat, 
	GLint x, GLint y, 
	GLsizei width, GLsizei height,
	GLint border)
{
	sTextureMgr->CopyTexImage2D(target, level, internalformat, x, y, width, height, border);
	return g_old_pfnglCopyTexImage2D(target, level, internalformat, x, y, width, height, border);
}
 void GL_APIENTRY DetourglCopyTexSubImage2D (
	 GLenum target, 
	 GLint level, 
	 GLint xoffset,
	 GLint yoffset, 
	 GLint x, GLint y, 
	 GLsizei width, 
	 GLsizei height)
 {
	 sTextureMgr->CopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
	 return g_old_pfnglCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
 }

 void GL_APIENTRY DetourglFramebufferTexture2D(GLenum target,  
	 GLenum attachment,  GLenum textarget,  
	 GLuint texture,  GLint level)
 {
	 sTextureMgr->FramebufferTexture2D(target, attachment, textarget, texture, level);
	 return g_old_pfnglFramebufferTexture2D(target, attachment, textarget, texture, level);
 }

void GL_APIENTRY DetourglBindFramebuffer(GLenum target,  GLuint framebuffer)
{
	sTextureMgr->BindFramebuffer(target, framebuffer);
	return g_old_pfnglBindFramebuffer(target, framebuffer);
}

 void GL_APIENTRY DetourglTexSubImage2D (
	 GLenum target, 
	 GLint level, 
	 GLint xoffset, 
	 GLint yoffset, 
	 GLsizei width, 
	 GLsizei height, 
	 GLenum format, 
	 GLenum type, 
	 const void* pixels)
 {
	 sTextureMgr->TexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
	 return g_old_pfnglTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
 }

 GLuint  GL_APIENTRY DetourglCreateShader (GLenum type)
 {
	GLuint shader = g_old_pfnglCreateShader(type);
	sShaderMgr->CreateShader(type, shader);
	return shader;
 }

 void  GL_APIENTRY DetourglDeleteShader (GLuint shader)
 {
	 sShaderMgr->DeleteShader(shader);
	 return g_old_pfnglDeleteShader(shader);
 }

 void  GL_APIENTRY DetourglShaderSource (GLuint shader, GLsizei count, const char** string, const GLint* length)
 {
	 sShaderMgr->ShaderSource(shader, count, string, length);
	 return g_old_pfnglShaderSource(shader, count, string, length);
 }

 GLuint  GL_APIENTRY DetourglCreateProgram (void)
 {
	GLuint pro = g_old_pfnglCreateProgram();
	sShaderMgr->CreateProgram(pro);
	return pro;
 }

 void    GL_APIENTRY DetourglAttachShader (GLuint program, GLuint shader)
 {
	 sShaderMgr->AttachShader(program, shader);
	 return g_old_pfnglAttachShader(program, shader);
 }

 void   GL_APIENTRY DetourglDetachShader (GLuint program, GLuint shader)
 {
	 sShaderMgr->DetachShader(program, shader);
	 return g_old_pfnglDetachShader(program, shader);
 }

 void  GL_APIENTRY DetourglUseProgram (GLuint program)
 {
	 sShaderMgr->UseProgram(program);
	 return g_old_pfnglUseProgram(program);
 }

void   GL_APIENTRY DetourglFinish ()
{
	sVertexAttribMgr->Finish();
	return g_old_pfnglFinish();
}


EGLContext EGLAPIENTRY DetoureglCreateContext(EGLDisplay dpy, EGLConfig config,
											   EGLContext share_context,
											   const EGLint *attrib_list)
{
	EGLContext context = g_old_pfneglCreateContext(dpy, config, share_context, attrib_list);
	sEglContextMgr->CreateContext(context, share_context);
	return context;
}

void GameLoop();
EGLBoolean EGLAPIENTRY DetoureglSwapBuffers(EGLDisplay dpy, EGLSurface surface)
{
	GameLoop();
	return g_old_pfneglSwapBuffers(dpy, surface);
}
#define DUMP_BASE_FOLDER	"./gldump/"
std::string g_dumpFolder = DUMP_BASE_FOLDER;

#define GET_FUN_EGL(P2) g_old_pfn##P2 = (pfn##P2)DetourFindFunction(EGL, #P2);
#define GET_FUN_GLESV2(P2) g_old_pfn##P2 = (pfn##P2)DetourFindFunction(GLESV2, #P2);

#define ATTACH_FUN_GLESV2( P2)	g_old_pfn##P2 = (pfn##P2)DetourFindFunction(GLESV2, #P2);\
	DetourAttach( (PVOID *)&g_old_pfn##P2,		Detour##P2); 

#define ATTACH_FUN_EGL( P2)	g_old_pfn##P2 = (pfn##P2)DetourFindFunction(EGL, #P2);\
	DetourAttach( (PVOID *)&g_old_pfn##P2,		Detour##P2);

void UpdateNewWork()
{

}
void InstallHook()
{	
	if(g_bHooked)
	{
		return;
	}
	g_bHooked = true;
	DetourTransactionBegin();  
	DetourUpdateThread( GetCurrentThread() ); 

	ATTACH_FUN_GLESV2(glFinish);

	//buffer object
	ATTACH_FUN_GLESV2(glBindBuffer);
	ATTACH_FUN_GLESV2(glBufferData);
	ATTACH_FUN_GLESV2(glBufferSubData);
	ATTACH_FUN_GLESV2(glGenBuffers);
	ATTACH_FUN_GLESV2(glDeleteBuffers);

	//vertex attrib
	ATTACH_FUN_GLESV2(glEnableVertexAttribArray);
	ATTACH_FUN_GLESV2(glDisableVertexAttribArray);
	ATTACH_FUN_GLESV2(glVertexAttribPointer);

	//shader

	ATTACH_FUN_GLESV2(glCreateShader);
	ATTACH_FUN_GLESV2(glDeleteShader);
	ATTACH_FUN_GLESV2(glShaderSource);
	ATTACH_FUN_GLESV2(glCreateProgram);
	ATTACH_FUN_GLESV2(glAttachShader);
	ATTACH_FUN_GLESV2(glDetachShader);
	ATTACH_FUN_GLESV2(glUseProgram);

	//texture
	ATTACH_FUN_GLESV2(glGenTextures);
	ATTACH_FUN_GLESV2(glDeleteTextures);
	ATTACH_FUN_GLESV2(glBindTexture);
	ATTACH_FUN_GLESV2(glTexImage2D);
	ATTACH_FUN_GLESV2(glActiveTexture);
	ATTACH_FUN_GLESV2(glCompressedTexImage2D);
	ATTACH_FUN_GLESV2(glCompressedTexSubImage2D);
	ATTACH_FUN_GLESV2(glCopyTexImage2D);
	ATTACH_FUN_GLESV2(glCopyTexSubImage2D);
	ATTACH_FUN_GLESV2(glTexSubImage2D);
	ATTACH_FUN_GLESV2(glFramebufferTexture2D);
	ATTACH_FUN_GLESV2(glBindFramebuffer);

	ATTACH_FUN_GLESV2(glDrawElements);
	ATTACH_FUN_GLESV2(glDrawElementsInstanced);
	ATTACH_FUN_EGL(eglSwapBuffers);
	ATTACH_FUN_EGL(eglCreateContext);
	LONG ret = DetourTransactionCommit();

	GET_FUN_GLESV2(glGetVertexAttribiv);
	GET_FUN_GLESV2(glGetVertexAttribPointerv);
	GET_FUN_GLESV2(glGetIntegerv);
	GET_FUN_GLESV2(glGetProgramiv);
	GET_FUN_GLESV2(glGetActiveAttrib);
	GET_FUN_GLESV2(glGetAttribLocation);	
	GET_FUN_GLESV2(glGetError);	
	GET_FUN_GLESV2(glPixelStorei);
	GET_FUN_GLESV2(glReadPixels);
	GET_FUN_GLESV2(glMapBufferRange);
	GET_FUN_GLESV2(glUnmapBuffer);
	GET_FUN_GLESV2(glGetFramebufferAttachmentParameteriv);
	GET_FUN_GLESV2(glGetAttachedShaders);
	GET_FUN_GLESV2(glGetShaderSource);
	GET_FUN_GLESV2(glGetShaderiv);
	GET_FUN_EGL(eglGetCurrentContext);
	GET_FUN_EGL(eglGetCurrentSurface);
	ConsoleLog::GetInstance()->Write("Welcome to ogl ripper.");
	ConsoleLog::GetInstance()->Write("ripper Install Ok.");
	ConsoleLog::GetInstance()->Write("dump: dump current scene.");
}

void UnInstallHook()
{
	if(!g_bHooked) return;
	DetourTransactionBegin();  
	DetourUpdateThread( GetCurrentThread() );  
	DetourDetach( (PVOID *)&g_old_pfnglBindBuffer,		DetourglBindBuffer		);  
	DetourDetach( (PVOID *)&g_old_pfnglBufferData,		DetourglBufferData		);  
	DetourDetach( (PVOID *)&g_old_pfnglBufferSubData,	DetourglBufferSubData	);  
	DetourDetach( (PVOID *)&g_old_pfnglDrawElements,	DetourglDrawElements	); 
	DetourDetach( (PVOID *)&g_old_pfnglVertexAttribPointer, DetourglVertexAttribPointer);
	DetourDetach( (PVOID *)&g_old_pfneglSwapBuffers,	DetoureglSwapBuffers	); 
	LONG ret=DetourTransactionCommit();

	ConsoleLog::GetInstance()->DebugWrite("UnInstallHook Ok");
}


BOOL RecursiveDirectory(std::string wstrDir)
{
	if (wstrDir.length() <= 3)
	{
		return FALSE;
	}
	if (wstrDir[wstrDir.length() - 1] == '\\')
	{
		wstrDir.erase(wstrDir.end() - 1);
	}

	if (PathFileExists(wstrDir.c_str()))
		return TRUE;

	if (CreateDirectory(wstrDir.c_str(), NULL) == false)
	{
		std::string wstrNewDir = wstrDir;
		while (wstrNewDir[wstrNewDir.length() - 1] != '\\' 
			&& wstrNewDir[wstrNewDir.length() - 1] != '/')
		{
			wstrNewDir.erase(wstrNewDir.length() - 1);
		}
		// delete '\\'   
		wstrNewDir.erase(wstrNewDir.length() - 1);

		RecursiveDirectory(wstrNewDir);
		CreateDirectory(wstrDir.c_str(), NULL);
	}

	if (!PathFileExists(wstrDir.c_str()))
		return FALSE;
	return TRUE;
}
void MakeAndCreateDumpFolder(const std::string& folder)
{
	struct tm *local; 
	time_t t; 
	t=time(NULL); 
	local=localtime(&t);
	char dumpFolder[128] = "";
	char cIniFileName[MAX_PATH];
	GetModuleFileName(NULL, cIniFileName, sizeof(cIniFileName));
	char * p = strrchr(cIniFileName, '\\');
	g_dumpFolder = folder;
	sprintf(dumpFolder, "%s/%04d_%03d_%02d_%02d_%02d",p, local->tm_year + 1900, local->tm_yday, local->tm_hour, local->tm_min, local->tm_sec);	
	g_dumpFolder += dumpFolder;
	RecursiveDirectory(g_dumpFolder);
}
void GameLoop()
{
	if (g_bDumpTex)
	{
		g_bDumpTex = false;
		ConsoleLog::GetInstance()->DebugWrite("g_bDumpTex End");
	}
	if(g_bDebug)
	{
		ConsoleLog::GetInstance()->DebugWrite("g_bDebug End");
	}

	g_bDebug = false;
	std::string cmd;
	cmd = ConsoleLog::GetInstance()->PopInput();
	if(cmd != "")
	{
		std::vector<std::string> strVec = SplitString(cmd, " ");
		if (strVec.size() > 1 && strVec[0] == "v")
		{
			g_drawMaxCount = atoi(strVec[1].c_str());
			ConsoleLog::GetInstance()->DebugWrite("set g_greadterCount = %d", g_drawMaxCount);
		}
		else if(strVec[0] == "dump")
		{
			g_dumpCommandIdx++;
			MakeAndCreateDumpFolder("./ogldump");
		}
		else if(strVec[0] == "tex")
		{
			g_bDumpTex = true;
		}
		else if(strVec[0] == "e")
		{
			g_errReturn = !g_errReturn;
			ConsoleLog::GetInstance()->DebugWrite("set g_errReturn:%d", g_errReturn);
		}
		else if(strVec[0] == "debug")
		{
			g_bDebug = true;
			ConsoleLog::GetInstance()->DebugWrite("set g_errReturn:%d", g_bDebug);
		}
 	}
	sClientMsg->Update(40);
}

void Main()
{
	InstallHook();
	sClientMsg;
}

void main()
{
	InstallHook();
}