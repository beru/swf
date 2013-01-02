#pragma once

#include <vector>

#include "swdInfo.h"

class ActionProcessor
{
public:
	ActionProcessor(const uint8_t* pStart, const SWDInfo& swdInfo, std::vector<uint8_t>& dst, const char* fileName)
		:
		pStart(pStart),
		swdInfo(swdInfo),
		dst(dst)
	{
		this->fileName = fileName;
		this->fileName += " ";
	}
	
	void Process(const uint8_t* buff, size_t len);
	
private:
	void append(const uint8_t* buff, size_t nBytes)
	{
		dst.insert(dst.end(), buff, buff+nBytes);
	}

	void pushString(const char* str);
	uint8_t getLineNo(const uint8_t* buff);

	const uint8_t* pStart;
	const SWDInfo& swdInfo;
	std::vector<uint8_t>& dst;
	std::string fileName;
};

