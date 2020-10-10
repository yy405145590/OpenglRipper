#include "TextureMgr.h"
#include <io.h>
#include <direct.h>
#include "PVRTexture.h"
#include "PVRTextureUtilities.h"
#include "BufferObjectMgr.h"
using namespace pvrtexture;
extern std::string g_dumpFolder;
TextureMgr::TextureMgr(void)
{
	m_activeTextureSlot = GL_TEXTURE0;
	m_frameBufferTex	= 0;
	m_frameBuffer		= 0;
}


TextureMgr::~TextureMgr(void)
{

}

void TextureMgr::ActiveTexture(GLenum texture)
{
	m_activeTextureSlot	= texture;
}

void TextureMgr::BindTexture(GLenum target, GLuint texture)
{
	m_mapTextureSlot[m_activeTextureSlot][target] = texture;
}

GLuint TextureMgr::GetActiveTextureName(GLenum target)
{
	return m_mapTextureSlot[m_activeTextureSlot][target];
}

void TextureMgr::CompressedTexImage2D(GLenum target, 
									  GLint level, 
									  GLenum internalformat, 
									  GLsizei width,
									  GLsizei height, 
									  GLint border, 
									  GLsizei imageSize, const void* data)
{

	GLuint tex = GetActiveTextureName(target);
	if (level != 0)
	{
		return;
	}
	if (tex != 0)
	{
		if (m_mapTextureObj.find(tex) == m_mapTextureObj.end())
		{
			sConsoleLog->DebugWrite("CompressedTexImage2D: tex[%d] is not exist", tex);
			return;
		}
		if (m_mapTextureObj[tex] != NULL)
		{
			delete m_mapTextureObj[tex];
			m_mapTextureObj[tex] = NULL;
		}

		TextureObject *pObj = new TextureObject(imageSize, (const char *)data);
		pObj->_texAttrib._level				 = level;
		pObj->_texAttrib._internalFormat	 = internalformat;
		pObj->_texAttrib._width				 = width;
		pObj->_texAttrib._height			 = height;
		pObj->_texAttrib._border			 = border;
		m_mapTextureObj[tex]				 = pObj;
	}
	else
	{
		sConsoleLog->DebugWrite("CompressedTexImage2D: tex is 0");
	}
}

uint32 TextureMgr::GetBytesPerPixel(GLenum internalFormat, GLenum format, GLenum type, uint32 width, uint32 height)
{
	CPVRTextureHeader cHeader;
	cHeader.setOGLESFormat(internalFormat, format, type);
	cHeader.setWidth(width);
	cHeader.setHeight(height);
	CPVRTexture cTexture(cHeader);
	return cTexture.getBitsPerPixel() / 8;
}

