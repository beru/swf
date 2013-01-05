#pragma once

#include <boost/variant.hpp>
#include <vector>
#include <string>

#include "Interpreter.h"
#include "util.h"

namespace SWF {

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
		RegisterNum(uint8_t n) : n(n) {}
		uint8_t n;
	};
	typedef boost::variant<
		const char*,
		float,
		NullValue,
		UndefinedValue,
		RegisterNum,
		bool,
		double,
		std::string
	> Value;
	double toFloat(const Value& v) {
		switch (v.which()) {
		case ActionPushType::Float:
			return boost::get<float>(v);
		case ActionPushType::Register:
//			return toFloat();
		case ActionPushType::Double:
			return boost::get<double>(v);
		default:
			return 0.0;
		}
	}
	std::string toString(const Value& v) {
		char buff[32];
		buff[0] = 0;
		switch (v.which()) {
		case ActionPushType::Float:
			return _gcvt(boost::get<float>(v), 8, buff);
		case ActionPushType::Null:
			return "[object Null]";
		case ActionPushType::Undefined:
			return "[object Undefined]";
		case ActionPushType::Register:
//			return toFloat();
		case ActionPushType::Double:
			return _gcvt(boost::get<double>(v), 8, buff);
		default:
			return "";
		}
	}
	double toInteger(const Value& v) {
		switch (v.which()) {
		case ActionPushType::Float:
			return fabs(boost::get<float>(v));
		case ActionPushType::Register:
//			return toFloat();
		case ActionPushType::Double:
			return fabs(boost::get<double>(v));
		default:
			return 0;
		}
	}
	std::vector<Value> stack;

	void stackPush(const Value& v) {
		stack.push_back(v);
	}
	Value stackPop() {
		Value ret = stack.back();
		stack.pop_back();
		return ret;
	}
	
	void actionEndFlag() {
	
	}
	// SWF 3 actions
	void actionGotoFrame() {
		throw "not implemented";
	}
	void actionPrevFrame() {
		throw "not implemented";
	}
	void actionPlay() {
		throw "not implemented";
	}
	void actionStop() {
		throw "not implemented";
	}
	void actionToggleQuality() {
		throw "not implemented";
	}
	void actionStopSounds() {
		throw "not implemented";
	}
	void actionWaitFrame() {
		throw "not implemented";
	}
	void actionSetTarget() {
		throw "not implemented";
	}
	void actionGoToLabel() {
		throw "not implemented";
	}
	// SWF 4 actions
	void actionPush() {
		using namespace ActionPushType;
		const uint8_t* pEnd = buff + 3 + recLen;
		const uint8_t* pData = buff + 3;
		while (pData < pEnd) {
			Enum type = (ActionPushType::Enum) *pData++;
			switch (type) {
			case String:
				stackPush((const char*)pData);
				pData += strlen((const char*)pData) + 1;
				break;
			case Float:
				stackPush(*(const float*)pData);
				pData += 4;
				break;
			case Null:
				stackPush(NullValue());
				break;
			case Undefined:
				stackPush(UndefinedValue());
				break;
			case Register:
				stackPush(RegisterNum(*(const uint8_t*)pData));
				++pData;
				break;
			case Boolean:
				stackPush((*(const uint8_t*)pData == 0) ? true : false);
				++pData;
				break;
			case Double:
				stackPush(*(const double*)pData);
				pData += 8;
				break;
			case Integer:
				stackPush((double)*(const uint32_t*)pData);
				pData += 4;
				break;
			case Constant8:
				++pData;
				break;
			case Constant16:
				pData += 2;
				break;
			}
		}
	}
	void actionPop() {
		stackPop();
	}
	void actionAdd() {
		Value a = stackPop();
		Value b = stackPop();
		stackPush(toFloat(a) + toFloat(b));
	}
	void actionSubtract() {
		Value a = stackPop();
		Value b = stackPop();
		stackPush(toFloat(b) - toFloat(a));
	}
	void actionMultiply() {
		Value a = stackPop();
		Value b = stackPop();
		stackPush(toFloat(a) * toFloat(b));
	}
	void actionDivide() {
		Value a = stackPop();
		Value b = stackPop();
		stackPush(toFloat(b) / toFloat(a));
	}
	void actionEquals() {
		Value a = stackPop();
		Value b = stackPop();
		stackPush(toFloat(a) == toFloat(b));
	}
	void actionLess() {
		Value a = stackPop();
		Value b = stackPop();
		stackPush(toFloat(b) < toFloat(a));
	}
	void actionAnd() {
		Value a = stackPop();
		Value b = stackPop();
		stackPush(toFloat(a) != 0.0 && toFloat(b) != 0.0);
	}
	void actionOr() {
		Value a = stackPop();
		Value b = stackPop();
		stackPush(toFloat(a) != 0.0 || toFloat(b) != 0.0);
	}
	void actionNot() {
		Value a = stackPop();
		stackPush(toFloat(a) == 0.0);
	}
	void actionStringEquals() {
		Value a = stackPop();
		Value b = stackPop();
		stackPush(toString(a) == toString(b));
	}
	void actionStringLength() {
		Value a = stackPop();
		stackPush((double)strlen(toString(a).c_str()));
	
	}
	void actionStringAdd() {
		Value a = stackPop();
		Value b = stackPop();
		stackPush(toString(b) + toString(a));
	}
	void actionStringExtract() {
		Value count = stackPop();
		Value index = stackPop();
		Value string = stackPop();
		std::string str = toString(string);
		stackPush(str.substr(toInteger(index), toInteger(count)));
	}
	void actionStringLess() {
		Value a = stackPop();
		Value b = stackPop();
		stackPush(toString(b) < toString(a));
	}
	void actionMBStringLength() {
		Value a = stackPop();
		stackPush((double)utf8_to_utf16(toString(a)).size());
	}
	void actionMBStringExtract() {
		Value count = stackPop();
		Value index = stackPop();
		Value string = stackPop();
		stackPush(utf16_to_utf8(utf8_to_utf16(toString(string)).substr(toInteger(index), toInteger(count))));
	}
	void actionToInteger() {
		Value a = stackPop();
		stackPush(fabs(toFloat(a)));
	}
	void actionCharToAscii() {
		Value a = stackPop();
		std::string s = toString(a);
		int code = 0;
		if (s.size()) {
			code = s[0];
		}
		stackPush((double)code);
	}
	void actionAsciiToChar() {
		Value a = stackPop();
		stackPush(std::string((char)toInteger(a), 1));
	}
	void actionMBCharToAscii() {
		Value a = stackPop();
		std::wstring s = utf8_to_utf16(toString(a));
		int code = 0;
		if (s.size()) {
			code = s[0];
		}
		stackPush((double)code);
	}
	void actionMBAsciiToChar() {
		Value a = stackPop();
		stackPush(utf16_to_utf8(std::wstring((char)toInteger(a), 1)));
	}
	void actionJump() {
		throw "not implemented";
	}
	void actionIf() {
		Value a = stackPop();
		throw "not implemented";
	}
	void actionCall() {
		throw "not implemented";
	}
	void actionGetVariable() {
		
	}
	void actionSetVariable() {
	
	}
	void actionGetURL2() {
		throw "not implemented";
	}
	void actionGotoFrame2() {
		throw "not implemented";
	}
	void actionSetTarget2() {
		throw "not implemented";
	}
	void actionGetProperty() {
	
	}
	void actionSetProperty() {
	
	}
	void actionCloneSprite() {
		throw "not implemented";
	}
	void actionRemoveSprite() {
		throw "not implemented";
	}
	void actionStartDrag() {
		throw "not implemented";
	}
	void actionEndDrag() {
		throw "not implemented";
	}
	void actionWaitForFrame2() {
		throw "not implemented";
	}
	void actionTrace() {
	
	}
	void actionGetTime() {
	
	}
	void actionRandomNumber() {
	
	}
	
	// SWF 5 actions
	void actionCallFunction() {
	
	}
	void actionCallMethod() {
	
	}
	void actionConstantPool() {
	
	}
	void actionDefineFunction() {
	
	}
	void actionDefineLocal() {
	
	}
	void actionDefineLocal2() {
	
	}
	void actionDelete() {
	
	}
	void actionDelete2() {
	
	}
	void actionEnumerate() {
	
	}
	void actionEquals2() {
	
	}
	void actionGetMember() {
	
	}
	void actionInitArray() {
	
	}
	void actionInitObject() {
	
	}
	void actionNewMethod() {
	
	}
	void actionNewObject() {
	
	}
	void actionSetMember() {
	
	}
	void actionTargetPath() {
	
	}
	void actionWith() {
	
	}
	void actionToNumber() {
	
	}
	void actionToString() {
	
	}
	void actionTypeOf() {
	
	}
	void actionAdd2() {
	
	}
	void actionLess2() {
	
	}
	void actionModulo() {
	
	}
	void actionBitAnd() {
	
	}
	void actionBitLShift() {
	
	}
	void actionBitOr() {
	
	}
	void actionBitRShift() {
	
	}
	void actionBitURShift() {
	
	}
	void actionBitXor() {
	
	}
	void actionDecrement() {
	
	}
	void actionIncrement() {
	
	}
	void actionPushDuplicate() {
	
	}
	void actionReturn() {
	
	}
	void actionStackSwap() {
	
	}
	void actionStoreRegister() {
	
	}
	// SWF 6 actions
	void actionInstanceOf() {
	
	}
	void actionEnumerate2() {
	
	}
	void actionStrictEquals() {
	
	}
	void actionGreater() {
	
	}
	void actionStringGreater() {
	
	}
	// SWF 7 actions
	void actionDefineFunction2() {
	
	}
	void actionExtends() {
	
	}
	void actionCastOp() {
	
	}
	void actionImplementsOp() {
	
	}
	void actionTry() {
	
	}
	void actionThrow() {
	
	}

};

} // namespace SWF
