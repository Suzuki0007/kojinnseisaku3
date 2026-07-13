#include "pch.h"
#include "utf8sjis.h"
#include <cstring>
#include <bit>

int (*Utf8Sjis::s_pfunc_SJIStoUTF8 )(char*, int, char*) = nullptr;
int (*Utf8Sjis::s_pfunc_UTF8toSJIS )(char*, int, char*) = nullptr;

bool Utf8Sjis::SJIS_IsKanji(unsigned char cData)
{
	return (cData >= 0x81 && cData <= 0x9f) || (cData >= 0xe0 && cData <= 0xfc);
}

int Utf8Sjis::UTF8_OctBytes(char8_t c)
{
	if((c & 0x80) == 0x00) { return 1; }
	if((c & 0xe0) == 0xc0) { return 2; }
	if((c & 0xf0) == 0xe0) { return 3; }
	if((c & 0xf8) == 0xf0) { return 4; }
	if((c & 0xfc) == 0xf8) { return 5; }
	if((c & 0xfe) == 0xfc) { return 6; }
	return 0;
}

unsigned long Utf8Sjis::ConvSJIStoUTF8Single(unsigned long lSjis)
{
	unsigned char szSjis [ 8 ] = { 0 };
	// std::byteswapはデータのバイト順をまとめてひっくり返す関数
	// メリットは、複数のバイトを一度に処理できるため、効率的にバイト順を変換できることです
	// デメリットは、単純にバイト順を反転させるだけなので、
	// UTF-8のバイト列を生成する必要がある
	// メモリ状で数値を扱うときの並び順には2種類ある
	// リトルエンティティ（小さい値が先に来る）
	// ビッグエンティティ（大きい値が先に来る）
	// 例えば、0x12345678という数値を4バイトで表すとき
	// リトルエンティティの場合は、78 56 34 12の順でメモリに格納される
	// ビッグエンティティの場合は、12 34 56 78の順でメモリに格納される
	unsigned long swapped = std::byteswap(lSjis);
	std::memcpy(szSjis, &swapped, sizeof(lSjis));

	int inSize = 2;
	if(lSjis >= 0x1000000) { inSize = 5; }
	else if(lSjis >= 0x10000) { inSize = 4; }
	else if(lSjis >= 0x100) { inSize = 3; }

	// u8""は、UTF-8エンコードされた空白文字を表すリテラルで、char8_t型の配列に格納される
	char8_t buffUtf8 [ 16 ] = u8"";
	if(s_pfunc_SJIStoUTF8)
	{
		// std::bit_castは、型のオブジェクトを別の型のオブジェクトに変換するための関数です
		auto pInSjis = std::bit_cast< char* >(&szSjis[0]);
		auto pOutUtf8 = std::bit_cast< char* >(&buffUtf8[0]);
		// std::invokeは、関数ポインタや関数オブジェクトを呼び出すための関数です
		std::invoke(s_pfunc_SJIStoUTF8, pInSjis, inSize, pOutUtf8);
	}
	else
	{
		if(inSize == 2)
		{
			buffUtf8 [ 0 ] = szSjis [ 0 ];
		}

#ifdef ANDROID
#ifdef USE_ICONV
	#define MAX_BUF 1024
		char inbuf [ MAX_BUF + 1 ] = { 0 };	// 変換前のSJISを格納するバッファ
		char outbuf [ MAX_BUF + 1 ] = { 0 };// 変換後のUTF-8を格納するバッファ
		char* in = inbuf;// iconv関数に渡すために入力の先頭を指すポインタ
		char* out = outbuf;// iconv関数に渡すために出力の先頭を指すポインタ
		size_t inSizeIc = ( size_t ) MAX_BUF;// iconvが使用する変換前のSJISのサイズ
		size_t outSizeIc = ( size_t ) MAX_BUF;// iconvが使用する変換後の最大サイズ
		iconv_t ic = iconv_open("UTF-8", "SHIFT_JIS");// iconv(Lunux/Androidの文字変換する)の変換を作成

		strcpy(in, (char*)szSjis);// 変換前のSJISを入力バッファにコピー
		iconv(ic, &in, &inSizeIc, &out, &outSizeIc);// iconv関数を呼び出して、SJISからUTF-8への変換を実行
		iconv_close(ic);// iconvの変換を閉じる
		strcpy(( char* ) buffUtf8, outbuf);// 変換後のUTF-8を出力バッファからコピーして、最終的なUTF-8の結果を格納する
	#undef MAX_BUF
#endif
#endif
#ifdef WIN32
		wchar_t buffUtf16 [ 16 ];
		// WindowsのAPIを使って、Shift_JISからUTF-16への変換を行う
		MultiByteToWideChar(932, 0, ( LPCSTR ) szSjis, -1, buffUtf16, 16);
		// WindowsのAPIを使って、UTF-16からUTF-8への変換を行う
		WideCharToMultiByte(CP_UTF8, 0, ( LPCWSTR ) buffUtf16, -1, ( LPSTR ) buffUtf8, 16, nullptr, nullptr);
#endif

	}
	uint32_t rawValue = 0;
	std::memcpy(&rawValue, &buffUtf8, sizeof(rawValue));
	return std::byteswap(rawValue);
}