void TextureMgr::CompressedTexSubImage2D(GLenum target, 
										 GLint level, 
										 GLint xoffset, 
										 GLint yoffset, 
										 GLsizei width, GLsizei height, 
										 GLenum format, GLsizei imageSize, const void* data)
{	
	if (IsBadReadPtr(data, 4) != 0)
	{
		sConsoleLog->DebugWrite("TexSubImage2D: pixels[0x%x] IsBadReadPtr", data);
		sBufferObjMgr->UnMapPBO();
		return;
	}
	if (imageSize != 0 && level == 0)
	{
		GLuint tex = GetActiveTextureName(target);
		if (m_mapTextureObj.find(tex) == m_mapTextureObj.end())
		{
			sConsoleLog->DebugWrite("TexSubImage2D: tex[%d] is not exist", tex);
			return;
		}
		if (m_mapTextureObj[tex] == NULL)
		{
			sConsoleLog->DebugWrite("TexSubImage2D: tex[%d] is NULL", tex);
			return;
		}


		TextureObject *pObj = m_mapTextureObj[tex];
		if (pObj->_texAttrib._format != format)
		{
			sConsoleLog->DebugWrite("TexSubImage2D: format[%d]:[%d] is not match", pObj->_texAttrib._format, format);
			return;
		}
		uint32 bytesNum = GetBytesPerPixel(format, format, pObj->_texAttrib._target, width, height);
		{
			memcpy((void *)(pObj->_pBuffer + (yoffset * pObj->_texAttrib._width + xoffset) * bytesNum),
				(const char *)data, imageSize);
		}
	}
	sBufferObjMgr->UnMapPBO();
}
uint32 NextPOT(uint32 x )
{
	x = x - 1;
	x = x | ( x >> 1);
	x = x | ( x >> 2);
	x = x | ( x >> 4);
	x = x | ( x >> 8);
	x = x | ( x >>16);
	return x + 1;
}
void TextureMgr::CopyTexImage2D(GLenum target, 
								GLint level, GLenum internalformat, 
								GLint x, GLint y, 
								GLsizei width, GLsizei height, GLint border)
{
	//sConsoleLog->Write("CopyTexImage2D");
	width = NextPOT(width);
	height = NextPOT(height);
	int *pixel = (int*)malloc(sizeof(int) * width * height);
	memset(pixel,0,sizeof(int) * width * height);
	g_old_pfnglPixelStorei(GL_PACK_ALIGNMENT,1);
	g_old_pfnglReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
	TexImage2D(target, 0, GL_RGBA, width, height, border, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
	delete[] pixel;
}

void TextureMgr::CopyTexSubImage2D(GLenum target, 
								   GLint level, 
								   GLint xoffset, GLint yoffset, 
								   GLint x, GLint y, 
								   GLsizei width, GLsizei height)
{
	//sConsoleLog->Write("CopyTexSubImage2D not support");
	width = NextPOT(width);
	height = NextPOT(height);
	int *pixel = (int*)malloc(sizeof(int) * width * height);
	memset(pixel,0,sizeof(int) * width * height);
	g_old_pfnglPixelStorei(GL_PACK_ALIGNMENT,1);
	g_old_pfnglReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
	//TexSubImage2D(target, level, xoffset, yoffset, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
	TexImage2D(target, level, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
	delete[] pixel;
}

void TextureMgr::DeleteTextures(GLsizei n, const GLuint* textures)
{
	for (int i = 0; i < n; ++i)
	{
		if (m_mapTextureObj.find(textures[i]) == m_mapTextureObj.end())
		{
			sConsoleLog->DebugWrite("GenTextures:tex[%d] is not exist", textures[i]);
		}
		else
		{
			if(m_mapTextureObj[textures[i]] != NULL)
			{
				delete m_mapTextureObj[textures[i]];
				m_mapTextureObj[textures[i]] = NULL;
			}
			m_mapTextureObj.erase(textures[i]);
		}
	}
}

void TextureMgr::GenTextures(GLsizei n, GLuint* textures)
{
	for (int i = 0; i < n; ++i)
	{
		if (m_mapTextureObj.find(textures[i]) != m_mapTextureObj.end())
		{
			sConsoleLog->DebugWrite("GenTextures:tex[%d] had exist", textures[i]);
		}
		else
		{
			m_mapTextureObj[textures[i]] = NULL;
		}
	}
}

void TextureMgr::TexImage2D(GLenum target, 
							GLint level, GLenum internalformat, 
							GLsizei width, GLsizei height, 
							GLint border, GLenum format, GLenum type, 
							const void* pixels)
{
	pixels = sBufferObjMgr->MapPBOOffset(pixels);
	GLuint tex = GetActiveTextureName(target);
	if (level != 0)
	{
		return;
	}
	if (pixels != 0 && IsBadReadPtr(pixels, 4) != 0)
	{
		sConsoleLog->DebugWrite("TexImage2D: pixels[0x%x] IsBadReadPtr", pixels);
		sBufferObjMgr->UnMapPBO();
		return;
	}
	if (tex != 0)
	{
		if (m_mapTextureObj.find(tex) == m_mapTextureObj.end())
		{
			sConsoleLog->DebugWrite("TexImage2D: tex[%d] is not exist", tex);
			sBufferObjMgr->UnMapPBO();
			return;
		}
		if (m_mapTextureObj[tex] != NULL)
		{
			delete m_mapTextureObj[tex];
			m_mapTextureObj[tex] = NULL;
		}
		uint32 bytesNum = GetBytesPerPixel(format, format, type, width, height);
		TextureObject *pObj = new TextureObject(bytesNum * width * height, (const char *)pixels);
		pObj->_texAttrib._level				 = level;
		pObj->_texAttrib._internalFormat	 = internalformat;
		pObj->_texAttrib._width				 = width;
		pObj->_texAttrib._height			 = height;
		pObj->_texAttrib._border			 = border;
		pObj->_texAttrib._format			 = format;
		pObj->_texAttrib._type				 = type;	
		m_mapTextureObj[tex]				 = pObj;
	}
	sBufferObjMgr->UnMapPBO();
}

void TextureMgr::TexSubImage2D(GLenum target, 
							   GLint level, 
							   GLint xoffset, GLint yoffset, 
							   GLsizei width, GLsizei height, 
							   GLenum format, GLenum type, 
							   const void* pixels)
{
	pixels = sBufferObjMgr->MapPBOOffset(pixels);
	if (IsBadReadPtr(pixels, 4) != 0)
	{
		sConsoleLog->DebugWrite("TexSubImage2D: pixels[0x%x] IsBadReadPtr", pixels);
		sBufferObjMgr->UnMapPBO();
		return;
	}
	uint32 bytesNum = GetBytesPerPixel(format, format, type, width, height);
	if (bytesNum == 0)
	{
		sConsoleLog->DebugWrite("TexSubImage2D:format[%d] type[%d] not support", format, type);
	}
	if (bytesNum != 0 && level == 0)
	{
		GLuint tex = GetActiveTextureName(target);
		if (m_mapTextureObj.find(tex) == m_mapTextureObj.end())
		{
			sConsoleLog->DebugWrite("TexSubImage2D: tex[%d] is not exist", tex);
			sBufferObjMgr->UnMapPBO();
			return;
		}
		if (m_mapTextureObj[tex] == NULL)
		{
			sConsoleLog->DebugWrite("TexSubImage2D: tex[%d] is NULL", tex);
			sBufferObjMgr->UnMapPBO();
			return;
		}
		
		
		TextureObject *pObj = m_mapTextureObj[tex];
		if (pObj->_texAttrib._format != format)
		{
			sConsoleLog->DebugWrite("TexSubImage2D: format[%d]:[%d] is not match", pObj->_texAttrib._format, format);
			sBufferObjMgr->UnMapPBO();
			return;
		}
		if (xoffset + width > pObj->_texAttrib._width || yoffset + height > pObj->_texAttrib._height)
		{
			sConsoleLog->DebugWrite("TexSubImage2D: weight or height not enough");
			sBufferObjMgr->UnMapPBO();
			return;
		}
		for (int i = 0; i < height; ++i)
		{
			memcpy((void *)(pObj->_pBuffer + ((i + yoffset) * pObj->_texAttrib._width + xoffset) * bytesNum),
				(const char *)pixels + i * bytesNum, width * bytesNum);
		}
	}
	sBufferObjMgr->UnMapPBO();
	
}
extern void MemoryToTGA(TexAttrib attrib, const char* pBuff, const std::string& fileName);

void TextureMgr::DumpTexture(uint32 dumpIdx, const std::string &folder)
{
	std::string textureInfo;
	uint32 idx = 0;
	auto it = m_mapTextureSlot.begin();
	for (; it != m_mapTextureSlot.end(); ++it)
	{
		if (it->second.find(GL_TEXTURE_2D) != it->second.end())
		{
			GLuint texId = it->second[GL_TEXTURE_2D];
			if (m_mapTextureObj.find(texId) != m_mapTextureObj.end())
			{
				TextureObject* pObj = m_mapTextureObj[texId];
				if (pObj == NULL || pObj->_pBuffer == NULL)
				{
					sConsoleLog->DebugWrite("DumpTexture:TextureObject[%d] is null", texId);
					continue;
				}
				char texName[128] = "";
				sprintf(texName, "/AllTex/%d.tga",texId);
				textureInfo += texName;
				textureInfo += "\r\n";
// 				char fileName[128] = "";
// 				sprintf(fileName, "%s/%d_%d_%d", folder.c_str(), dumpIdx, idx++, texId);
// 				MemoryToTGA(pObj->_texAttrib, pObj->_pBuffer, fileName);
				/*sprintf(fileName, "%s/t%d_%d_%d.ogltex", g_dumpFolder.c_str(), GetID(), dumpIdx, idx++);
				FILE* fp = fopen(fileName, "wb+");
				if (fp)
				{
					fwrite(&pObj->_texAttrib, sizeof(pObj->_texAttrib), 1, fp);
					fwrite(pObj->_pBuffer,  pObj->_buffSize, 1, fp);
					fclose(fp);
				}*/
			}
			else
			{
				//sConsoleLog->Write("DumpTexture:TextureObject[%d] had not exist", texId);
				continue;
			}
		}
	}
	char fileName[128] = "";
	sprintf(fileName, "%s/%d.texinfo.txt", folder.c_str(), dumpIdx);
	FILE* fp = fopen(fileName, "wb+");
	if (fp)
	{
		fwrite(textureInfo.c_str(), textureInfo.length(), 1, fp);
		fclose(fp);
	}
}

void TextureMgr::DumpAllTexture(const std::string &folder)
{
	if (_access((folder + "/AllTex").c_str(), 0) == -1)
	{
		_mkdir((folder + "/AllTex").c_str());
		uint32 idx = 0;
		for (auto it = m_mapTextureObj.begin(); it != m_mapTextureObj.end(); ++it)
		{
			TextureObject* pObj = it->second;
			if (pObj == NULL)
			{
				sConsoleLog->DebugWrite("DumpTextureAll:TextureObject[%d] is null", it->first);
				continue;
			}
			char fileName[128] = "";
			sprintf(fileName, "%s/AllTex/%d", folder.c_str(), it->first);
			MemoryToTGA(pObj->_texAttrib, pObj->_pBuffer, fileName);
			/*sprintf(fileName, "%s/AllTex/%d_%d.ogltex", g_dumpFolder.c_str(), GetID(), idx++);
			FILE* fp = fopen(fileName, "wb+");
			if (fp)
			{
				fwrite(&pObj->_texAttrib, sizeof(pObj->_texAttrib), 1, fp);
				fwrite(pObj->_pBuffer,  pObj->_buffSize, 1, fp);
				fclose(fp);
			}*/
		}
	}	
}

void TextureMgr::FramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
	if (attachment == GL_COLOR_ATTACHMENT0 && textarget == 	GL_TEXTURE_2D && level == 0)
	{
		m_frameBufferTex = texture;
	}
}

void TextureMgr::BindFramebuffer(GLenum target, GLuint framebuffer)
{
	if (m_frameBuffer != framebuffer && m_frameBufferTex != 0)
	{
		//read pix
		GLuint bkTex = GetActiveTextureName(GL_TEXTURE_2D);
		BindTexture(GL_TEXTURE_2D, m_frameBufferTex);
		auto it = m_mapTextureObj.find(m_frameBufferTex);
		if (it != m_mapTextureObj.end() && it->second != NULL)
		{
			CopyTexImage2D(GL_TEXTURE_2D, 0, it->second->_texAttrib._internalFormat, 
				0, 0, it->second->_texAttrib._width, it->second->_texAttrib._height, 0);
		}
		BindTexture(GL_TEXTURE_2D, bkTex);
		m_frameBufferTex = 0;
	}
	m_frameBuffer		= framebuffer;
	m_frameBufferTex	= 0;
}