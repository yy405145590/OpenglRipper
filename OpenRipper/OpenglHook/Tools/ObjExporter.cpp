#include "stdafx.h"
#include "ObjExporter.h"


ObjExporter::ObjExporter(uint32 idx)
{
	m_data << "#" << "opengl ripper generate" << std::endl;	
	m_idx = idx;
}


ObjExporter::~ObjExporter(void)
{
}

void ObjExporter::SetVert(const std::vector<Vector3d<float>>& v, 
						  const std::vector<Vector3d<float>>& vn, 
						  const std::vector<Vector2d<float>>& vt,
						  const std::vector<unsigned int>& triangles)
{
	for each (auto var in v)
	{
		m_data << "v " << var.X() << " " << var.Y() << " " << var.Z() << std::endl;
	}
// 	for each (auto var in vn)
// 	{
// 		m_data << "vn " << var.X() << " " << var.Y() << " " << var.Z() << std::endl;
// 	}
	for each (auto var in vt)
	{
		m_data << "vt " << var.X() << " " << var.Y() << std::endl;
	}
	m_data << "g " << m_idx << std::endl;
	if (!m_mtlFile.empty())
	{
		m_data << "usemtl " << "Standard" << std::endl;
		m_data << "usemap " << "Standard" << std::endl;
	}
	for(size_t i = 0; i < (triangles.size() / 3) * 3; i += 3)
	{
		m_data	<< "f " << triangles[i]		+ 1		<< "/" << triangles[i]		+ 1 
				<< " "	<< triangles[i+1]	+ 1		<< "/" << triangles[i+1]	+ 1 
				<< " "	<< triangles[i + 2]	+ 1		<< "/" << triangles[i+2]	+ 1
				<< std::endl;
	}
}

void ObjExporter::SetMat(const std::vector<std::string>& texList, const std::string &folder)
{

	if (texList.size() <= 0)
	{
		return;
	}
	std::stringstream ss;
	ss << m_idx << ".mtl";
	m_mtlFile = folder + ss.str();
	m_data << "mtllib " << "./" << ss.str() << " " << std::endl;

	std::stringstream strMtl;
	strMtl << "newmtl Standard" << std::endl;
	strMtl << "Kd  0.6 0.6 0.6" << std::endl;
	strMtl << "Ks  0.9 0.9 0.9" << std::endl;
	strMtl << "Ka  0.6 0.6 0.6" << std::endl;
	strMtl << "d  1.0" << std::endl;
	strMtl << "Ns  0.0" << std::endl;
	strMtl << "illum 2" << std::endl;
	strMtl << "map_Kd " << "./" << texList[0] << std::endl;

	FILE *f = fopen(m_mtlFile.c_str(), "wb+");
	if (f)
	{
		fwrite(strMtl.str().c_str(), strMtl.str().size(), 1, f);
		fclose(f);
	}
}

void ObjExporter::ExportToFile(const std::string &fileName)
{
	FILE *f = fopen(fileName.c_str(), "wb+");
	if (f)
	{
		fwrite(m_data.str().c_str(), m_data.str().size(), 1, f);
		fclose(f);
	}
}