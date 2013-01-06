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

