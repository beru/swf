#pragma once

#include <vector>
#include <wchar.h>

struct Setting
{
	bool addFilePathAndLineNo;
	bool useOnlyFileName;
	bool useFileNameFiltering;
	std::vector<std::string> targetFileNames;
};

bool Load(Setting& s, const char* filePath);
bool Save(const Setting& s, const char* filePath);
