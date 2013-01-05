#include "ASVM.h"

#include "swf.h"
#include "FastDelegate.h"

class ASVM::Impl
{
public:
	Impl()
	{
	}

	void Process(const uint8_t* buff, SWF::ActionCode::Enum code, size_t recLen)
	{
		const ProcessPtr p = processPtrs[code];
		if (p) {
			(this->*p)(buff, code, recLen);
		}
	}
	
private:
	typedef void (Impl::*ProcessPtr)(const uint8_t* buff, SWF::ActionCode::Enum code, size_t recLen);
	ProcessPtr processPtrs[256];


};


ASVM::ASVM()
	:
	pImpl(new Impl())
{
}

ASVM::~ASVM()
{
	delete pImpl;
}

