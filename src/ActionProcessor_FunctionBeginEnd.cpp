
#include "ActionProcessor_FunctionBeginEnd.h"

#include "swf.h"

#include "ActionScript/Interpreter.h"

SWF::Interpreter interpreter;

// override
// virtual
void ActionProcessor_FunctionBeginEnd::Process(
	const uint8_t* const pStart,
	const uint8_t* buff,
	size_t len
	)
{
	this->pStart = pStart;
	iterate(buff, len);
}

void ActionProcessor_FunctionBeginEnd::iterate(const uint8_t* buff, size_t len)
{
	const uint8_t* start = buff;
	while (buff - start < len) {
		uint8_t code = *buff;
		SWF::ActionCode::Enum ecode = (SWF::ActionCode::Enum) code;
		size_t recLen = 0;
		if (code & 0x80) {
			recLen = *(const uint16_t*)(buff+1);
		}
		interpreter.Process(buff, ecode, recLen);
		switch (ecode) {
		case SWF::ActionCode::DefineFunction:
		case SWF::ActionCode::DefineFunction2:
			{
				append(ecode);
				++buff;
				// HeaderLength
				uint16_t headerLength = *(const uint16_t*)buff;
				append(buff, 2);
				buff += 2;
				// FunctionName
				functionName = (const char*)buff;
				append(buff, headerLength);
				buff += headerLength;
				// codeSize
				size_t srcSize = *(const uint16_t*)(buff-2);
				size_t prevSize = dst.size();
				callFunction(beginFuncName);
				iterate(buff, srcSize);
				callFunction(endFuncName);
				buff += srcSize;
				size_t newSize = dst.size() - prevSize;
				*(uint16_t*)(&dst[prevSize-2]) = newSize;
			}
			break;
		case SWF::ActionCode::Return:
			callFunction(endFuncName);
			append(buff, 1);
			++buff;
			break;
		default:
			append(buff, 1);
			++buff;
			if (code & 0x80) {
				size_t recLen = *(const uint16_t*)buff;
				append(buff, 2 + recLen);
				buff += 2 + recLen;
			}
			break;
		}
	}
}

void ActionProcessor_FunctionBeginEnd::pushString(const char* str)
{
	append(SWF::ActionCode::Push);
	size_t slen = strlen(str);
	uint16_t len = 1 + slen + 1;
	append((const uint8_t*)&len, 2);
	append(0); // string literal
	append((const uint8_t*)str, slen + 1);
}

void ActionProcessor_FunctionBeginEnd::pushNumber(uint32_t v)
{
	append(SWF::ActionCode::Push);
	uint16_t len = 5;
	append((const uint8_t*)&len, 2);
	append(7); // integer
	append((const uint8_t*)&v, 4);
}

void ActionProcessor_FunctionBeginEnd::pushConstant(uint8_t idx)
{
	append(SWF::ActionCode::Push);
	uint16_t len = 2;
	append((const uint8_t*)&len, 2);
	append(8); // integer
	append(idx);
}

void ActionProcessor_FunctionBeginEnd::callFunction(const char* callFuncName)
{
	if (!functionName || strcmp(functionName, beginFuncName) == 0 || strcmp(functionName, endFuncName) == 0) {
		return;
	}
	pushString(functionName);
	pushNumber(1);
	pushString(callFuncName);
	append(SWF::ActionCode::CallFunction);
	append(SWF::ActionCode::Pop);
}
