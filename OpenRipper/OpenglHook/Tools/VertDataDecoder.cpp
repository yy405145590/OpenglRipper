#include "stdafx.h"
#include "VertDataDecoder.h"
#include "DetoursGlDef.h"
#include "MemoryStream.h"
#include <iostream>
#include <sstream>
#include <math.h>
unsigned int GetSizeFromGLType(unsigned int type)
{
	switch (type)
	{
	case GL_BYTE:
		return 1;
	case GL_UNSIGNED_BYTE:
		return 1;
	case GL_HALF_FLOAT_OES:
	case GL_SHORT:
		return 2;
	case GL_UNSIGNED_SHORT:
		return 2;
	case GL_INT:
		return 4;
	case GL_UNSIGNED_INT:
		return 4;
	case GL_FLOAT:
		return 4;
	case GL_FIXED:
		return 4;
	default:
		break;
	}
	return 0;
}
float Float16ToFloat(short fltInt16)
{
	int fltInt32 = ((fltInt16 & 0x8000) << 16);
	fltInt32 |= ((fltInt16 & 0x7fff) << 13) + 0x38000000;
	float fRet;
	memcpy(& fRet, & fltInt32, sizeof(float));
	return fRet;
}
template<typename T>
void ReadFormat(unsigned int type, MStream& buff, T &val)
{
	switch (type)
	{
	case GL_UNSIGNED_BYTE:
	{
		unsigned char d;
		buff >> d;
		val = (T)d;
		break;
	}
	case GL_BYTE:
		{
			signed char d;
			buff >> d;
			val = (T)d;
			break;
		}
	case GL_SHORT:
		{
			short d;
			buff >> d;
			val = d;
			break;
		}
	case GL_UNSIGNED_SHORT:
		{
			unsigned short d;
			buff >> d;
			val = (T)d;
			break;
		}
	case GL_FIXED:
	case GL_INT:
		{
			int d;
			buff >> d;
			val = (T)d;
			break;
		}
	case GL_UNSIGNED_INT:
		{
			unsigned int d;
			buff >> d;
			val = (T)d;
			break;
		}
	case GL_FLOAT:
		{
			float d;
			buff >> d;
			val = (T)d;
			break;
		}
	case GL_HALF_FLOAT_OES:
	{
		unsigned short d;
		buff >> d;
		val = Float16ToFloat(d);
	}
	break;
	default:
		return;
		break;
	}
}
namespace VertDataDecoder
{
	bool VertDecode(const std::string& vertFile, 
		const std::string& indxFile, 
		const std::string& uvFile, 
		bool flipUv, bool flipXY, 
		std::vector<Vector3d<float>>& v, 
		std::vector<Vector3d<float>>& vn, 
		std::vector<Vector2d<float>>& vt, 
		std::vector<unsigned int>& triangles)
	{
		MStream vertStream(vertFile);
		if (!vertStream.IsValid())
		{
			return false;
		}
		VertexAttribParam vertexHeader;
		vertStream >> vertexHeader;
		unsigned int strideSize = vertexHeader._stride == 0
			? GetSizeFromGLType(vertexHeader._type) * vertexHeader._size 
			: vertexHeader._stride;
		vertStream.Skip((unsigned int)vertexHeader.offset);
		unsigned int readCount = vertexHeader._size * GetSizeFromGLType(vertexHeader._type);
		
		while (readCount > 0 && vertStream.CanRead(readCount))
		{
			Vector3d<float> v3;
			int vIdx = 0;
			for (int i = 0; i < vertexHeader._size && vIdx < 3; ++i)
			{
				float d = 0;
				ReadFormat<float>(vertexHeader._type, vertStream, d);
				if (abs(d) < INT_MAX)
				{
					v3.m[vIdx++] = d;
				}
				else
				{
					v3.m[vIdx++] = 0;
				}
			}
			vertStream.Skip(strideSize - readCount);
			v.push_back(v3);
		}

		MStream uvStream(uvFile);
		if (uvStream.IsValid())
		{
			VertexAttribParam uvHeader;
			uvStream >> uvHeader;
			strideSize = uvHeader._stride == 0
				? GetSizeFromGLType(uvHeader._type) * uvHeader._size 
				: uvHeader._stride;
			uvStream.Skip((unsigned int)uvHeader.offset);
			readCount = uvHeader._size * GetSizeFromGLType(uvHeader._type);
			while (readCount > 0 && uvStream.CanRead(readCount))
			{
				Vector2d<float> v2;
				int vIdx = 0;
				for (int i = 0; i < uvHeader._size && vIdx < 2; ++i)
				{
					float d = 0;
					ReadFormat<float>(uvHeader._type, uvStream, d);
					if (uvHeader._normalized == 1 && uvHeader._type == GL_UNSIGNED_BYTE)
					{
						d /= 255.0f;
					}
					if (abs(d) < INT_MAX)
					{
						v2.m[vIdx++] = d;
						if (flipUv)
						{
							if (vIdx == 1)
							{
								v2.m[vIdx] = 1 - v2.m[vIdx];
							}
						}
					}
					else
					{
						v2.m[vIdx++] = 0;
					}
				}
				uvStream.Skip(strideSize - readCount);
				vt.push_back(v2);
			}
		}	

		for (; vt.size() < v.size();)
		{
			vt.push_back(Vector2d<float>(0, 0));
		}

		for (; v.size() < vt.size();)
		{
			v.push_back(Vector3d<float>(0, 0, 0));
		}

		MStream indxStream(indxFile);
		if (indxStream.IsValid())
		{
			IndxAttrib indxHeader;
			indxStream >> indxHeader;
			readCount = GetSizeFromGLType(indxHeader._type);
			while (readCount > 0 && indxStream.CanRead(readCount))
			{
				int d = 0;
				ReadFormat(indxHeader._type, indxStream, d);
				triangles.push_back(d);
			}
		}
		else
		{
			return false;
		}

		if (flipXY)
		{
			for (size_t i = 0; i < triangles.size(); i += 3)
			{
				int v = triangles[i];
				triangles[i] = triangles[i + 1];
				triangles[i + 1] = v;
			}
		}
		return true;
	}
}
