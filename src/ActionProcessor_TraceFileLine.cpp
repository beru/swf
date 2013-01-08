
#include "ActionProcessor_TraceFileLine.h"

#include "swf.h"
#include <algorithm>

struct SWDInfoOffsetLessThan
{
    bool operator() (const SWDInfo::Offset& left, const SWDInfo::Offset& right)
    {
		return left.swf < right.swf;
    }
    bool operator() (const SWDInfo::Offset& left, float right)
    {
        return left.swf < right;
    }
    bool operator() (float left, const SWDInfo::Offset& right)
    {
        return left < right.swf;
    }
};

// override
// virtual
void ActionProcessor_TraceFileLine::Process(
	const uint8_t* const pStart,
	const uint8_t* buff,
	size_t len
	)
{
	this->pStart = pStart;
	fileName = findFileName(swdInfo, buff - pStart);
	fileName += " ";
	iterate(buff, len);
}

void ActionProcessor_TraceFileLine::iterate(const uint8_t* buff, size_t len)
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
				iterate(buff, srcSize);
				buff += srcSize;
				size_t newSize = dst.size() - (prevSize + 2);
				*(uint16_t*)(&dst[prevSize]) = newSize;
			}
			break;
// �f�o�b�O�����Ƀ`�F�b�N��t�����DefineFunction2�͎g���Ȃ�
//		case SWF::ActionCode::DefineFunction2:
//			break;
		case SWF::ActionCode::Trace:
			{
				size_t lineNo = getLineNo(buff);
				char str[512];
				sprintf(str, "%s%d ", fileName.c_str(), lineNo);
				++buff;
				pushString(str);
				append(SWF::ActionCode::StackSwap);
				append(SWF::ActionCode::StringAdd);
				append(SWF::ActionCode::Trace);
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

void ActionProcessor_TraceFileLine::pushString(const char* str)
{
	append(SWF::ActionCode::Push);
	size_t slen = strlen(str);
	uint16_t len = 1 + slen + 1;
	append((const uint8_t*)&len, 2);
	append(0); // string literal
	append((const uint8_t*)str, slen + 1);

}

uint8_t ActionProcessor_TraceFileLine::getLineNo(const uint8_t* buff)
{
	size_t offset = buff - pStart;
	const std::vector<SWDInfo::Offset>& offsets = swdInfo.offsets;
	if (offsets.size() == 0) {
		return 0;
	}
	std::vector<SWDInfo::Offset>::const_iterator it = std::lower_bound(offsets.begin(), offsets.end(), offset, SWDInfoOffsetLessThan());
	if (it == offsets.end() || it == offsets.begin()) {
		return offsets.front().line;
	}else {
		(--it)->line;
	}
}
