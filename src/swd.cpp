
#include "swd.h"

#include <string.h>

namespace SWD {

size_t Tag::getSize() const
{
	switch (id) {
	case TagId_BreakPoint:
		return ((const Tag_BreakPoint*)this)->getSize();
	case TagId_Offset:
		return ((const Tag_Offset*)this)->getSize();
	case TagId_Script:
		return ((const Tag_Script*)this)->getSize();
		break;
	}
	return 0;
}

size_t Tag_Script::getSize() const
{
	const char* pText = text();
	size_t txtLen = strlen(pText);
	return (pText + txtLen + 1) - (const char*)this;
}

const char* Tag_Script::name() const
{
	return (const char*)this + 12;
}

const char* Tag_Script::text() const
{
	const char* pName = name();
	size_t nameLen = strlen(pName);
	return pName + nameLen + 1;
}


} // namespace SWD

