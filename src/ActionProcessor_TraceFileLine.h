#pragma once

#include "IActionProcessor.h"

#include "swdInfo.h"

class ActionProcessor_TraceFileLine : public IActionProcessor
{
public:
	ActionProcessor_TraceFileLine(
		std::vector<uint8_t>& dst,
		const SWDInfo& swdInfo
		)
		:
		dst(dst),
		swdInfo(swdInfo)
	{
	}
	
	// override
	virtual
	void Process(
		const uint8_t* const pFileStart,
		const uint8_t* buff,
		size_t len
		);
	
private:
	void iterate(const uint8_t* buff, size_t len);

	void append(const uint8_t* buff, size_t nBytes)
	{
		dst.insert(dst.end(), buff, buff+nBytes);
	}
	void append(uint8_t b)
	{
		dst.push_back(b);
	}
	
	void pushString(const char* str);
	uint8_t getLineNo(const uint8_t* buff);

	const uint8_t* pFileStart;
	const SWDInfo& swdInfo;
	std::vector<uint8_t>& dst;
	std::string fileName;
};


