
#include <stdio.h>

#include "swdInfo.h"
#include "swf_processor.h"
#include "ActionProcessor_TraceFileLine.h"
#include "ActionProcessor_FunctionBeginEnd.h"

static inline
size_t getFileSize(FILE* file)
{
	fseek(file, 0, SEEK_END);
	int length = ftell(file);
	fseek(file, 0, SEEK_SET);
	return length;
}

int main(int argc, char* argv[])
{
	if (argc < 3) {
		return puts("usage: src_swf dst_swf");
	}
	
	const char* srcFileName = argv[1];
	const char* dstFileName = argv[2];
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
		
		ActionProcessor_TraceFileLine ap(dstSwfBuff, swdInfo);
//		ActionProcessor_FunctionBeginEnd ap("onFuncBegin", "onFuncEnd", dstSwfBuff);
		ProcessSWF(ap, &srcSwfBuff[0], sz, dstSwfBuff);

		f = fopen(dstFileName, "wb");
		fwrite(&dstSwfBuff[0], 1, dstSwfBuff.size(), f);
		fclose(f);
	}
	
	return 0;
}


