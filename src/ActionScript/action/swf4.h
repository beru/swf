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
			{
				RegisterNum r;
				r.n = *(const uint8_t*)pData;
				stackPush(r);
			}
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
			stackPush(constantPool[*pData]);
			++pData;
			break;
		case Constant16:
			stackPush(constantPool[*(const uint16_t*)pData]);
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
	stackPush(toBoolean(a) == 0.0);
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
	Value a = stackPop();
	std::string name = toString(a);
	if (name == "_global") {
		stackPush(&global);
	}else {
		if (global.HasMember(name)) {
			stackPush(global.GetMember(name));
		}else {
			stackPush(UndefinedValue());
		}
	}
}

void actionSetVariable() {
	Value value = stackPop();
	Value name = stackPop();

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
	Value value = stackPop();
}

void actionGetTime() {
	DWORD time = GetTickCount();
	stackPush((double)time);
}

void actionRandomNumber() {
	double max = toInteger(stackPop());
	
}

