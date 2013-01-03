
#include "swf_processor.h"

#include "swf.h"

#define MINIZ_HEADER_FILE_ONLY
#include "miniz.c"

#include <vector>
#include <algorithm>
#include <string.h>

#include "swdInfo.h"

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

static inline
bool encode(std::vector<uint8_t>& buff)
{
	std::vector<uint8_t> src = buff;
	const SWF::Header* header = (const SWF::Header*) &src[0];
	mz_ulong destLen = buff.size() - sizeof(SWF::Header);
	int ret = mz_compress(&buff[0] + sizeof(SWF::Header), &destLen, &src[0] + sizeof(SWF::Header), destLen);
	if (ret) {
		return false;
	}
	buff[0] = 'C';
	buff.resize(sizeof(SWF::Header) + destLen);
	return true;
}

size_t getRectangleByteLength(const uint8_t* buff)
{
	uint8_t b = buff[0];
	uint8_t nBits = b >> 3;
	size_t nBytes = (5 + 4 * nBits + 7) / 8;
	return nBytes;
}

static inline
void append(std::vector<uint8_t>& dst, const uint8_t* buff, size_t nBytes)
{
	dst.insert(dst.end(), buff, buff+nBytes);
}

void ProcessSWF(IActionProcessor& actionProcessor, const uint8_t* src, size_t length, std::vector<uint8_t>& dst)
{
	dst.clear();

	// decompress zlib
	std::vector<uint8_t> decompressed;
	const uint8_t* buff = src;
	bool bCompressed = false;
	if (strncmp((const char*)buff, "FWS", 3) == 0) {
	}else if (strncmp((const char*)buff, "CWS", 3) == 0) {
		if (!decode(decompressed, buff, length)) {
			return;
		}
		buff = &decompressed[0];
		length = decompressed.size();
		bCompressed = true;
	}else {
		return;
	}

	const uint8_t* const pStart = buff;

	// header
	append(dst, buff, sizeof(SWF::Header));
	buff += sizeof(SWF::Header);
	
	// frame size
	size_t rectLen = getRectangleByteLength(buff);
	append(dst, buff, rectLen+4);
	buff += rectLen + 4;

	do {
		// header
		uint16_t recordHeader = *(const uint16_t*) buff;
		uint16_t type = recordHeader >> 6;	// 10 bits
		uint16_t len = recordHeader & 0x3F;	// 6 bits
		append(dst, buff, 2);
		buff += 2;
		// record length
		size_t tagLenPos = 0;
		if (len == 0x3F) {
			len = *(const int32_t*)buff;
			tagLenPos = dst.size();
			append(dst, buff, 4);
			buff += 4;
		}
		switch (type) {
		case SWF::TagType::DoAction:
			{
				actionProcessor.Process(pStart, buff, len);
				buff += len;
			}
			break;
		case SWF::TagType::DoInitAction:
			{
				uint16_t spriteID = *(const uint16_t*) buff;
				append(dst, buff, 2);
				actionProcessor.Process(pStart, buff+2, len-2);
				buff += len;
			}
			break;
		default:
			append(dst, buff, len);
			buff += len;
			break;
		}
		if (tagLenPos) {
			*(uint32_t*)&dst[tagLenPos] = dst.size() - (tagLenPos+4);
		}
	} while (buff - pStart < length);

	SWF::Header* pHeader = (SWF::Header*) &dst[0];
	pHeader->fileLength = dst.size();

	if (bCompressed) {
		encode(dst);
	}
}

