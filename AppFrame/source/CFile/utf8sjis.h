#pragma once

#ifdef WIN32
#include <windows.h>
#endif
#include <string>
#include <string_view>

class Utf8Sjis
{
public:
	Utf8Sjis() = delete;

	// Shift_JISの文字が漢字かどうかを判定する関数
	static bool SJIS_IsKanji(unsigned char cData);

	// UTF-8の文字が何バイトで構成されているかを判定する関数
	static int UTF8_OctBytes(char8_t c);

	// UTF-8エンコードされた文字列をShift_JISエンコードされた文字列に変換する関数
	[[nodiscard]] static std::string ConvUTF8toSJIS(std::u8string_view strUtf8);

	// Shift_JISエンコードされた文字列をUTF-8エンコードされた文字列に変換する関数
	[[nodiscard]] static std::u8string ConvSJIStoUTF8(std::string_view strSjis);

	// UTF-8エンコードされた文字列をShift_JISエンコードされた文字列に変換する関数
	static int ConvUTF8toSJIS(const char8_t* pszUtf8, char* pszOutSjis, int nOutSjisSize);

	// Shift_JISエンコードされた文字列をUTF-8エンコードされた文字列に変換する関数
	static int ConvSJIStoUTF8(const char* pszSjis, char8_t* pszOutUtf8);

	// UTF-8エンコードされた文字列をUTF-32エンコードされた文字列に変換する関数
	char32_t ConvUTF8toUTF32Single(const char8_t* input);

private:
	// UTF-8エンコードされた文字をShift_JISエンコードされた文字に変換する関数
	[[nodiscard]] static unsigned long ConvSJIStoUTF8Single(unsigned long lSjis);

	// UTF-8エンコードされた文字をShift_JISエンコードされた文字に変換する関数
	[[nodiscard]] static unsigned long ConvUTF8toSJISSingle(unsigned long lUtf8);

	// Shift_JISからUTF-8への変換を行う関数へのポインタ
	static int(*s_pfunc_SJIStoUTF8)(char*, int, char*);

	// UTF-8からShift_JISへの変換を行う関数へのポインタ
	static int(*s_pfunc_UTF8toSJIS)(char*, int, char*);

};

