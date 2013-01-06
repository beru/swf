
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
	Value name = stackPop();
	Value so = stackPop();
	Object* member = toObject(so)->GetMember(toString(name));
	stackPush(member);
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

