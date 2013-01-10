#pragma once

#include "IActionProcessor.h"

#include "swdInfo.h"

#include "ActionProcessor_CollectInfo.h"

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
	const SWDInfo::Offset* getSWDInfo(const uint8_t* buff);
	void checkPositions(const uint8_t* buff, int addedSize);
	void updatePositions();

	const uint8_t* pFileStart;
	const uint8_t* pBuffStart;
	const SWDInfo& swdInfo;
	std::vector<uint8_t>& dst;
	
	std::vector<PositioningInfo> orgPositions;
	std::vector<PositioningInfo> newPositions;
	size_t dstStartSize;
};


