
#include <stdio.h>

#include "swdInfo.h"
#include "swf_reader.h"

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
		return puts("specify swd swf filenames.");
	}
	
	SWDInfo swdInfo;
	
	std::vector<uint8_t> swdBuff;
	{
		FILE* f = fopen(argv[1], "rb");
		size_t sz = getFileSize(f);
		swdBuff.resize(sz);
		fread(&swdBuff[0], 1, sz, f);
		fclose(f);
		swdInfo.Read(&swdBuff[0], sz);
	}
	
	std::vector<uint8_t> swfBuff;
	{
		FILE* f = fopen(argv[2], "rb");
		size_t sz = getFileSize(f);
		swfBuff.resize(sz);
		fread(&swfBuff[0], 1, sz, f);
		fclose(f);
		ReadSWF(&swfBuff[0], sz);
	}
	
	return 0;
}


