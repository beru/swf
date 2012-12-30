
#include "swf_reader.h"

#include "swf.h"

#define MINIZ_HEADER_FILE_ONLY
#include "miniz.c"

#include <vector>
#include <string.h>

static inline
bool decode(std::vector<uint8_t>& decompressed, const uint8_t* src, size_t length)
{
	const SWF::Header* header = (const SWF::Header*) src;
	decompressed.resize(header->fileLength);
	uint8_t* pBuff = &decompressed[0];
	memcpy(pBuff, src, sizeof(SWF::Header));
	mz_ulong sz = header->fileLength - sizeof(SWF::Header);
	int ret = mz_uncompress(pBuff + sizeof(SWF::Header), &sz, (const unsigned char*)(header+1), length - sizeof(SWF::Header));
	if (ret) {
		puts(mz_error(ret));
		return false;
	}
	pBuff[0] = 'F';
	return true;
}

size_t getRectangleByteLength(const uint8_t* buff)
{
	uint8_t b = buff[0];
	uint8_t nBits = b >> 3;
	size_t nBytes = (5 + 4 * nBits + 7) / 8;
	return nBytes;
}

void parseActions(const uint8_t* buff, size_t len)
{
	const uint8_t* start = buff;
	do {
		uint8_t code = *buff;
		++buff;
		if (code & 0x80) {
			size_t recLen = *(const uint16_t*)buff;
			buff += 2 + recLen;
		}

	}while (buff - start < len);
}

void ReadSWF(const uint8_t* buff, size_t length)
{
	std::vector<uint8_t> decompressed;
	if (strncmp((const char*)buff, "FWS", 3) == 0) {
	}else if (strncmp((const char*)buff, "CWS", 3) == 0) {
		if (!decode(decompressed, buff, length)) {
			return;
		}
		buff = &decompressed[0];
		length = decompressed.size();
	}else {
		return;
	}
	// frame size
	const uint8_t* pStart = buff;
	buff += sizeof(SWF::Header);
	buff += getRectangleByteLength(buff);
	buff += 4;
	do {
		uint16_t recordHeader = *(const uint16_t*) buff;
		uint16_t type = recordHeader >> 6;
		uint16_t len = recordHeader & 0x3F;
		buff += 2;
		if (len == 0x3F) {
			len = *(const int32_t*)buff;
			buff += 4;
		}
		switch (type) {
		case SWF::TagType::DoAction:
			parseActions(buff, len);
			break;
		case SWF::TagType::DoInitAction:
			{
				int hoge = 0;
			}
			break;
		}
		buff += len;
	} while (buff - pStart < length);
	bool hoge = 0;
}

