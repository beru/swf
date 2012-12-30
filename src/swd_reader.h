
#pragma once

#include <vector>
#include <map>

struct FileInfo
{
	uint32_t index;
	uint32_t bitmap;
	const char* name;
	const char* src;
	
	struct Offset
	{
		uint32_t src;
		uint32_t swf;
	};
	std::vector<Offset> offsets;
	std::vector<uint16_t> breakPoints;

};

typedef std::map<uint32_t, FileInfo> FileInfoMap;
typedef FileInfoMap::iterator FileInfoMapIt;

void ReadSWD(const uint8_t* buff, size_t length, FileInfoMap& fileInfoMap);

