
#include "util.h"

#include <windows.h>

std::string utf8_to_ansi(const char* utf8)
{
	return utf16_to_ansi(utf8_to_utf16(utf8));
}

std::string ansi_to_utf8(const char* ansi)
{
	return utf16_to_utf8(ansi_to_utf16(ansi));
}

static inline
std::string utf16_to_mbcs(const wchar_t* utf16, UINT codePage)
{
	int length = ::WideCharToMultiByte( codePage, 0, &utf16[0], -1, NULL, 0, NULL, NULL );
	std::string mbcs( length - 1, 0 );
	::WideCharToMultiByte( codePage, 0, &utf16[0], -1, &mbcs[ 0 ], length, NULL, NULL );
	return mbcs;
}

std::string utf16_to_ansi(const wchar_t* utf16)
{
	return utf16_to_mbcs(utf16, CP_ACP);
}

std::string utf16_to_utf8(const wchar_t* utf16)
{
	return utf16_to_mbcs(utf16, CP_UTF8);
}

static inline
std::wstring mbcs_to_utf16(const char* mbcs, UINT codePage)
{
	int length = ::MultiByteToWideChar( codePage, 0, mbcs, -1, NULL, 0 );
	if (length == 0) {
		DWORD err = GetLastError();
		int hoge = 0;
	}
	std::wstring utf16( length, 0 );
	::MultiByteToWideChar( codePage, 0, mbcs, -1, &utf16[ 0 ], length );
	return utf16;
}

std::wstring utf8_to_utf16(const char* utf8)
{
	return mbcs_to_utf16(utf8, CP_UTF8);
}

std::wstring ansi_to_utf16(const char* ansi)
{
	return mbcs_to_utf16(ansi, CP_ACP);
}

