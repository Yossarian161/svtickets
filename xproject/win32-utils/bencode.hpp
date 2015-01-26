#ifndef _win32_utils_bencode_hpp_
#define _win32_utils_bencode_hpp_

namespace win32_utils
{
	/**
	 *	Functions:	ansi to unicode
	 *	Input:		Ansi 格式字符
	 *	Output:		null
	 *	Return:		Unicode 格式字符
	 *
	 *	Create	@jianyaoy	[2012-9-4 22:52]	
	 */
	inline wchar_t* AnsiToUnicode(const char *szAnsi)
	{
		// 与转换，得到所需空间的大小
		int wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, szAnsi, strlen(szAnsi), NULL, 0);
		//分配空间要给'\0'留个空间，MultiByteToWideChar不会给'\0'空间
		wchar_t* szUnicode = new wchar_t[wcsLen + 1];
		//转换
		::MultiByteToWideChar(CP_ACP, NULL, szAnsi, strlen(szAnsi), szUnicode, wcsLen);
		//最后加上 '\0'
		szUnicode[wcsLen] = '\0';

		return szUnicode;
	}

	/**
	 *	Functions:	unicode to ansi
	 *	Input:		unicode 格式字符
	 *	Output:		null
	 *	Return:		Ansi 格式字符
	 *
	 *	Create	@jianyaoy	[2012-9-4 22:51]	
	 */
	inline  char * UnicodeToAnsi(const wchar_t* szUnicode)
	{
		//预转换，得到所需空间的大小
		int ansiLen = ::WideCharToMultiByte(CP_ACP, NULL, szUnicode, wcslen(szUnicode), NULL, 0, NULL, NULL);
		//分配空间要给'\0'留个空间
		char* szAnsi = new char[ansiLen + 1];
		//转换
		//unicode版对应的strlen是wcslen
		::WideCharToMultiByte(CP_ACP, NULL, szUnicode, wcslen(szUnicode), szAnsi, ansiLen, NULL, NULL);
		//最后加上'\0'
		szAnsi[ansiLen] = '\0';

		return szAnsi;
	}

	/**
	 *	Functions:	utf8 to unicode
	 *	Input:		utf8 格式字符
	 *	Output:		null
	 *	Return:		unicode 格式字符
	 *
	 *	Create	@jianyaoy	[2012-9-4 22:51]	
	 */
	inline  wchar_t * Utf8ToUnicode(const char* szUtf)
	{
		//预转换，得到所需空间的大小
		int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szUtf, strlen(szUtf), NULL, 0);
		//分配空间要给'\0'留个空间，MultiByteToWideChar不会给'\0'空间
		wchar_t* szUnicode = new wchar_t[wcsLen + 1];
		//转换
		::MultiByteToWideChar(CP_UTF8, NULL, szUtf, strlen(szUtf), szUnicode, wcsLen);
		//最后加上'\0'
		szUnicode[wcsLen] = '\0';

		return szUnicode;
	}

	/**
	 *	Functions:	unicode to utf8
	 *	Input:		unicode格式 字符
	 *	Output:		null
	 *	Return:		utf8 格式字符
	 *
	 *	Create	@jianyaoy	[2012-9-4 22:50]	
	 */
	inline char* UnicodeToUtf8(const wchar_t* szUnicode)
	{
		//预转换，得到所需空间的大小，这次用的函数和上面名字相反
		int u8Len = ::WideCharToMultiByte(CP_UTF8, NULL, szUnicode, wcslen(szUnicode), NULL, 0, NULL, NULL);
		//UTF8虽然是Unicode的压缩形式，但也是多字节字符串，所以可以以char的形式保存
		char* szUtf = new char[u8Len + 1];
		//转换
		//unicode版对应的strlen是wcslen
		::WideCharToMultiByte(CP_UTF8, NULL, szUnicode, wcslen(szUnicode), szUtf, u8Len, NULL, NULL);
		//最后加上'\0'
		szUtf[u8Len] = '\0';

		return szUtf;
	}
}

#endif