#include "Interpreter.h"

#include "InterpreterImpl.h"

namespace SWF {
namespace ActionScript {

Interpreter::Interpreter()
	:
	pImpl(new Impl())
{
}

Interpreter::~Interpreter()
{
	delete pImpl;
}

void Interpreter::Process(const uint8_t* buff, ActionCode::Enum code, size_t recLen)
{
	pImpl->Process(buff, code, recLen);
}

} // namespace ActionScript
} // namespace SWF
