
#include "swdInfo.h"

#include "swd.h"
#include "util.h"

#include <algorithm>


bool compare(const SWDInfo::Offset& lhs, const SWDInfo::Offset& rhs)
{
return lhs.swf < rhs.swf;
}

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
				file.name = pScript->name();
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

	std::sort(offsets.begin(), offsets.end(), compare);
}

const char* findFileName(const SWDInfo& swdInfo, size_t pos)
{
	const std::vector<SWDInfo::Offset>& offsets = swdInfo.offsets;
	for (size_t i=0; i<offsets.size(); ++i) {
		const SWDInfo::Offset& o = offsets[i];
		if (o.swf >= pos) {
			size_t index = o.file;
			for (size_t j=0; j<swdInfo.files.size(); ++j) {
				const SWDInfo::File& f = swdInfo.files[j];
				if (f.index == index) {
					return f.name;
				}
			}

		}
	}
	return 0;
}

