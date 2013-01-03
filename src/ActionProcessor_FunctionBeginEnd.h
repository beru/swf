#pragma once

#include "IActionProcessor.h"

class ActionProcessor_FunctionBeginEnd : public IActionProcessor
{
public:
	ActionProcessor_FunctionBeginEnd(
		const char* beginFuncName,
		const char* endFuncName,
		std::vector<uint8_t>& dst
		)
		:
		beginFuncName(beginFuncName),
		endFuncName(endFuncName),
		dst(dst),
		functionName(0)
	{
	}
	
	// override
	virtual
	void Process(
		const uint8_t* const pStart,
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
	void pushNumber(uint32_t v);
	void pushConstant(uint8_t idx);

	void callFunction(const char* callFuncName);

	const uint8_t* pStart;
	std::vector<uint8_t>& dst;

	const char* beginFuncName;
	const char* endFuncName;
	const char* functionName;
};