int Utf8Sjis::ConvSJIStoUTF8(const char* pszSjis, char8_t* pszOutUtf8)
{
	int n = 0;		// 変換されたUTF-8のバイト数をカウントするための変数
	int c = 0;		// 変換前のSJISのバイト数をカウントするための変数

	while(pszSjis[n] != '\0')
	{
		unsigned long l = (unsigned char)pszSjis[n++];
		if(SJIS_IsKanji((unsigned char)l))
		{
			// SJISの漢字は2バイトで表されるため、次のバイトも読み込む
			l = (l << 8) | (unsigned char) pszSjis[n++];
		}

		unsigned long x = ConvSJIStoUTF8Single(l);

		int byte = 1;
		if(x >= 0x1000000) { byte = 4; }
		else if(x >= 0x10000) { byte = 3; }
		else if(x >= 0x100) { byte = 2; }

		if(pszOutUtf8)
		{
			for(int i = byte - 1; i >= 0; i--)
			{
				pszOutUtf8[c++] = (x >> (i * 8)) & 0xff;
			}
		}
		else
		{
			c += byte;
		}
	}

	if(pszOutUtf8)
	{
		pszOutUtf8[c] = u8'\0'; // 変換されたUTF-8文字列の末尾にNULL文字を追加
	}

	return c + 1; // 変換されたUTF-8のバイト数を返す
}

unsigned long Utf8Sjis::ConvUTF8toSJISSingle(unsigned long lUtf8)
{
	unsigned char szUtf8[8] = { 0 };

	unsigned long swapped = std::byteswap(lUtf8);
	std::memcpy(szUtf8, &swapped, sizeof(lUtf8));

	int inSize = 2;
	if(lUtf8 >= 0x1000000) { inSize = 5; }
	else if(lUtf8 >= 0x10000) { inSize = 4; }
	else if(lUtf8 >= 0x100) { inSize = 3; }

	char buffSjis[16] = "";

	if(s_pfunc_UTF8toSJIS)
	{
		auto pInUtf8 = std::bit_cast<char*>(&szUtf8[0]);
		auto pOutSjis = std::bit_cast<char*>(&buffSjis[0]);
		std::invoke(s_pfunc_UTF8toSJIS, pInUtf8, inSize, pOutSjis);
	}
	else
	{
		if(inSize == 2)
		{
			buffSjis[0] = szUtf8[0];
		}

#ifdef WIN32
		wchar_t buffUtf16[16];
		MultiByteToWideChar(CP_UTF8, 0, (LPCSTR) szUtf8, -1, buffUtf16, 16);
		WideCharToMultiByte(932, 0, (LPCWSTR) buffUtf16, -1, (LPSTR) buffSjis, 16, nullptr, nullptr);
#endif

#ifdef ANDROID
#ifdef USE_ICONV
#define MAX_BUF 1024
		char inbuf[MAX_BUF + 1] = { 0 };	// 変換前のUTF-8を格納するバッファ
		char outbuf[MAX_BUF + 1] = { 0 };	// 変換後のSJISを格納するバッファ
		char* in = inbuf;					// iconv関数に渡すために入力の先頭を指すポインタ
		char* out = outbuf;				// iconv関数に渡すために出力の先頭を指すポインタ
		size_t inSizeIc = (size_t) MAX_BUF;	// iconvが使用する変換前のUTF-8のサイズ
		size_t outSizeIc = (size_t) MAX_BUF;// iconvが使用する変換後の最大サイズ
		iconv_t ic = iconv_open("SHIFT_JIS", "UTF-8"); // iconv(Lunux/Androidの文字変換する)の変換を作成
		strcpy(in, (char*) szUtf8);			// 変換前のUTF-8を入力バッファにコピー
		iconv(ic, &in, &inSizeIc, &out, &outSizeIc); // iconv関数を呼び出して、UTF-8からSJISへの変換を実行
		iconv_close(ic);					// iconvの変換を閉じる
		strcpy(buffSjis, outbuf);			// 変換後のSJISを出力バッファからコピーして、最終的なSJISの結果を格納する
#undef MAX_BUF
#endif
#endif
	}

	uint16_t rawValue = 0;
	std::memcpy(&rawValue, buffSjis, sizeof(rawValue));
	return std::byteswap(rawValue);
}

