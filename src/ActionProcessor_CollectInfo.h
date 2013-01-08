#pragma once

#include "IActionProcessor.h"

struct PositioningInfo
{
	uint16_t fieldOffset;
	bool isSignedField;
	uint16_t fromOffset;
	uint16_t toOffset;
};

class ActionProcessor_CollectInfo : public IActionProcessor
{
public:
	ActionProcessor_CollectInfo()
	{
	}
	
	// override
	virtual
	void Process(
		const uint8_t* const pFileStart,
		const uint8_t* buff,
		size_t len
	);
	
	std::vector<PositioningInfo> positions;
};

