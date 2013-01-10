
#pragma once

#include <vector>
#include <map>

struct SWDInfo
{
	struct File
	{
		uint32_t index;
		uint32_t bitmap;
		const char* name;
		const char* src;
	};
	
	struct Offset
	{
		uint32_t file;
		uint32_t line;
		uint32_t swf;
	};
	
	std::map<uint32_t, File> files;
	std::vector<Offset> offsets;
	std::vector<uint16_t> breakPoints;
	
	void Read(const uint8_t* buff, size_t length);
};

