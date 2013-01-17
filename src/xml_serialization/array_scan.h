#pragma once

enum ArrayTokenType {
	ArrayTokenType_LEFT_SQUARE_BRACKET,
	ArrayTokenType_RIGHT_SQUARE_BRACKET,
	ArrayTokenType_COMMA,
	ArrayTokenType_SPACE,
	ArrayTokenType_EOF,
	ArrayTokenType_EOL,
	ArrayTokenType_VALUE,
	ArrayTokenType_OTHER,
};

ArrayTokenType array_scan(const char*& p);

