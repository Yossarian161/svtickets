#ifndef _win32_utils_bencode_hpp_
#define _win32_utils_bencode_hpp_

namespace win32_utils
{
	/**
	 *	Functions:	ansi to unicode
	 *	Input:		Ansi ��ʽ�ַ�
	 *	Output:		null
	 *	Return:		Unicode ��ʽ�ַ�
	 *
	 *	Create	@jianyaoy	[2012-9-4 22:52]	
	 */
	inline wchar_t* AnsiToUnicode(const char *szAnsi)
	{
		// ��ת�����õ�����ռ�Ĵ�С
		int wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, szAnsi, strlen(szAnsi), NULL, 0);
		//����ռ�Ҫ��'\0'�����ռ䣬MultiByteToWideChar�����'\0'�ռ�
		wchar_t* szUnicode = new wchar_t[wcsLen + 1];
		//ת��
		::MultiByteToWideChar(CP_ACP, NULL, szAnsi, strlen(szAnsi), szUnicode, wcsLen);
		//������ '\0'
		szUnicode[wcsLen] = '\0';

		return szUnicode;
	}

	/**
	 *	Functions:	unicode to ansi
	 *	Input:		unicode ��ʽ�ַ�
	 *	Output:		null
	 *	Return:		Ansi ��ʽ�ַ�
	 *
	 *	Create	@jianyaoy	[2012-9-4 22:51]	
	 */
	inline  char * UnicodeToAnsi(const wchar_t* szUnicode)
	{
		//Ԥת�����õ�����ռ�Ĵ�С
		int ansiLen = ::WideCharToMultiByte(CP_ACP, NULL, szUnicode, wcslen(szUnicode), NULL, 0, NULL, NULL);
		//����ռ�Ҫ��'\0'�����ռ�
		char* szAnsi = new char[ansiLen + 1];
		//ת��
		//unicode���Ӧ��strlen��wcslen
		::WideCharToMultiByte(CP_ACP, NULL, szUnicode, wcslen(szUnicode), szAnsi, ansiLen, NULL, NULL);
		//������'\0'
		szAnsi[ansiLen] = '\0';

		return szAnsi;
	}

	/**
	 *	Functions:	utf8 to unicode
	 *	Input:		utf8 ��ʽ�ַ�
	 *	Output:		null
	 *	Return:		unicode ��ʽ�ַ�
	 *
	 *	Create	@jianyaoy	[2012-9-4 22:51]	
	 */
	inline  wchar_t * Utf8ToUnicode(const char* szUtf)
	{
		//Ԥת�����õ�����ռ�Ĵ�С
		int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szUtf, strlen(szUtf), NULL, 0);
		//����ռ�Ҫ��'\0'�����ռ䣬MultiByteToWideChar�����'\0'�ռ�
		wchar_t* szUnicode = new wchar_t[wcsLen + 1];
		//ת��
		::MultiByteToWideChar(CP_UTF8, NULL, szUtf, strlen(szUtf), szUnicode, wcsLen);
		//������'\0'
		szUnicode[wcsLen] = '\0';

		return szUnicode;
	}

	/**
	 *	Functions:	unicode to utf8
	 *	Input:		unicode��ʽ �ַ�
	 *	Output:		null
	 *	Return:		utf8 ��ʽ�ַ�
	 *
	 *	Create	@jianyaoy	[2012-9-4 22:50]	
	 */
	inline char* UnicodeToUtf8(const wchar_t* szUnicode)
	{
		//Ԥת�����õ�����ռ�Ĵ�С������õĺ��������������෴
		int u8Len = ::WideCharToMultiByte(CP_UTF8, NULL, szUnicode, wcslen(szUnicode), NULL, 0, NULL, NULL);
		//UTF8��Ȼ��Unicode��ѹ����ʽ����Ҳ�Ƕ��ֽ��ַ��������Կ�����char����ʽ����
		char* szUtf = new char[u8Len + 1];
		//ת��
		//unicode���Ӧ��strlen��wcslen
		::WideCharToMultiByte(CP_UTF8, NULL, szUnicode, wcslen(szUnicode), szUtf, u8Len, NULL, NULL);
		//������'\0'
		szUtf[u8Len] = '\0';

		return szUtf;
	}
}

#endif