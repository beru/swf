
#include <stdio.h>

#include "swdInfo.h"
#include "swf_processor.h"
#include "ActionProcessor_Trace.h"
#include "ActionProcessor_FunctionBeginEnd.h"
#include "setting.h"

#include <algorithm>

static inline
size_t getFileSize(FILE* file)
{
	fseek(file, 0, SEEK_END);
	int length = ftell(file);
	fseek(file, 0, SEEK_SET);
	return length;
}

static
void usage()
{
	puts("usage: src_swf dst_swf");
}

static
void findFileIds(const std::vector<std::string>& fileNames, const SWDInfo& swdInfo, std::vector<uint32_t>& fileIds)
{
	const std::map<uint32_t, SWDInfo::File>& files = swdInfo.files;
	for (std::map<uint32_t, SWDInfo::File>::const_iterator it = files.begin(); it!=files.end(); ++it) {
		for (size_t i=0; i<fileNames.size(); ++i) {
			if (strstr(it->second.name, fileNames[i].c_str())) {
				fileIds.push_back(it->second.index);
				break;
			}
		}
	}
	std::sort(fileIds.begin(), fileIds.end());
	std::vector<uint32_t>::iterator lastIt = std::unique(fileIds.begin(), fileIds.end());
	fileIds.resize(lastIt - fileIds.begin());
}

int main(int argc, char* argv[])
{
	if (argc < 3) {
		usage();
		return 0;
	}
	
	const char* srcFileName = argv[1];
	const char* dstFileName = argv[2];

	if (strcmp(srcFileName, dstFileName) == 0) {
		puts("do not specify same file name.\n");
		usage();
		return 0;
	}

	SWDInfo swdInfo;
	
	std::vector<uint8_t> swdBuff;
	{
		const char* pDot = strrchr(srcFileName, '.');
		if (pDot) {
			char swdFileName[_MAX_PATH];
			size_t len = (pDot+1)-srcFileName;
			memcpy(swdFileName, srcFileName, len);
			memcpy(swdFileName+len, "swd", 4);
			FILE* f = fopen(swdFileName, "rb");
			if (f) {
				size_t sz = getFileSize(f);
				swdBuff.resize(sz);
				fread(&swdBuff[0], 1, sz, f);
				fclose(f);
				swdInfo.Read(&swdBuff[0], sz);
			}else {
				puts("swd file not found.\n");
				return 0;
			}
		}
	}
	
	std::vector<uint8_t> srcSwfBuff;
	std::vector<uint8_t> dstSwfBuff;
	{
		FILE* f = fopen(srcFileName, "rb");
		size_t sz = getFileSize(f);
		srcSwfBuff.resize(sz);
		fread(&srcSwfBuff[0], 1, sz, f);
		fclose(f);

		std::vector<uint32_t> fileIds;
		Setting setting;
		Load(setting, "setting.xml");
		if (setting.useFileNameFiltering) {
			findFileIds(setting.targetFileNames, swdInfo, fileIds);
		}
		ActionProcessor_Trace ap(dstSwfBuff, swdInfo, setting.addFilePathAndLineNo, setting.useOnlyFileName, fileIds);
//		ActionProcessor_FunctionBeginEnd ap("onFuncBegin", "onFuncEnd", dstSwfBuff);
		ProcessSWF(ap, &srcSwfBuff[0], sz, dstSwfBuff);

		f = fopen(dstFileName, "wb");
		fwrite(&dstSwfBuff[0], 1, dstSwfBuff.size(), f);
		fclose(f);
	}
	
	return 0;
}


