
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

extern bool SaveOGLToTga(const std::string &fileName);
int _tmain(int argc, _TCHAR* argv[])
{
	std::string fileName = argv[1];	
	if(SaveOGLToTga(fileName))
	{
		printf("%s success\n", fileName.c_str());
	}
	else
	{
		printf("%s fail\n", fileName.c_str());
	}
	return 0;
}