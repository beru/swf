#pragma once

#include <vector>
#include <string>
#include <math.h>

#include "Interpreter.h"
#include "util.h"
#include "Object.h"

namespace SWF {
namespace ActionScript {

class Interpreter::Impl
{
public:
	Impl()
	{
		using namespace ActionCode;
	#define ATTACH(name) processPtrs[name] = &Impl::action ## name;
		ATTACH(EndFlag)
		ATTACH(GotoFrame)
		ATTACH(PrevFrame)
		ATTACH(Play)
		ATTACH(Stop)
		ATTACH(ToggleQuality)
		ATTACH(StopSounds)
		ATTACH(WaitFrame)
		ATTACH(SetTarget)
		ATTACH(GoToLabel)
		ATTACH(Push)
		ATTACH(Pop)
		ATTACH(Add)
		ATTACH(Subtract)
		ATTACH(Multiply)
		ATTACH(Divide)
		ATTACH(Equals)
		ATTACH(Less)
		ATTACH(And)
		ATTACH(Or)
		ATTACH(Not)
		ATTACH(StringEquals)
		ATTACH(StringLength)
		ATTACH(StringAdd)
		ATTACH(StringExtract)
		ATTACH(StringLess)
		ATTACH(MBStringLength)
		ATTACH(MBStringExtract)
		ATTACH(ToInteger)
		ATTACH(CharToAscii)
		ATTACH(AsciiToChar)
		ATTACH(MBCharToAscii)
		ATTACH(MBAsciiToChar)
		ATTACH(Jump)
		ATTACH(If)
		ATTACH(Call)
		ATTACH(GetVariable)
		ATTACH(SetVariable)
		ATTACH(GetURL2)
		ATTACH(GotoFrame2)
		ATTACH(SetTarget2)
		ATTACH(GetProperty)
		ATTACH(SetProperty)
		ATTACH(CloneSprite)
		ATTACH(RemoveSprite)
		ATTACH(StartDrag)
		ATTACH(EndDrag)
		ATTACH(WaitForFrame2)
		ATTACH(Trace)
		ATTACH(GetTime)
		ATTACH(RandomNumber)
		ATTACH(CallFunction)
		ATTACH(CallMethod)
		ATTACH(ConstantPool)
		ATTACH(DefineFunction)
		ATTACH(DefineLocal)
		ATTACH(DefineLocal2)
		ATTACH(Delete)
		ATTACH(Delete2)
		ATTACH(Enumerate)
		ATTACH(Equals2)
		ATTACH(GetMember)
		ATTACH(InitArray)
		ATTACH(InitObject)
		ATTACH(NewMethod)
		ATTACH(NewObject)
		ATTACH(SetMember)
		ATTACH(TargetPath)
		ATTACH(With)
		ATTACH(ToNumber)
		ATTACH(ToString)
		ATTACH(TypeOf)
		ATTACH(Add2)
		ATTACH(Less2)
		ATTACH(Modulo)
		ATTACH(BitAnd)
		ATTACH(BitLShift)
		ATTACH(BitOr)
		ATTACH(BitRShift)
		ATTACH(BitURShift)
		ATTACH(BitXor)
		ATTACH(Decrement)
		ATTACH(Increment)
		ATTACH(PushDuplicate)
		ATTACH(Return)
		ATTACH(StackSwap)
		ATTACH(StoreRegister)
		ATTACH(InstanceOf)
		ATTACH(Enumerate2)
		ATTACH(StrictEquals)
		ATTACH(Greater)
		ATTACH(StringGreater)
		ATTACH(DefineFunction2)
		ATTACH(Extends)
		ATTACH(CastOp)
		ATTACH(ImplementsOp)
		ATTACH(Try)
		ATTACH(Throw)
	#undef ATTACH
	}
	
	void Process(const uint8_t* buff, ActionCode::Enum code, size_t recLen)
	{
		this->buff = buff;
		this->code = code;
		this->recLen = recLen;
		const ProcessPtr p = processPtrs[code];
		if (p) {
			(this->*p)();
		}
	}
	
private:
	
	const uint8_t* buff;
	ActionCode::Enum code;
	size_t recLen;
	
	typedef void (Impl::*ProcessPtr)();
	ProcessPtr processPtrs[256];
	
	// stack
	struct NullValue { };
	struct UndefinedValue { };
	struct RegisterNum {
		uint8_t n;
	};
	struct Value {
		enum Type {
			Type_String,
			Type_Float,
			Type_Null,
			Type_Undefined,
			Type_Register,
			Type_Boolean,
			Type_Double,
			Type_Object,
		} type;
		union {
			const char* str;
			float f;
			NullValue null;
			UndefinedValue undefined;
			RegisterNum registerNum;
			bool boolean;
			double d;
			Object* obj;
		};
	};
	
	double toFloat(const Value& v) {
		switch (v.type) {
		case Value::Type_Float:
			return v.f;
		case Value::Type_Register:
//			return toFloat();
		case Value::Type_Double:
			return v.d;
		default:
			return 0.0;
		}
	}
	std::string toString(const Value& v) {
		char buff[32];
		buff[0] = 0;
		switch (v.type) {
		case Value::Type_Float:
			return _gcvt(v.f, 8, buff);
		case ActionPushType::Null:
			return "[object Null]";
		case ActionPushType::Undefined:
			return "[object Undefined]";
		case Value::Type_Register:
//			return toFloat();
		case Value::Type_Double:
			return _gcvt(v.d, 8, buff);
		default:
			return "";
		}
	}
	double toInteger(const Value& v) {
		switch (v.type) {
		case Value::Type_Float:
			return fabs(v.f);
		case Value::Type_Register:
//			return toFloat();
		case Value::Type_Double:
			return fabs(v.d);
		default:
			return 0;
		}
	}
	Object* toObject(const Value& v) {
		switch (v.type) {
//		case Value::Type_Float:
//			return fabs(v.f);
//		case Value::Type_Register:
//			return toFloat();
//		case Value::Type_Double:
//			return fabs(v.d);
		case Value::Type_Object:
			return v.obj;
//		default:
//			return 0;
		}
	}

	std::vector<Value> stack;

	void stackPush(const char* str) {
		Value v;
		v.str = str;
		stack.push_back(v);
	}
	void stackPush(float f) {
		Value v;
		v.f = f;
		stack.push_back(v);
	}
	void stackPush(NullValue null) {
		Value v;
		v.null = null;
		stack.push_back(v);
	}
	void stackPush(UndefinedValue undefined) {
		Value v;
		v.undefined = undefined;
		stack.push_back(v);
	}
	void stackPush(RegisterNum registerNum) {
		Value v;
		v.registerNum = registerNum;
		stack.push_back(v);
	}
	void stackPush(bool boolean) {
		Value v;
		v.boolean = boolean;
		stack.push_back(v);
	}
	void stackPush(double d) {
		Value v;
		v.d = d;
		stack.push_back(v);
	}
	void stackPush(Object* obj) {
		Value v;
		v.obj = obj;
		stack.push_back(v);
	}
	Value stackPop() {
		Value ret = stack.back();
		stack.pop_back();
		return ret;
	}

	Object global;
	Object local;
	
	void actionEndFlag() {
	
	}

#include "action/swf3.h"
#include "action/swf4.h"
#include "action/swf5.h"
#include "action/swf6.h"
#include "action/swf7.h"

};

} // namespace ActionScript
} // namespace SWF
