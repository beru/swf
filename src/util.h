#pragma once

#include <string>

std::string utf8_to_ansi(const char* utf8);
std::string ansi_to_utf8(const char* ansi);
std::string utf16_to_ansi(const wchar_t* utf16);
std::string utf16_to_utf8(const wchar_t* utf16);

std::wstring utf8_to_utf16(const char* utf8);
std::wstring ansi_to_utf16(const char* ansi);

inline std::string utf8_to_ansi(const std::string& utf8) { return utf8_to_ansi(utf8.c_str()); }
inline std::string ansi_to_utf8(const std::string& ansi) { return utf8_to_ansi(ansi.c_str()); }
inline std::string utf16_to_ansi(const std::wstring& utf16) { return utf16_to_ansi(utf16.c_str()); }
inline std::string utf16_to_utf8(const std::wstring& utf16) { return utf16_to_utf8(utf16.c_str()); }

inline std::wstring utf8_to_utf16(const std::string& utf8) { return utf8_to_utf16(utf8.c_str()); }
inline std::wstring ansi_to_utf16(const std::string& ansi) { return ansi_to_utf16(ansi.c_str()); }

