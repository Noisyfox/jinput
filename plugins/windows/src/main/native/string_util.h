#ifndef _STRINGUTIL_H
#define _STRINGUTIL_H

#include <windows.h>

/* -------------------------------------------------------------
内码转换
------------------------------------------------------------- */

// 转换UCS4编码到UTF8编码
INT UCS4_To_UTF8(DWORD dwUCS4, BYTE* pbUTF8);

// 转换UTF8编码到UCS4编码
INT UTF8_To_UCS4(const BYTE* pbUTF8, DWORD* dwUCS4);

// 转换UCS4编码到UCS2编码
INT UCS4_To_UTF16(DWORD dwUCS4, WORD* pwUTF16);

// 转换UCS2编码到UCS4编码
INT UTF16_To_UCS4(const WORD* pwUTF16, DWORD* dwUCS4);

// 转换UTF8字符串到UTF16字符串
INT UTF8Str_To_UTF16Str(const BYTE* pbszUTF8Str, WORD* pwszUTF16Str);

// 转换UTF16字符串到UTF8字符串
INT UTF16Str_To_UTF8Str(const WORD* pwszUTF16Str, BYTE* pbszUTF8Str);

#endif
