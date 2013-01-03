#pragma once

#include <vector>

class IActionProcessor
{
public:
	virtual
	void Process(
		const uint8_t* const pStart,
		const uint8_t* buff,
		size_t len
		) = 0;
	
};

