
#include <stdio.h>

#include "swdInfo.h"
#include "swf_processor.h"

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
	if (argc < 4) {
		return puts("usage: swd src_swf dst_swf");
	}
	
	SWDInfo swdInfo;
	
	std::vector<uint8_t> swdBuff;
	{
		FILE* f = fopen(argv[1], "rb");
		if (f) {
			size_t sz = getFileSize(f);
			swdBuff.resize(sz);
			fread(&swdBuff[0], 1, sz, f);
			fclose(f);
			swdInfo.Read(&swdBuff[0], sz);
		}
	}
	
	std::vector<uint8_t> srcSwfBuff;
	std::vector<uint8_t> dstSwfBuff;
	{
		FILE* f = fopen(argv[2], "rb");
		size_t sz = getFileSize(f);
		srcSwfBuff.resize(sz);
		fread(&srcSwfBuff[0], 1, sz, f);
		fclose(f);
		ProcessSWF(swdInfo, &srcSwfBuff[0], sz, dstSwfBuff);

		f = fopen(argv[3], "wb");
		fwrite(&dstSwfBuff[0], 1, dstSwfBuff.size(), f);
		fclose(f);
	}
	
	return 0;
}


