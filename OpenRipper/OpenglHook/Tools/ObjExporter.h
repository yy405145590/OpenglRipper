#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "DetoursGlDef.h"

class ObjExporter
{
public:
	ObjExporter(uint32 idx);
	~ObjExporter(void);
	void SetVert(const std::vector<Vector3d<float>>& v, 
		const std::vector<Vector3d<float>>& vn, 
		const std::vector<Vector2d<float>>& vt,
		const std::vector<unsigned int>& triangles);
	void SetMat(const std::vector<std::string>& texList, const std::string &folder);
	void ExportToFile(const std::string &fileName);

private:
	std::stringstream m_data;
	std::string		  m_mtlFile;
	uint32		      m_idx;
};

