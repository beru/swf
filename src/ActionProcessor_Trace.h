#pragma once

#include "IActionProcessor.h"
#include "swdInfo.h"
#include "ActionProcessor_CollectInfo.h"

#include <vector>

class ActionProcessor_Trace : public IActionProcessor
{
public:
	ActionProcessor_Trace(
		std::vector<uint8_t>& dst,
		const SWDInfo& swdInfo,
		bool addFilePathAndLineNo,
		bool useOnlyFileName,
		const std::vector<uint32_t>& filteringFileIds
		)
		:
		dst(dst),
		swdInfo(swdInfo),
		firstPass(swdInfo),
		addFilePathAndLineNo(addFilePathAndLineNo),
		useOnlyFileName(useOnlyFileName),
		filteringFileIds(filteringFileIds)
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
	void moveToNextAction(const uint8_t*& buff);
	void append(const uint8_t* buff, size_t nBytes)
	{
		dst.insert(dst.end(), buff, buff+nBytes);
	}
	void append(uint8_t b)
	{
		dst.push_back(b);
	}
	
	void pushString(const char* str);
	void pushConstant(uint16_t idx);

	const SWDInfo::Offset* getSWDInfo(const uint8_t* buff);
	void checkPositions(const uint8_t* buff, int addedSize);
	void updatePositions();

	const uint8_t* pFileStart;
	const uint8_t* pBuffStart;
	const SWDInfo& swdInfo;
	std::vector<uint8_t>& dst;
	
	ActionProcessor_CollectInfo firstPass;
	std::vector<PositioningInfo> newPositions;
	size_t dstStartSize;

	SWDInfo::Offset lastTraceOffset;	// used to check if SWD Offset is broken
	uint16_t constantPoolNewEntryIndex;	// used for index offset
	
	bool addFilePathAndLineNo;
	bool useOnlyFileName;
	const std::vector<uint32_t>& filteringFileIds;

};


