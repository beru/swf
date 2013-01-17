#pragma once

#include "array_scan.h"

namespace XMLBinding
{

/*
inline
bool convert(const char* from, size_t fromLen, CString& to)
{
	to = from;
	return true;
}

inline
std::string ToString(const CString& from)
{
#ifdef _UNICODE
	return ToMultiByte(from);
#else
	return std::string((const char*)from);
#endif
}
*/

inline
bool convert(const char* from, size_t fromLen, bool& to)
{
	to = (strcmp(from, "1") == 0);
	return true;
}

inline
std::string ToString(const bool& from)
{
	return from ? std::string("1") : std::string("0");
}

template <typename T>
bool convert(const char* from, size_t fromLen, T& to)
{
	static const size_t BUFFSIZE = 128;
	char buff[BUFFSIZE+1];
	assert(fromLen < BUFFSIZE);
	memcpy(buff, from, fromLen);
	buff[fromLen] = 0;
	if (fromLen > 2 && buff[0] == '0' && buff[1] == 'x') {
		to = (T) strtoul(buff+2, 0, 16);
	}else {
		to = (T) atoi(buff);
	}
	return true;
}

inline
bool convert(const char* from, size_t fromLen, double& to)
{
	static const size_t BUFFSIZE = 128;
	char buff[BUFFSIZE+1];
	assert(fromLen < BUFFSIZE);
	memcpy(buff, from, fromLen);
	buff[fromLen] = 0;
	to = atof(buff);
	return true;
}

inline
bool convert(const char* from, size_t fromLen, std::string& to)
{
	to = from;
	return true;
}

template <typename T>
inline
std::string ToString(const T& from)
{
	char buff[32];
	return _itoa(from, buff, 10);
}

inline
std::string ToString(double from)
{
	char buff[32];
	_gcvt(from, 4, buff);
	return buff;
}

inline
std::string ToString(const std::string& to)
{
	return to;
}

const char* FindMatchingRightSquareBracketPosition(const char* from, const char* end)
{
	const char* readPos = from;
	size_t stackLevel = 0;
	while (readPos < end) {
		ArrayTokenType tokenType = array_scan(readPos);
		switch (tokenType) {
		case ArrayTokenType_LEFT_SQUARE_BRACKET:
			++stackLevel;
			break;
		case ArrayTokenType_RIGHT_SQUARE_BRACKET:
			if (stackLevel == 0) {
				return readPos;
			}
			--stackLevel;
			break;
		}
	}
	return end;
}

template <typename T>
__declspec(noinline)
const char* readArrayStr(
	const char* from, const char* end,
	T* to, size_t toArrayLen,
	size_t arrayDepth)
{
	size_t arrPos = 0;
	const char* readPos = from;
	while (readPos < end) {
		const char* oldReadPos = readPos;
		ArrayTokenType tokenType = array_scan(readPos);
		switch (tokenType) {
		case ArrayTokenType_LEFT_SQUARE_BRACKET:
			{
				const char* arrayEndPos = FindMatchingRightSquareBracketPosition(readPos, end);
				const char* ret = readArrayStr(readPos, arrayEndPos, to, toArrayLen, arrayDepth+1);
				if (ret == end) {
					return ret;
				}
			}
			break;
		case ArrayTokenType_RIGHT_SQUARE_BRACKET:
			return readPos;
			break;
		case ArrayTokenType_VALUE:
			if (!convert(oldReadPos, readPos-oldReadPos, to[arrPos])) {
				return end;
			}
			++arrPos;
			if (arrPos == toArrayLen) {
				return readPos;
			}
			break;
		case ArrayTokenType_COMMA:
			break;
		}
	}
	return end;
}

namespace {

template <typename Type, Type Value>
struct integral_constant {
    static const Type value = Value;
    typedef Type value_type;
};

typedef integral_constant<bool, true>  true_type;
typedef integral_constant<bool, false> false_type;

// is_array : ”z—ñŒ^
template <typename Type>
struct is_array : public false_type {};

template <typename Type, int Size>
struct is_array<Type[Size]> : public true_type {};

template <typename Type>
struct is_array<Type[]> : public true_type {};

} // namespace {

template<typename T, size_t N>
const char* convert(const char* from, const char* end, T (&to)[N], false_type v)
{
	return readArrayStr(from, end, to, N, 0);
}

template<typename T, size_t N>
const char* convert(const char* from, const char* end, T (&to)[N], true_type v)
{
	const char* subStart = from;
	for (size_t i=0; i<N; ++i) {
		const char* subEnd = FindMatchingRightSquareBracketPosition(from+1, end);
		subStart = convertSub(
			subStart,
			subEnd,
			to[i]
		);
		if (subStart == end) {
			break;
		}
		++subStart;
	}
	return end;
}

template<typename T, size_t N>
const char* convertSub(const char* from, const char* end, T (&to)[N])
{
	return convert(from, end, to, is_array<T>());
}

template <typename T, size_t N>
inline
bool convert(const char* from, size_t fromLen, T (&to)[N])
{
	const char* end = convertSub(
		from,
		from+fromLen,
		to
	);
	return true;
}

template <typename T>
__declspec(noinline)
std::string ToString(const T* from, size_t len)
{
	std::string ret = "[";
	for (size_t i=0; i<len; ++i) {
		if (i != 0) {
			ret += ",";
		}
		ret += ToString(from[i]);
	}
	ret += "]";
	return ret;
}

template <typename T, size_t N>
inline
std::string ToString(const T(&from)[N])
{
	return ToString(from, N);
}

} // namespace XMLBinding