int Utf8Sjis::ConvUTF8toSJIS(const char8_t* pszUtf8, char* pszOutSjis, int nOutSjisSize)
{
	int n = 0;		// 【読み込み位置カウンター】元のUTF-8(pszUtf8)を読み進める
	int c = 0;		// 【書き込み位置カウンター】出力先のバイト数を数える・書き込む

	while(pszUtf8[n] != u8'\0')
	{
		int len = UTF8_OctBytes(pszUtf8[n]);
		unsigned long l = 0;

		// i = 0 から始め、読み込みカウンター n を使って正しく1文字分を読み進める
		for(int i = 0; i < len; i++)
		{
			l = (l << 8) | (unsigned char) pszUtf8[n++];
		}

		unsigned long x = ConvUTF8toSJISSingle(l);

		int byte = 1;
		if(x >= 0x100) { byte = 2; }

		// 本番書き込みモードの場合
		if(pszOutSjis)
		{
			if(c + byte >= nOutSjisSize)
			{
				break; // 出力バッファのサイズを超える場合は変換を中止
			}
			for(int i = byte - 1; i >= 0; i--)
			{
				pszOutSjis[c++] = (x >> (i * 8)) & 0xff; // 書き込み先を進める
			}
		}
		// サイズ計測モード（pszOutSjis == nullptr）の場合
		else
		{
			c += byte; // 純粋に必要バイト数だけをカウントアップしていく
		}
	}

	if(pszOutSjis && c < nOutSjisSize)
	{
		pszOutSjis[c] = '\0'; // 変換されたSJIS文字列の末尾にNULL文字を追加
	}

	return c + 1; // 変換されたSJISのバイト数を返す（サイズ計測モードでもここが正しく返ります）
}

std::u8string Utf8Sjis::ConvSJIStoUTF8(std::string_view strSjis)
{
	if(strSjis.empty())
	{
		return std::u8string();
	}

	int nSize = ConvSJIStoUTF8(strSjis.data(), nullptr);
	if(nSize <= 0)
	{
		return std::u8string();
	}

	std::u8string strOut;
	strOut.resize(nSize);

	// 変換されたUTF-8の変換を行う
	ConvSJIStoUTF8(strSjis.data(), strOut.data());
	strOut.resize(nSize - 1); // NULL文字を除くためにサイズを調整

	return strOut;
}

std::string Utf8Sjis::ConvUTF8toSJIS(std::u8string_view strUtf8)
{
	if(strUtf8.empty())
	{
		return std::string();
	}
	int nSize = ConvUTF8toSJIS(strUtf8.data(), nullptr, 0);
	if(nSize <= 0)
	{
		return std::string();
	}
	std::string strOut;
	strOut.resize(nSize);
	// 変換されたSJISの変換を行う
	ConvUTF8toSJIS(strUtf8.data(), strOut.data(), nSize);
	strOut.resize(nSize - 1); // NULL文字を除くためにサイズを調整
	return strOut;
}

char32_t Utf8Sjis::ConvUTF8toUTF32Single(const char8_t* input)
{
	if(!input || *input == u8'\0') { return 0; }

	// 最初（1バイト目）の文字を見て、このUTF-8が何バイト構成かを割り出す
	int len = UTF8_OctBytes(*input);

	// 1バイト文字
	if(len == 1)
	{
		return static_cast<char32_t>(input[0]);
	}

	// 2バイト文字
	if(len == 2)
	{
		return static_cast<char32_t>
		(
			((input[0] & 0x1F) << 6) | (input[1] & 0x3F)
		);
	}

	// 3バイト文字
	if(len == 3)
	{
		return static_cast<char32_t>
		(
			((input[0] & 0x0F) << 12) |
			((input[1] & 0x3F) << 6) |
			(input[2] & 0x3F)
		);
	}

	// 4バイト文字
	if(len == 4)
	{
		return static_cast<char32_t>
		(
			((input[0] & 0x07) << 18) |
			((input[1] & 0x3F) << 12) |
			((input[2] & 0x3F) << 6) |
			(input[3] & 0x3F)
		);
	}

	return 0;
}