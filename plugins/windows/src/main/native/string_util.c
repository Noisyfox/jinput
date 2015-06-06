#include "string_util.h"

/* -------------------------------------------------------------
内码转换
------------------------------------------------------------- */

// 转换UCS4编码到UTF8编码
INT UCS4_To_UTF8(DWORD dwUCS4, BYTE* pbUTF8)
{
	const BYTE	abPrefix[] = { 0, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };
	const DWORD adwCodeUp[] = {
		0x80,			// U+00000000 ～ U+0000007F
		0x800,			// U+00000080 ～ U+000007FF
		0x10000,		// U+00000800 ～ U+0000FFFF
		0x200000,		// U+00010000 ～ U+001FFFFF
		0x4000000,		// U+00200000 ～ U+03FFFFFF
		0x80000000		// U+04000000 ～ U+7FFFFFFF
	};

	INT	i, iLen;

	// 根据UCS4编码范围确定对应的UTF-8编码字节数
	iLen = sizeof(adwCodeUp) / sizeof(DWORD);
	for (i = 0; i < iLen; i++)
	{
		if (dwUCS4 < adwCodeUp[i])
		{
			break;
		}
	}

	if (i == iLen)return 0;	// 无效的UCS4编码

	iLen = i + 1;	// UTF-8编码字节数
	if (pbUTF8 != NULL)
	{	// 转换为UTF-8编码
		for (; i > 0; i--)
		{
			pbUTF8[i] = (BYTE)((dwUCS4 & 0x3F) | 0x80);
			dwUCS4 >>= 6;
		}

		pbUTF8[0] = (BYTE)(dwUCS4 | abPrefix[iLen - 1]);
	}

	return iLen;
}

// 转换UTF8编码到UCS4编码
INT UTF8_To_UCS4(const BYTE* pbUTF8, DWORD* dwUCS4)
{
	INT		i, iLen;
	BYTE	b;

	if (pbUTF8 == NULL)
	{	// 参数错误
		return 0;
	}

	b = *pbUTF8++;
	if (b < 0x80)
	{
		*dwUCS4 = b;
		return 1;
	}

	if (b < 0xC0 || b > 0xFD)
	{	// 非法UTF8
		return 0;
	}

	if (b < 0xE0)
	{
		*dwUCS4 = b & 0x1F;
		iLen = 2;
	}
	else if (b < 0xF0)
	{
		*dwUCS4 = b & 0x0F;
		iLen = 3;
	}
	else if (b < 0xF8)
	{
		*dwUCS4 = b & 7;
		iLen = 4;
	}
	else if (b < 0xFC)
	{
		*dwUCS4 = b & 3;
		iLen = 5;
	}
	else
	{
		*dwUCS4 = b & 1;
		iLen = 6;
	}

	for (i = 1; i < iLen; i++)
	{
		b = *pbUTF8++;
		if (b < 0x80 || b > 0xBF)
		{	// 非法UTF8
			break;
		}

		*dwUCS4 = (*dwUCS4 << 6) + (b & 0x3F);
	}

	if (i < iLen)
	{	// 非法UTF8
		return 0;
	}
	else
	{
		return iLen;
	}
}

// 转换UCS4编码到UCS2编码
INT UCS4_To_UTF16(DWORD dwUCS4, WORD* pwUTF16)
{
	if (dwUCS4 <= 0xFFFF)
	{
		if (pwUTF16 != NULL)
		{
			*pwUTF16 = (WORD)(dwUCS4);
		}

		return 1;
	}
	else if (dwUCS4 <= 0xEFFFF)
	{
		if (pwUTF16 != NULL)
		{
			pwUTF16[0] = (WORD)(0xD800 + (dwUCS4 >> 10) - 0x40);	// 高10位
			pwUTF16[1] = (WORD)(0xDC00 + (dwUCS4 & 0x03FF));		// 低10位
		}

		return 2;
	}
	else
	{
		return 0;
	}
}

// 转换UCS2编码到UCS4编码
INT UTF16_To_UCS4(const WORD* pwUTF16, DWORD* dwUCS4)
{
	WORD	w1, w2;

	if (pwUTF16 == NULL)
	{	// 参数错误
		return 0;
	}

	w1 = pwUTF16[0];
	if (w1 >= 0xD800 && w1 <= 0xDFFF)
	{	// 编码在替代区域（Surrogate Area）
		if (w1 < 0xDC00)
		{
			w2 = pwUTF16[1];
			if (w2 >= 0xDC00 && w2 <= 0xDFFF)
			{
				*dwUCS4 = (w2 & 0x03FF) + (((w1 & 0x03FF) + 0x40) << 10);
				return 2;
			}
		}

		return 0;	// 非法UTF16编码	
	}
	else
	{
		*dwUCS4 = w1;
		return 1;
	}
}

// 转换UTF8字符串到UTF16字符串
INT UTF8Str_To_UTF16Str(const BYTE* pbszUTF8Str, WORD* pwszUTF16Str)
{
	INT		iNum, iLen;
	DWORD	dwUCS4;

	if (pbszUTF8Str == NULL)
	{	// 参数错误
		return 0;
	}

	iNum = 0;	// 统计有效字符个数
	while (*pbszUTF8Str)
	{	// UTF8编码转换为UCS4编码
		iLen = UTF8_To_UCS4(pbszUTF8Str, &dwUCS4);
		if (iLen == 0)
		{	// 非法的UTF8编码
			return 0;
		}

		pbszUTF8Str += iLen;

		// UCS4编码转换为UTF16编码
		iLen = UCS4_To_UTF16(dwUCS4, pwszUTF16Str);
		if (iLen == 0)
		{
			return 0;
		}

		if (pwszUTF16Str != NULL)
		{
			pwszUTF16Str += iLen;
		}

		iNum += iLen;
	}

	if (pwszUTF16Str != NULL)
	{
		*pwszUTF16Str = 0;	// 写入字符串结束标记
	}

	return iNum;
}

// 转换UTF16字符串到UTF8字符串
INT UTF16Str_To_UTF8Str(const WORD* pwszUTF16Str, BYTE* pbszUTF8Str)
{
	INT		iNum, iLen;
	DWORD	dwUCS4;

	if (pwszUTF16Str == NULL)
	{	// 参数错误
		return 0;
	}

	iNum = 0;
	while (*pwszUTF16Str)
	{	// UTF16编码转换为UCS4编码
		iLen = UTF16_To_UCS4(pwszUTF16Str, &dwUCS4);
		if (iLen == 0)
		{	// 非法的UTF16编码
			return 0;
		}

		pwszUTF16Str += iLen;

		// UCS4编码转换为UTF8编码
		iLen = UCS4_To_UTF8(dwUCS4, pbszUTF8Str);
		if (iLen == 0)
		{
			return 0;
		}

		if (pbszUTF8Str != NULL)
		{
			pbszUTF8Str += iLen;
		}

		iNum += iLen;
	}

	if (pbszUTF8Str != NULL)
	{
		*pbszUTF8Str = 0;	// 写入字符串结束标记
	}

	return iNum;
}
