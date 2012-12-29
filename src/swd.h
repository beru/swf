
#pragma once
#pragma pack(push, 1)

namespace SWD {

struct Header
{
	char sign[3];
	uint8_t version;
};

enum TagId : uint32_t {
	TagId_Script = 0,
	TagId_Offset = 1,
	TagId_BreakPoint = 2,
	TagId_ID = 3,
	TagId_Registers = 5,
};

struct Tag
{
	TagId id;
	size_t getSize() const;
};

struct Tag_Script : Tag
{
	uint32_t index;
	uint32_t bitmap;
	const char* name() const;
	const char* text() const;
	size_t getSize() const;
};

struct Tag_Offset : Tag
{
	uint32_t index;
	uint32_t lineNumber;
	uint32_t byteOffset;
	size_t getSize() const { return 16; }
};

struct Tag_BreakPoint : Tag
{
	uint16_t index;
	uint16_t lineNumber;
	size_t getSize() const { return 8; }
};

struct Tag_ID : Tag
{
	uint8_t debugId[16];
	size_t getSize() const { return 20; }
};

struct Tag_Registers : Tag
{
	uint32_t offset;
	uint8_t count;

	struct Register
	{
		uint8_t number;
		const char* variableName() const;
	};
	void registers(const Register* registerPtrs[]) const;
	
	size_t getSize() const;
};


} // namespace SWD

#pragma pack(pop)
