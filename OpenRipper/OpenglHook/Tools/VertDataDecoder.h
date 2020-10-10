#pragma once
#include <string>
#include <vector>
#include "DetoursGlDef.h"
namespace VertDataDecoder
{
	bool VertDecode(const std::string& vertFile, 
		const std::string& indxFile, 
		const std::string& uvFile,
		bool flipUv, bool flipXY,
		std::vector<Vector3d<float>>& v, 
		std::vector<Vector3d<float>>& vn, 
		std::vector<Vector2d<float>>& vt,
		std::vector<unsigned int>& triangles);
};

