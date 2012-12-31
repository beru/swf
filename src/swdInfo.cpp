
#include "swdInfo.h"

#include "swd.h"
#include "util.h"

void SWDInfo::Read(const uint8_t* buff, size_t length)
{
	const uint8_t* pStart = buff;
	const uint8_t* pBuff = pStart;
	const SWD::Header* pHeader = (const SWD::Header*) pBuff;
	pBuff += sizeof(SWD::Header);
	const SWD::Tag_ID* pId = (const SWD::Tag_ID*)(pBuff);
	pBuff += pId->getSize();
	const SWD::Tag* pTag = (const SWD::Tag*) (pBuff);
	while (pBuff - pStart < length) {
		
		switch (pTag->id) {
		case SWD::TagId_Offset:
			{
				const SWD::Tag_Offset* pOffset = (const SWD::Tag_Offset*) pTag;
				SWDInfo::Offset offset;
				offset.file = pOffset->index;
				offset.line = pOffset->lineNumber;
				offset.swf = pOffset->byteOffset;
				offsets.push_back(offset);
			}
			break;
		case SWD::TagId_Script:
			{
				const SWD::Tag_Script* pScript = (const SWD::Tag_Script*) pTag;
				SWDInfo::File file;
				file.index = pScript->index;
				file.bitmap = pScript->bitmap;
				file.name = utf8_to_sjis(pScript->name());
				file.src = pScript->text();
				files.push_back(file);
			}
			break;
		case SWD::TagId_BreakPoint:
			{
				const SWD::Tag_BreakPoint* pBP = (const SWD::Tag_BreakPoint*) pTag;
				breakPoints.push_back(pBP->lineNumber);
			}
			break;
		}
		
		pBuff += pTag->getSize();
		pTag = (const SWD::Tag*) pBuff;
	}
}

