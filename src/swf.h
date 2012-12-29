
#pragma once
#pragma pack(push, 1)

namespace SWF {

struct Header
{
	char signatures[3];
	uint8_t version;
	uint32_t fileLength;
};

struct RecordHeader
{
	uint16_t type : 10;
	uint16_t length : 6; // if 0x3F, long record
};

enum ActionCode : unsigned char
{
	ActionCode_EndFlag = 0x00,
	
	// SWF 3 actions
	ActionCode_GotoFrame = 0x81,
	ActionCode_PrevFrame = 0x05,
	ActionCode_Play = 0x06,
	ActionCode_Stop = 0x07,
	ActionCode_ToggleQuality = 0x08,
	ActionCode_StopSounds = 0x09,
	ActionCode_WaitFrame = 0x8A,
	ActionCode_SetTarget = 0x8B,
	ActionCode_GoToLabel = 0x8C,
	
	// SWF 4 actions
	ActionCode_Push = 0x96,
	ActionCode_Pop = 0x17,
	ActionCode_Add = 0x0A,
	ActionCode_Subtract = 0x0B,
	ActionCode_Multiply = 0x0C,
	ActionCode_Divide = 0x0D,
	ActionCode_Equals = 0x0E,
	ActionCode_Less = 0x0F,
	ActionCode_And = 0x10,
	ActionCode_Or = 0x11,
	ActionCode_Not = 0x12,
	ActionCode_StringEquals = 0x13,
	ActionCode_StringLength = 0x14,
	ActionCode_StringAdd = 0x21,
	ActionCode_StringExtract = 0x15,
	ActionCode_StringLess = 0x29,
	ActionCode_MBStringLength = 0x31,
	ActionCode_MBStringExtract = 0x35,
	ActionCode_ToInteger = 0x18,
	ActionCode_CharToAscii = 0x32,
	ActionCode_AsciiToChar = 0x33,
	ActionCode_MBCharToAscii = 0x36,
	ActionCode_MBAsciiToChar = 0x37,
	ActionCode_Jump = 0x99,
	ActionCode_If = 0x9D,
	ActionCode_Call = 0x9E,
	ActionCode_GetVariable = 0x1C,
	ActionCode_SetVariable = 0x1D,
	ActionCode_GetURL2 = 0x9A,
	ActionCode_GotoFrame2 = 0x9F,
	ActionCode_SetTarget2 = 0x20,
	ActionCode_GetProperty = 0x22,
	ActionCode_SetProperty = 0x23,
	ActionCode_CloneSprite = 0x24,
	ActionCode_RemoveSprite = 0x25,
	ActionCode_StartDrag = 0x27,
	ActionCode_EndDrag = 0x28,
	ActionCode_WaitForFrame2 = 0x8D,
	ActionCode_Trace = 0x26,
	ActionCode_GetTime = 0x34,
	ActionCode_RandomNumber = 0x30,

	// SWF 5 actions
	ActionCode_CallFunction = 0x3D,
	ActionCode_CallMethod = 0x52,
	ActionCode_ConstantPool = 0x88,
	ActionCode_DefineFunction = 0x9B,
	ActionCode_DefineLocal = 0x3C,
	ActionCode_DefineLocal2 = 0x41,
	ActionCode_Delete = 0x3A,
	ActionCode_Delete2 = 0x3B,
	ActionCode_Enumerate = 0x46,
	ActionCode_Equals2 = 0x49,
	ActionCode_GetMember = 0x4E,
	ActionCode_InitArray = 0x42,
	ActionCode_InitObject = 0x43,
	ActionCode_NewMethod = 0x53,
	ActionCode_NewObject = 0x40,
	ActionCode_SetMember = 0x4F,
	ActionCode_TargetPath = 0x45,
	ActionCode_With = 0x94,
	ActionCode_ToNumber = 0x4A,
	ActionCode_ToString = 0x4B,
	ActionCode_TypeOf = 0x44,
	ActionCode_Add2 = 0x47,
	ActionCode_Less2 = 0x48,
	ActionCode_Modulo = 0x3F,
	ActionCode_BitAnd = 0x60,
	ActionCode_BitLShift = 0x63,
	ActionCode_BitOr = 0x61,
	ActionCode_BitRShift = 0x64,
	ActionCode_BitURShift = 0x65,
	ActionCode_BitXor = 0x62,
	ActionCode_Decrement = 0x51,
	ActionCode_Increment = 0x50,
	ActionCode_PushDuplicate = 0x4C,
	ActionCode_Return = 0x3E,
	ActionCode_StackSwap = 0x4D,
	ActionCode_StoreRegister = 0x87,

	// SWF 6 actions
	ActionCode_InstanceOf = 0x54,
	ActionCode_Enumerate2 = 0x55,
	ActionCode_StrictEquals = 0x66,
	ActionCode_Greater = 0x67,
	ActionCode_StringGreater = 0x68,
	
	// SWF 7 actions
	ActionCode_DefineFunction2 = 0x8E,
	ActionCode_Extends = 0x69,
	ActionCode_CastOp = 0x2B,
	ActionCode_ImplementsOp = 0x2C,
	ActionCode_Try = 0x8F,
	ActionCode_Throw = 0x2A,

};

struct ActionWaitForFrame
{
	uint16_t frame;
	uint8_t skipCount;
};

struct ActionSetTarget
{
	const char* targetName();
};

struct ActionGoToLabel
{
	const char* label();
};

struct ActionPush
{
	uint8_t type;
	const char* string();
	union {
		float floatValue;
		uint8_t registerNumber;
		uint8_t boolean;
		double doubleValue;
		uint32_t integer;
		uint8_t consntant8;
		uint16_t constant16;
	};
};

struct ActionJump
{
	int16_t branchOffset;
};

struct ActionIf
{
	int16_t branchOffset;
};

struct ActionGetURL2
{
	uint8_t sendVarsMethod : 2;
	uint8_t reserved : 4;
	uint8_t loadTargetFlag : 1;
	uint8_t loadVariablesFlag : 1;
};

struct ActionGotoFrame2
{
	uint8_t reserved : 6;
	uint8_t sceneBiasFlag : 1;
	uint8_t playFlag : 1;
	uint16_t sceneBias;
};

struct ActionWaitFrame2
{
	uint8_t skipCount;
};

struct ActionConsntantPool
{
	uint16_t count;
	void constants(const char* strings[]);
};

struct ActionDefineFunction
{
	
};


} // namespace SWF

#pragma pack(pop)
