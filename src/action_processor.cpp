
#include "action_processor.h"

#include "swf.h"

void ActionProcessor::Process(const uint8_t* buff, size_t len)
{
	const uint8_t* start = buff;
	while (buff - start < len) {
		uint8_t code = *buff;
		SWF::ActionCode::Enum ecode = (SWF::ActionCode::Enum) code;
		size_t recLen = 0;
		if (code & 0x80) {
			recLen = *(const uint16_t*)(buff+1);
		}
		switch (ecode) {
		case SWF::ActionCode::DefineFunction:
			{
				append(buff, 3);
				buff += 3;
				// FunctionName
				size_t slen = strlen((const char*)buff);
				append(buff, slen+1);
				buff += slen + 1;
				// NumParams
				size_t nParams = *(const uint16_t*)buff;
				append(buff, 2);
				buff += 2;
				for (size_t i=0; i<nParams; ++i) {
					slen = strlen((const char*)buff);
					append(buff, slen+1);
					buff += slen + 1;
				}
				// codeSize
				size_t srcSize = *(const uint16_t*)buff;
				size_t prevSize = dst.size();
				append(buff, 2);
				buff += 2;
				Process(buff, srcSize);
				buff += srcSize;
				size_t newSize = dst.size() - (prevSize + 2);
				*(uint16_t*)(&dst[prevSize]) = newSize;
			}
			break;
		case SWF::ActionCode::DefineFunction2:
			append(buff, 1);
			buff += 3;
			break;
		case SWF::ActionCode::Trace:
			{
				size_t lineNo = getLineNo(buff);
				char str[512];
				sprintf(str, "%s%d ", fileName.c_str(), lineNo);
				++buff;
				pushString(str);
				dst.push_back(SWF::ActionCode::StackSwap);
				dst.push_back(SWF::ActionCode::StringAdd);
				dst.push_back(SWF::ActionCode::Trace);
			}
			break;
		default:
			append(buff, 1);
			++buff;
			if (code & 0x80) {
				size_t recLen = *(const uint16_t*)buff;
				append(buff, 2 + recLen);
				buff += 2 + recLen;
			}
			break;
		}
	}
}

void ActionProcessor::pushString(const char* str)
{
	dst.push_back(SWF::ActionCode::Push);
	size_t slen = strlen(str);
	uint16_t len = 1 + slen + 1;
	append((const uint8_t*)&len, 2);
	dst.push_back(0); // string literal
	append((const uint8_t*)str, slen + 1);

}

uint8_t ActionProcessor::getLineNo(const uint8_t* buff)
{
	size_t offset = buff - pStart;
	const std::vector<SWDInfo::Offset>& offsets = swdInfo.offsets;
	for (size_t i=0; i<offsets.size(); ++i) {
		const SWDInfo::Offset& o = offsets[i];
		if (o.swf > offset) {
			if (i) {
				return offsets[i-1].line;
			}
		}
	}
	return 0;
}
