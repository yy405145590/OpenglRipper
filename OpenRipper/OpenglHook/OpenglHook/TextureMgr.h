#pragma once
#include "stdafx.h"
#include <map>

struct TextureObject
{
	const char*		_pBuffer;
	unsigned int	_buffSize;
	TexAttrib		_texAttrib;
	TextureObject(unsigned int size, const char* pData)
	{
		_pBuffer	= new char[size];
		_buffSize	= size;		
		if (pData)
		{			
			memcpy((void *)_pBuffer, pData, size);
		}
		else
		{
			memset((void *)_pBuffer, 0x00, _buffSize);
		}
	}
	~TextureObject()
	{
		if (_pBuffer)
		{
			delete[] _pBuffer;
		}
	}
};
class TextureMgr:public TlsInstance<TextureMgr>
{
public:
	TextureMgr(void);
	~TextureMgr(void);
	void GenTextures(GLsizei n, GLuint* textures);
	void DeleteTextures(GLsizei n, const GLuint* textures);
	void BindTexture(GLenum target, GLuint texture);
	void TexImage2D(GLenum target, 
		GLint level, 
		GLenum internalformat, 
		GLsizei width, GLsizei height, 
		GLint border, GLenum format, 
		GLenum type, const void* pixels);
	void ActiveTexture(GLenum texture);
	void CompressedTexImage2D(GLenum target, 
		GLint level, 
		GLenum internalformat, 
		GLsizei width, GLsizei height, 
		GLint border, GLsizei imageSize, 
		const void* data);
	void CompressedTexSubImage2D(GLenum target, 
		GLint level, 
		GLint xoffset, GLint yoffset, 
		GLsizei width, GLsizei height, 
		GLenum format, GLsizei imageSize, const void* data);
	void CopyTexImage2D(GLenum target, 
		GLint level, 
		GLenum internalformat, 
		GLint x, GLint y, 
		GLsizei width, GLsizei height, 
		GLint border);
	void CopyTexSubImage2D(GLenum target, 
		GLint level, 
		GLint xoffset, GLint yoffset, 
		GLint x, GLint y, 
		GLsizei width, GLsizei height);
	void TexSubImage2D(GLenum target, 
		GLint level, 
		GLint xoffset, GLint yoffset, 
		GLsizei width, GLsizei height, 
		GLenum format, GLenum type, 
		const void* pixels);
	void BindFramebuffer(GLenum target,  GLuint framebuffer);
	void FramebufferTexture2D(GLenum target,  
		GLenum attachment,  
		GLenum textarget,  
		GLuint texture,  
		GLint level);

	void DumpTexture(uint32 dumpIdx, const std::string &folder);
	void DumpAllTexture(const std::string &folder);

private:
	GLuint GetActiveTextureName(GLenum target);

	GLenum	m_activeTextureSlot;
	std::map<GLenum, std::map<GLenum, GLuint> > m_mapTextureSlot;
	std::map<GLuint, TextureObject*>			m_mapTextureObj;
	GLuint	m_frameBufferTex;
	GLuint  m_frameBuffer;
	uint32  GetBytesPerPixel(GLenum internalFormat, GLenum format, GLenum type,
		uint32 width, uint32 height);
};


#define sTextureMgr		TextureMgr::GetInstance()