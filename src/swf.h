
#pragma once
#pragma pack(push, 1)

namespace SWF {

struct Header
{
	char signatures[3];
	uint8_t version;
	uint32_t fileLength;
};

struct TagType
{
	enum Enum {
		// Control
		PlaceObject = 4,
		PlaceObject2 = 26,
		PlaceObject3 = 70,
		RemoveObject = 5,
		RemoveObject2 = 28,
		ShowFrame = 1,
		SetBackgroundColor = 9,
		FrameLabel = 43,
		Protect = 24,
		End = 0,
		ExportAssets = 56,
		ImportAssets = 57,
		EnableDebugger = 58,
		EnableDebugger2 = 64,
		ScriptLimits = 65,
		SetTabIndex = 66,
		FileAttributes = 69,
		ImportAssets2 = 71,
		SymbolClass = 76,
		Metadata = 77,
		DefineScalingGrid = 78,
		DefineSceneAndFrameLabelData = 86,
		
		// Action
		DoAction = 12,
		DoInitAction = 59,
		DoABC = 82,
		
		// Shape
		DefineShape = 2,
		DefineShape2 = 22,
		DefineShape3 = 32,
		DefineShape4 = 83,
		
		// Bitmap
		DefineBits = 6,
		JPEGTables = 8,
		DefineBitsJPEG2 = 21,
		DefineBitsJPEG3 = 35,
		DefineBitsLossless = 20,
		DefineBitsLossless2 = 36,
		DefineBitsJPEG4 = 90,
		
		// Shape Morphing
		DefineMorphShape = 46,
		DefineMorphShape2 = 84,
		
		// Fonts and Text
		DefineFont = 10,
		DefineFontInfo = 13,
		DefineFontInfo2 = 62,
		DefineFont2 = 48,
		DefineFont3 = 75,
		DefineFontAlignZones = 73,
		DefineFontName = 88,
		DefineText = 11,
		DefineEditText = 37,
		CSMTextSettings = 74,
		DefineFont4 = 91,
		
		// Sound
		DefineSound = 14,
		StartSound = 15,
		StartSound2 = 89,
		SoundStreamHead = 18,
		SoundStreamHead2 = 45,
		SoundStreamBlock = 19,
		
		// Button
		DefineButton = 7,
		DefineButton2 = 34,
		DefineButtonCxform = 23,
		DefineButtonSound = 17,
		
		// Sprite
		DefineSprite = 39,
		
		// Video
		DefineVideoStream = 60,
		VideoFrame = 61,
		
		// Binary Data
		DefineBinaryData = 87,
		
	};
};

namespace ActionCode {
	enum Enum : unsigned char {
		EndFlag = 0x00,
		
		// SWF 3 actions
		GotoFrame = 0x81,
		GetURL = 0x83,
		NextFrame = 0x04,
		PreviousFrame = 0x05,
		Play = 0x06,
		Stop = 0x07,
		ToggleQuality = 0x08,
		StopSounds = 0x09,
		WaitForFrame = 0x8A,
		SetTarget = 0x8B,
		GoToLabel = 0x8C,
		
		// SWF 4 actions
		Push = 0x96,
		Pop = 0x17,
		Add = 0x0A,
		Subtract = 0x0B,
		Multiply = 0x0C,
		Divide = 0x0D,
		Equals = 0x0E,
		Less = 0x0F,
		And = 0x10,
		Or = 0x11,
		Not = 0x12,
		StringEquals = 0x13,
		StringLength = 0x14,
		StringAdd = 0x21,
		StringExtract = 0x15,
		StringLess = 0x29,
		MBStringLength = 0x31,
		MBStringExtract = 0x35,
		ToInteger = 0x18,
		CharToAscii = 0x32,
		AsciiToChar = 0x33,
		MBCharToAscii = 0x36,
		MBAsciiToChar = 0x37,
		Jump = 0x99,
		If = 0x9D,
		Call = 0x9E,
		GetVariable = 0x1C,
		SetVariable = 0x1D,
		GetURL2 = 0x9A,
		GotoFrame2 = 0x9F,
		SetTarget2 = 0x20,
		GetProperty = 0x22,
		SetProperty = 0x23,
		CloneSprite = 0x24,
		RemoveSprite = 0x25,
		StartDrag = 0x27,
		EndDrag = 0x28,
		WaitForFrame2 = 0x8D,
		Trace = 0x26,
		GetTime = 0x34,
		RandomNumber = 0x30,

		// SWF 5 actions
		CallFunction = 0x3D,
		CallMethod = 0x52,
		ConstantPool = 0x88,
		DefineFunction = 0x9B,
		DefineLocal = 0x3C,
		DefineLocal2 = 0x41,
		Delete = 0x3A,
		Delete2 = 0x3B,
		Enumerate = 0x46,
		Equals2 = 0x49,
		GetMember = 0x4E,
		InitArray = 0x42,
		InitObject = 0x43,
		NewMethod = 0x53,
		NewObject = 0x40,
		SetMember = 0x4F,
		TargetPath = 0x45,
		With = 0x94,
		ToNumber = 0x4A,
		ToString = 0x4B,
		TypeOf = 0x44,
		Add2 = 0x47,
		Less2 = 0x48,
		Modulo = 0x3F,
		BitAnd = 0x60,
		BitLShift = 0x63,
		BitOr = 0x61,
		BitRShift = 0x64,
		BitURShift = 0x65,
		BitXor = 0x62,
		Decrement = 0x51,
		Increment = 0x50,
		PushDuplicate = 0x4C,
		Return = 0x3E,
		StackSwap = 0x4D,
		StoreRegister = 0x87,

		// SWF 6 actions
		InstanceOf = 0x54,
		Enumerate2 = 0x55,
		StrictEquals = 0x66,
		Greater = 0x67,
		StringGreater = 0x68,
		
		// SWF 7 actions
		DefineFunction2 = 0x8E,
		Extends = 0x69,
		CastOp = 0x2B,
		ImplementsOp = 0x2C,
		Try = 0x8F,
		Throw = 0x2A,

	};
} // namespace ActionCode

namespace ActionPushType {
	enum Enum : unsigned char {
		String = 0,
		Float = 1,
		Null = 2,
		Undefined = 3,
		Register = 4,
		Boolean = 5,
		Double = 6,
		Integer = 7,
		Constant8 = 8,
		Constant16 = 9,
	};
} // namespace ActionPushType

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
