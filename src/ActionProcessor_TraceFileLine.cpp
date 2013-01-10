
#include "ActionProcessor_TraceFileLine.h"

#include "swf.h"

#include "ActionProcessor_CollectInfo.h"

// override
// virtual
void ActionProcessor_TraceFileLine::Process(
	const uint8_t* const pFileStart,
	const uint8_t* buff,
	size_t len
	)
{
	ActionProcessor_CollectInfo firstPass;
	firstPass.Process(pFileStart, buff, len);
	newPositions = orgPositions = firstPass.positions;
	
	this->pFileStart = pFileStart;
	this->pBuffStart = buff;
	this->dstStartSize = dst.size();
	fileName = findFileName(swdInfo, buff - pFileStart);
	fileName += " ";
	iterate(buff, len);
	
	updatePositions();
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
// デバッグを許可にチェックを付けるとDefineFunction2は使われない
//		case SWF::ActionCode::DefineFunction2:
//			break;
		case SWF::ActionCode::Trace:
			{
				size_t lineNo = getLineNo(buff);
				char str[512];
				sprintf(str, "%s%d ", fileName.c_str(), lineNo);
				size_t before = dst.size();
				{
					pushString(str);
					append(SWF::ActionCode::StackSwap);
					append(SWF::ActionCode::StringAdd);
				}
				size_t after = dst.size();
				checkPositions(buff, after - before);
				{
					append(SWF::ActionCode::Trace);
				}
				++buff;
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
	size_t offset = buff - pFileStart;
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

void ActionProcessor_TraceFileLine::checkPositions(const uint8_t* buff, int addedSize)
{
	uint16_t pos = buff - pBuffStart;
	for (size_t i=0; i<orgPositions.size(); ++i) {
		const PositioningInfo& orgPos = orgPositions[i];
		PositioningInfo& newPos = newPositions[i];
		if (orgPos.fieldOffset >= pos) {
			newPos.fieldOffset += addedSize;
		}
		if (orgPos.fromOffset >= pos) {
			newPos.fromOffset += addedSize;
		}
		if (orgPos.toOffset >= pos) {
			newPos.toOffset += addedSize;
		}
	}
}

void ActionProcessor_TraceFileLine::updatePositions()
{
	uint8_t* pDstBuffStart = &(this->dst[this->dstStartSize]);
	for (size_t i=0; i<newPositions.size(); ++i) {
		const PositioningInfo& newPos = newPositions[i];
		int distance = newPos.toOffset - newPos.fromOffset;
		if (newPos.isSignedField) {
			int16_t* pDst = (int16_t*)(pDstBuffStart + newPos.fieldOffset);
			if (newPos.fromOffset < newPos.fieldOffset) {
				// minus
				distance = -distance;
			}
			*pDst = distance;
		}else {
			uint16_t* pDst = (uint16_t*)(pDstBuffStart + newPos.fieldOffset);
			*pDst = distance;
		}
	}
}

