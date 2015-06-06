#ifndef _STRINGUTIL_H
#define _STRINGUTIL_H

#include <windows.h>

/* -------------------------------------------------------------
内码转换
------------------------------------------------------------- */

// Ansi 转 utf-8
void ConvertAnsiToUtf8(const char* strIn, char* strOut);

// utf-8 转 Ansi
void ConvertUtf8ToAnsi(const char* strIn, char* strOut);

#endif
