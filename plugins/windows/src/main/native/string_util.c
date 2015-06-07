#include "string_util.h"

/* -------------------------------------------------------------
内码转换
------------------------------------------------------------- */

void Convert(const char* strIn, char* strOut, int maxLength, int sourceCodepage, int targetCodepage)
{
	int unicodeLen = MultiByteToWideChar(sourceCodepage, 0, strIn, -1, NULL, 0);
	wchar_t* pUnicode;
	pUnicode = malloc((unicodeLen + 1)*sizeof(wchar_t));
	memset(pUnicode, 0, (unicodeLen + 1)*sizeof(wchar_t));
	MultiByteToWideChar(sourceCodepage, 0, strIn, -1, (LPWSTR)pUnicode, unicodeLen);

	BYTE * pTargetData = NULL;
	int targetLen = WideCharToMultiByte(targetCodepage, 0, (LPWSTR)pUnicode, -1, (char *)pTargetData, 0, NULL, NULL);
	pTargetData = malloc((targetLen + 1) * sizeof(BYTE));
	memset(pTargetData, 0, targetLen + 1);
	WideCharToMultiByte(targetCodepage, 0, (LPWSTR)pUnicode, -1, (char *)pTargetData, targetLen, NULL, NULL);

	lstrcpyn(strOut, (char*)pTargetData, maxLength);

	free(pUnicode);
	free(pTargetData);
}

// Ansi 转 utf-8
void ConvertAnsiToUtf8(const char* strIn, char* strOut, int maxLength)
{
	Convert(strIn, strOut, maxLength, CP_ACP, CP_UTF8);
}

// utf-8 转 Ansi
void ConvertUtf8ToAnsi(const char* strIn, char* strOut, int maxLength)
{
	Convert(strIn, strOut, maxLength, CP_UTF8, CP_ACP);
}
