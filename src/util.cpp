
#include "util.h"

#include <windows.h>

std::string ToMultiByte(const wchar_t* val)
{
	int length( ::WideCharToMultiByte( CP_ACP, 0, val, -1, NULL, 0, NULL, NULL ) );
	std::string result( length - 1, 0 );
	::WideCharToMultiByte( CP_ACP, 0, &val[ 0 ], -1, &result[ 0 ], length, NULL, NULL );
	return result;
}

std::wstring ToWideChar(const char* val)
{
	int length( ::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, &val[ 0 ], -1, NULL, 0 ) );
	std::wstring result( length, 0 );
	::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, &val[ 0 ], -1, &result[ 0 ], length );
	return result;
}

std::string utf8_to_sjis(const char* utf8)
{
	int length = ::MultiByteToWideChar( CP_UTF8, 0, utf8, -1, NULL, 0 );
	if (length == 0) {
		DWORD err = GetLastError();
		int hoge = 0;
	}
	std::wstring utf16( length, 0 );
	::MultiByteToWideChar( CP_UTF8, 0, utf8, -1, &utf16[ 0 ], length );
	
	length = ::WideCharToMultiByte( CP_ACP, 0, &utf16[0], -1, NULL, 0, NULL, NULL );
	std::string sjis( length - 1, 0 );
	::WideCharToMultiByte( CP_ACP, 0, &utf16[0], -1, &sjis[ 0 ], length, NULL, NULL );
	return sjis;
}


