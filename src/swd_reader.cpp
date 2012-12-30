
#include "swd_reader.h"

#include "swd.h"

static inline
FileInfo& getFileInfo(FileInfoMap& map, uint32_t index)
{
	FileInfoMapIt it = map.find(index);
	if (it != map.end()) {
		return it->second;
	}else {
		FileInfo info;
		info.index = index;
		return map[index] = info;
	}
}

void ReadSWD(const uint8_t* buff, size_t length, FileInfoMap& fileInfoMap)
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
				FileInfo& info = getFileInfo(fileInfoMap, pOffset->index);
				FileInfo::Offset offset;
				offset.src = pOffset->lineNumber;
				offset.swf = pOffset->byteOffset;
				info.offsets.push_back(offset);
			}
			break;
		case SWD::TagId_Script:
			{
				const SWD::Tag_Script* pScript = (const SWD::Tag_Script*) pTag;
				FileInfo& info = getFileInfo(fileInfoMap, pScript->index);
				info.bitmap = pScript->bitmap;
				info.name = pScript->name();
				info.src = pScript->text();
			}
			break;
		case SWD::TagId_BreakPoint:
			{
				const SWD::Tag_BreakPoint* pBP = (const SWD::Tag_BreakPoint*) pTag;
				FileInfo& info = getFileInfo(fileInfoMap, pBP->index);
				info.breakPoints.push_back(pBP->lineNumber);
			}
			break;
		}

		pBuff += pTag->getSize();
		pTag = (const SWD::Tag*) pBuff;
	}
	
}

