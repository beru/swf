#pragma once

#include "swf.h"

namespace SWF {

class Interpreter
{
public:
	Interpreter();
	~Interpreter();
	
	void Process(const uint8_t* buff, ActionCode::Enum code, size_t recLen);
private:
	class Impl;
	Impl* pImpl;
};

} // namespace SWF
