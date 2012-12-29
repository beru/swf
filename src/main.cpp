
#include <stdio.h>
#include <vector>
#include <map>

#include "swd.h"

static inline
size_t getFileSize(FILE* file)
{
	fseek(file, 0, SEEK_END);
	int length = ftell(file);
	fseek(file, 0, SEEK_SET);
	return length;
}

struct FileInfo
{
	uint32_t index;
	uint32_t bitmap;
	const char* name;
	const char* src;
	
	struct Offset
	{
		uint32_t src;
		uint32_t swf;
	};
	std::vector<Offset> offsets;
	std::vector<uint16_t> breakPoints;

};

typedef std::map<uint32_t, FileInfo> FileInfoMap;
typedef FileInfoMap::iterator FileInfoMapIt;
FileInfoMap fileInfoMap;

static inline
FileInfo& getFileInfo(FileInfoMap& map, uint32_t index)
{
	FileInfoMapIt it = fileInfoMap.find(index);
	if (it != fileInfoMap.end()) {
		return it->second;
	}else {
		FileInfo info;
		info.index = index;
		return fileInfoMap[index] = info;
	}
}

int main(int argc, char* argv[])
{
	if (argc < 2) {
		return puts("specify swd filename.");
	}
	FILE* f = fopen(argv[1], "rb");
	size_t sz = getFileSize(f);
	std::vector<uint8_t> buff(sz);
	fread(&buff[0], 1, sz, f);
	fclose(f);
	
	const uint8_t* pStart = &buff[0];
	const uint8_t* pBuff = pStart;
	const SWD::Header* pHeader = (const SWD::Header*) pBuff;
	pBuff += sizeof(SWD::Header);
	const SWD::Tag_ID* pId = (const SWD::Tag_ID*)(pBuff);
	pBuff += pId->getSize();
	const SWD::Tag* pTag = (const SWD::Tag*) (pBuff);
	while (pBuff - pStart < sz) {
		
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
	
	return 0;
}


