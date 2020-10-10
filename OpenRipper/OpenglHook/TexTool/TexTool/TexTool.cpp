// TexTool.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "PVRTexture.h"
//#include "png.h"
#include <string>
#include "DetoursGlDef.h"
#include "PVRTextureUtilities.h"
using namespace pvrtexture;
#include <Windows.h>  
#include <math.h>  
#include <iostream>  
using namespace std;  

#define RGB2INT(r,g,b) (r*256*256 + g*256 + b)  

void SaveTGA( const char *filename, DWORD width, DWORD height, const byte *data )  
{  
	FILE *savefile = fopen(filename, "wb");  

	GLubyte header[18]={0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	header[12] = width         & 0xFF;
	header[13] = ( width >> 8)  & 0xFF;
	header[14] = (height)       & 0xFF; 
	header[15] = (height >> 8)  & 0xFF;
	header[16] = 32;

	fwrite(header,sizeof(GLubyte),18, savefile);

	unsigned int image_size= 4*width*height;  
	byte * invert_data = new byte [image_size];  
	memset( invert_data,0,image_size*sizeof( byte ) );  

	for( unsigned int i=0; i<image_size; i++ )  
	{  
		invert_data[ i ]  = data[ i ];  
	}  

	// Swap red and blue,RGB转为BGR  
	for ( unsigned int cswap = 0; cswap < image_size; cswap += 4 )  
	{  
		byte r = invert_data[cswap];  
		invert_data[cswap] = invert_data[cswap + 2];  
		invert_data[cswap + 2] = r;  
	}  

	fwrite( invert_data,image_size*sizeof( byte ),1,savefile );  
	fclose( savefile );  
	delete [] invert_data;  
}  
uint32 GetOGLFormat(uint32 oglformat)
{
	switch (oglformat)
	{
	case GL_RGB:
		return OGL_RGB_888;
	case GL_RGBA:
		return OGL_RGBA_8888;
	default:
		break;
	}
	return OGL_RGBA_8888;
}

void MemoryToTGA(TexAttrib attrib, const char* pBuff, const std::string& fileName)
{
	CPVRTextureHeader cHeader;
	if(attrib._internalFormat == 0)
	{
		attrib._internalFormat = attrib._format;
	}		
	cHeader.setOGLESFormat(attrib._internalFormat, attrib._format, attrib._type);
	cHeader.setWidth(attrib._width);
	cHeader.setHeight(attrib._height);
	CPVRTexture cTexture(cHeader, pBuff);
	//Flip(cTexture, ePVRTAxisY);

	Transcode( cTexture,
		PVRStandard8PixelType,
		ePVRTVarTypeUnsignedByte,
		ePVRTCSpacelRGB
		);
	std::string dstFile = fileName + ".tga";
	SaveTGA(dstFile.c_str(), cTexture.getWidth(), cTexture.getHeight(), (const byte *)cTexture.getDataPtr());
}

bool SaveOGLToTga(const std::string &fileName)
{
	FILE* fp = fopen(fileName.c_str(), "rb");
	const char* pBuff = NULL;
	TexAttrib attrib;
	if (fp)
	{		
		fseek(fp, 0, SEEK_END);
		int len = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		fread(&attrib, sizeof(attrib), 1, fp);
		pBuff = new char[len - sizeof(attrib)];
		fread((void *)pBuff,  len - sizeof(attrib), 1, fp);
		fclose(fp);
	}
	if (pBuff)
	{
		MemoryToTGA(attrib, pBuff, fileName);
		/*CPVRTextureHeader cHeader;
		if(attrib._internalFormat == 0)
		{
			attrib._internalFormat = attrib._format;
		}		
		cHeader.setOGLESFormat(attrib._internalFormat, attrib._format, attrib._type);
		cHeader.setWidth(attrib._width);
		cHeader.setHeight(attrib._height);
		CPVRTexture cTexture(cHeader, pBuff);
		Flip(cTexture, ePVRTAxisY);

		Transcode( cTexture,
			PVRStandard8PixelType,
			ePVRTVarTypeUnsignedByte,
			ePVRTCSpacelRGB
			);
		std::string dstFile = fileName + ".tga";
		SaveTGA(dstFile.c_str(), cTexture.getWidth(), cTexture.getHeight(), (const byte *)cTexture.getDataPtr());*/
		delete[] pBuff;
		return true;
	}
	return false;
}



