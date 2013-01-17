
#include "ActionProcessor_CollectInfo.h"

#include "swf.h"

#include <algorithm>

/*

ActionJump
	BranchOffset	SI16
	
ActionIf
	BranchOffset	SI16
	
ActionDefineFunction
	codeSize		UI16
	
ActionDefineFunction2
	codeSize		UI16

ActionWith
	Size			UI16

ActionTry
	TrySize			UI16
	CatchSize		UI16
	FinallySize		UI16
	
*/

// virtual
void ActionProcessor_CollectInfo::Process(
	const uint8_t* const pFileStart,
	const uint8_t* buff,
	size_t len
	)
{
	positions.clear();
	traceFileIds.clear();
	const uint8_t* buffStart = buff;
	PositioningInfo info;
	while (buff - buffStart < len) {
		uint8_t code = *buff;
		SWF::ActionCode::Enum ecode = (SWF::ActionCode::Enum) code;
		size_t recLen = 0;
		if (code & 0x80) {
			recLen = *(const uint16_t*)(buff+1);
		}
		const uint8_t* buffNext = buff + 3 + recLen;
		switch (ecode) {
		case SWF::ActionCode::WaitForFrame:
		case SWF::ActionCode::WaitForFrame2:
			puts("WaitForFrame actions are not supported yet.\n");
			break;
		case SWF::ActionCode::Jump:
		case SWF::ActionCode::If:
			{
				int16_t branchOffset = *(const int16_t*)(buffNext-2);
				info.fieldOffset = (buffNext-2) - buffStart;
				info.isSignedField = true;
				uint16_t base = buffNext - buffStart;
				if (branchOffset >= 0) {
					info.fromOffset = base;
					info.toOffset = base + branchOffset;
				}else {
					info.fromOffset = base + branchOffset;
					info.toOffset = base;
				}
				positions.push_back(info);
			}
			break;
		case SWF::ActionCode::DefineFunction:
		case SWF::ActionCode::DefineFunction2:
		case SWF::ActionCode::With:
			{
				size_t codeSize = *(const uint16_t*)(buffNext-2);
				info.fieldOffset = (buffNext-2) - buffStart;
				info.isSignedField = false;
				info.fromOffset = buffNext - buffStart;
				info.toOffset = info.fromOffset + codeSize;
				positions.push_back(info);
			}
			break;
		case SWF::ActionCode::Try:
			{
				uint16_t trySize = *(const uint16_t*)(buff + 4);
				uint16_t catchSize = *(const uint16_t*)(buff + 6);
				uint16_t finallySize = *(const uint16_t*)(buff + 8);
				if (trySize) {
					info.fieldOffset = (buff+4) - buffStart;
					info.isSignedField = false;
					info.fromOffset = buffNext - buffStart;
					info.toOffset = info.fromOffset + trySize;
					positions.push_back(info);
				}
				if (catchSize) {
					info.fieldOffset = (buff+6) - buffStart;
					info.isSignedField = false;
					info.fromOffset = (buffNext + trySize) - buffStart;
					info.toOffset = info.fromOffset + catchSize;
					positions.push_back(info);
				}
				if (finallySize) {
					info.fieldOffset = (buff+6) - buffStart;
					info.isSignedField = false;
					info.fromOffset = (buffNext + trySize + catchSize) - buffStart;
					info.toOffset = info.fromOffset + finallySize;
					positions.push_back(info);
				}
			}
			break;
		case SWF::ActionCode::Trace:
			{
				const SWDInfo::Offset* pOffset = swdInfo.FindOffset(buff - pFileStart);
				traceFileIds.push_back(pOffset->file);
			}
			break;
		default:
			break;
		}
		++buff;
		if (code & 0x80) {
			buff += 2 + recLen;
		}
	}
	std::sort(traceFileIds.begin(), traceFileIds.end());
	std::vector<uint32_t>::iterator lastIt = std::unique(traceFileIds.begin(), traceFileIds.end());
	traceFileIds.resize(lastIt - traceFileIds.begin());
}

