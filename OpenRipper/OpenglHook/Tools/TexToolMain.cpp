
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
void MemoryToTGA(TexAttrib attrib, const char* pBuff, const std::string& fileName);
extern bool SaveOGLToTga(const std::string &fileName);
extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }
int _tmain(int argc, _TCHAR* argv[])
{
// 	std::string fileName = argv[1];	
// 	if(SaveOGLToTga(fileName))
// 	{
// 		printf("%s success\n", fileName.c_str());
// 	}
// 	else
// 	{
// 		printf("%s fail\n", fileName.c_str());
// 	}
	TexAttrib attrib;
	attrib._border = 0;
	attrib._format = GL_RGBA;
	attrib._height = 32;
	attrib._internalFormat = GL_RGBA;
	attrib._level = 0;
	attrib._target = 0;
	attrib._type = GL_UNSIGNED_BYTE;
	attrib._width = 32;	
	char *pBuff = new char[1024 * 10];
	MemoryToTGA(attrib, pBuff, "test");
	return 0;
}