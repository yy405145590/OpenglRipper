#pragma once
#include <string>
extern void OglExporter(const std::string &folder,
	const std::string &vFormat,
	const std::string &texFormat,
	unsigned int startIdx, unsigned int endIdx,
	bool flipXy, bool flipUv);