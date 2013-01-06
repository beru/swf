
// SWF 5 actions

void actionCallFunction() {
	Value name = stackPop();
	
	Value numArgs = stackPop();
	size_t cnt = toInteger(numArgs);
	for (size_t i=0; i<cnt; ++i) {
		
	}
}

void actionCallMethod() {
	Value methodName = stackPop();
	Value scriptObject = stackPop();

}

void actionConstantPool() {
	constantPool.clear();
	const char* pBuff = (const char*)buff + 3;
	uint16_t count = *(const uint16_t*)pBuff;
	pBuff += 2;
	for (size_t i=0; i<count; ++i) {
		constantPool.push_back(pBuff);
		size_t slen = strlen(pBuff);
		pBuff += (slen + 1);
	}
}

void actionDefineFunction() {
	const char* pBuff = (const char*)buff + 3;
	const char* pFuncName = pBuff;
	size_t slen = strlen(pFuncName);
	pBuff += (slen + 1);
	uint16_t nParams = *(const uint16_t*)pBuff;
	pBuff += 2;
	for (size_t i=0; i<nParams; ++i) {
		const char* paramName = pBuff;
		slen = strlen(paramName);
		pBuff += (slen + 1);
	}
	uint16_t codeSize = *(const uint16_t*)pBuff;
}

void actionDefineLocal() {
	Value value = stackPop();
	Value name = stackPop();

}

void actionDefineLocal2() {
	Value name = stackPop();
}

void actionDelete() {
	Value name = stackPop();
	Value scriptObject = stackPop();
}

void actionDelete2() {
	Value name = stackPop();
	
}

void actionEnumerate() {
	Value name = stackPop();
	
}

void actionEquals2() {

}

void actionGetMember() {
	std::string name = toString(stackPop());
	Object* obj = toObject(stackPop());
	if (obj->HasMember(name)) {
		Object* member = obj->GetMember(name);
		stackPush(member);
	}else {
		stackPush(UndefinedValue());
	}
}

void actionInitArray() {
	Value nArgs = stackPop();
	size_t n = toInteger(nArgs);
	Object* p = Array::New();
	for (size_t i=0; i<n; ++i) {
		Value v = stackPop();
	}
	stackPush(p);
}

void actionInitObject() {
	Value nProps = stackPop();
	size_t n = toInteger(nProps);
	Object* p = Object::New();
	for (size_t i=0; i<n; ++i) {
		Value value = stackPop();
		Value name = stackPop();
	
	}
	stackPush(p);
}

void actionNewMethod() {
	Value name = stackPop();
	Value so = stackPop();
	Value nArgs = stackPop();
	size_t n = toInteger(nArgs);
	for (size_t i=0; i<n; ++i) {
		stackPop();
	}
//	stackPush();
}

void actionNewObject() {
	Value name = stackPop();
	Value nArgs = stackPop();
	size_t n = toInteger(nArgs);
	for (size_t i=0; i<n; ++i) {
		stackPop();
	}
//	stackPush();
}

void actionSetMember() {
	Value value = stackPop();
	Value name = stackPop();
	Value obj = stackPop();
	
}

void actionTargetPath() {
	stackPop();
//	stackPush();
}

void actionWith() {
	
}

void actionToNumber() {
	Value value = stackPop();
	Object* obj = toObject(value);

}

void actionToString() {
	Value value = stackPop();
	Object* obj = toObject(value);
}

void actionTypeOf() {
	Value value = stackPop();

}

void actionAdd2() {
	Value arg1 = stackPop();
	Value arg2 = stackPop();
}

void actionLess2() {
	Value arg1 = stackPop();
	Value arg2 = stackPop();
}

void actionModulo() {
	double x = toFloat(stackPop());
	double y = toFloat(stackPop());
	double result = y ? fmod(x, y) : (float)0x7FC00000;
	stackPush(result);
}

void actionBitAnd() {
	uint32_t arg1 = (uint32_t) toInteger(stackPop());
	uint32_t arg2 = (uint32_t) toInteger(stackPop());
	stackPush((double)(int32_t)(arg1 & arg2));
}

void actionBitLShift() {
	uint32_t shift = ((uint32_t) toInteger(stackPop())) & 0x1F;
	int32_t value = (int32_t) toInteger(stackPop());
	stackPush((double)(int32_t)(value << shift));
}

void actionBitOr() {
	uint32_t arg1 = (uint32_t) toInteger(stackPop());
	uint32_t arg2 = (uint32_t) toInteger(stackPop());
	stackPush((double)(int32_t)(arg1 | arg2));
}

void actionBitRShift() {
	uint32_t shift = ((uint32_t) toInteger(stackPop())) & 0x1F;
	int32_t value = (int32_t) toInteger(stackPop());
	stackPush((double)(int32_t)(value >> shift));
}

void actionBitURShift() {
	uint32_t shift = ((uint32_t) toInteger(stackPop())) & 0x1F;
	int32_t value = (int32_t) toInteger(stackPop());
	stackPush((double)(uint32_t)(value >> shift));
}

void actionBitXor() {
	uint32_t arg1 = (uint32_t) toInteger(stackPop());
	uint32_t arg2 = (uint32_t) toInteger(stackPop());
	stackPush((double)(int32_t)(arg1 ^ arg2));
}

void actionDecrement() {
	stackPush(toFloat(stackPop()) - 1.0);
}

void actionIncrement() {
	stackPush(toFloat(stackPop()) + 1.0);
}

void actionPushDuplicate() {
	stackPush(stack.back());
}

void actionReturn() {
	stackPop();
}

void actionStackSwap() {
	Value item1 = stackPop();
	Value item2 = stackPop();
	stackPush(item2);
	stackPush(item1);
}

void actionStoreRegister() {

}

