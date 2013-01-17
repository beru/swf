
#include "ActionProcessor_Trace.h"

#include "swf.h"
#include <algorithm>


// override
// virtual
void ActionProcessor_Trace::Process(
	const uint8_t* const pFileStart,
	const uint8_t* buff,
	size_t len
	)
{
	firstPass.Process(pFileStart, buff, len);
	newPositions = firstPass.positions;
	
	this->pFileStart = pFileStart;
	this->pBuffStart = buff;
	this->dstStartSize = dst.size();
	this->lastTraceOffset.file = 0;
	this->lastTraceOffset.line = 0;
	iterate(buff, len);
	
	updatePositions();
}

void ActionProcessor_Trace::moveToNextAction(const uint8_t*& buff)
{
	uint8_t code = *buff;
	if (code & 0x80) {
		size_t recLen = *(const uint16_t*)(buff+1);
		append(buff, 3 + recLen);
		buff += 3 + recLen;
	}else {
		append(buff, 1);
		++buff;
	}
}

void ActionProcessor_Trace::iterate(const uint8_t* buff, size_t len)
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
		case SWF::ActionCode::ConstantPool:
			if (addFilePathAndLineNo) {
				append(ecode); ++buff;
				size_t recLenPos = dst.size();
				append(buff, 2); buff += 2;
				constantPoolNewEntryIndex = *(const uint16_t*)buff;
				size_t countPos = dst.size();
				append(buff, recLen); buff += recLen;
				size_t prevSize = dst.size();
				const std::vector<uint32_t>& traceFileIds = firstPass.traceFileIds;
				for (size_t i=0; i<traceFileIds.size(); ++i) {
					uint32_t id = traceFileIds[i];
					const std::map<uint32_t, SWDInfo::File>::const_iterator it = swdInfo.files.find(id);
					if (it == swdInfo.files.end()) {
						puts("SWD Script entry not found.\n");
						append(0);
					}else {
						append((const uint8_t*) it->second.name, strlen(it->second.name)+1);
					}
				}
				size_t plusSize = dst.size() - prevSize;
				*(uint16_t*)&dst[recLenPos] += plusSize;
				*(uint16_t*)&dst[countPos] = constantPoolNewEntryIndex + traceFileIds.size();
				checkPositions(buff, plusSize);
			}else {
				moveToNextAction(buff);
			}
			break;
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
			if (filteringFileIds.size() == 0 && !addFilePathAndLineNo) {
				append(SWF::ActionCode::Trace);
				++buff;
			}else {
				const SWDInfo::Offset* pOffset = swdInfo.FindOffset(buff - pFileStart);
				if (pOffset->file == lastTraceOffset.file && pOffset->line == lastTraceOffset.line) {
					puts("SWD file may contain wrong info. Multiple Trace calls hit the same offset data in SWD file. \n");
				}
				lastTraceOffset = *pOffset;
				if (filteringFileIds.size()) {
					std::vector<uint32_t>::const_iterator it = std::lower_bound(filteringFileIds.begin(), filteringFileIds.end(), pOffset->file);
					if (it == filteringFileIds.end() || *it != pOffset->file) {
						append(SWF::ActionCode::Pop);
						++buff;
						break;
					}
				}
				if (addFilePathAndLineNo) {
					const std::vector<uint32_t>& traceFileIds = firstPass.traceFileIds;
					std::vector<uint32_t>::const_iterator it = std::find(traceFileIds.begin(), traceFileIds.end(), pOffset->file);
					if (it != traceFileIds.end()) {
						size_t before = dst.size();
						pushConstant(constantPoolNewEntryIndex + (it - traceFileIds.begin()));
						char str[32];
						sprintf(str, " %d ", pOffset->line);
						pushString(str);
						append(SWF::ActionCode::StringAdd);
						append(SWF::ActionCode::StackSwap);
						append(SWF::ActionCode::StringAdd);
						size_t after = dst.size();
						checkPositions(buff, after - before);
					}
				}
				append(SWF::ActionCode::Trace);
				++buff;
			}
			break;
		default:
			moveToNextAction(buff);
			break;
		}
	}
}

void ActionProcessor_Trace::pushString(const char* str)
{
	append(SWF::ActionCode::Push);
	size_t slen = strlen(str);
	uint16_t len = 1 + slen + 1;
	append((const uint8_t*)&len, 2);
	append(0); // string literal
	append((const uint8_t*)str, slen + 1);

}

void ActionProcessor_Trace::pushConstant(uint16_t idx)
{
	append(SWF::ActionCode::Push);
	uint16_t len = 2;
	append((const uint8_t*)&len, 2);
	if (idx <= 255) {
		append(8); // Constant8
		append(idx);
	}else {
		append(9); // Constant16
		append((const uint8_t*)&idx, 2);
	}
}

void ActionProcessor_Trace::checkPositions(const uint8_t* buff, int addedSize)
{
	uint16_t pos = buff - pBuffStart;
	const std::vector<PositioningInfo>& orgPositions = firstPass.positions;
	const size_t orgPosCnt = orgPositions.size();
	for (size_t i=0; i<orgPosCnt; ++i) {
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

void ActionProcessor_Trace::updatePositions()
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

