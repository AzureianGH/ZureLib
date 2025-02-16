#include "fast_str.h"
#include <string.h>
#include "zl_init.h"
#include <stdlib.h>
#include <stdio.h>
#ifdef ZURELIB_OS_ENABLE_SSE
#include <emmintrin.h>
#endif
#define SIZE_ZL_MAX 0xFFFFFFFF
//function pointer to the correct implementation of zl_qstrlen

ZURELIB_RET(unsigned int) __cdecl zl_qstrlen_memchr(const char* str)
{
    const char* end = (const char*)memchr(str, '\0', SIZE_ZL_MAX);
    return end ? (unsigned int)(end - str) : 0;
}
unsigned int(__cdecl* zl_qstrlen_fp)(const char* str) = zl_qstrlen_memchr;
#ifdef ZURELIB_OS_ENABLE_SSE
ZURELIB_RET(unsigned int) __cdecl zl_qstrlen_sse(const char* str)
{
    __m128i* ptr = (__m128i*)str;
    __m128i zero = _mm_setzero_si128();  // SSE2 zero register
    unsigned int len = 0;

    while (1)
    {
        __m128i data = _mm_loadu_si128(ptr++);
        __m128i cmp = _mm_cmpeq_epi8(data, zero);  // Compare with zero
        int mask = _mm_movemask_epi8(cmp);  // Generate a mask for the match
        if (mask)
        {
            unsigned int idx = __builtin_ctz(mask);  // Find the first zero byte
            len += idx;
            break;
        }
        len += 16;
    }
    return len;
}
#endif

ZURELIB_EXTERN ZURELIB_RET(ZL_BOOL) __cdecl zl_enable_qstr_sse()
{
    zl_qstrlen_fp = zl_qstrlen_sse;
    return ZL_TRUE;
}

ZURELIB_API ZURELIB_RET(unsigned int) __cdecl zl_qstrlen(const char * str)
{
    return zl_qstrlen_fp(str);
}


ZURELIB_RET(unsigned int) __cdecl zl_qget_string_width(const char* str)
{
    return zl_qstrlen(str);
}

ZURELIB_RET(char*) __cdecl zl_qstrcat(const char* str1, const char* str2)
{
    unsigned int len1 = zl_qstrlen(str1);
    unsigned int len2 = zl_qstrlen(str2);
    char* result = (char*)malloc(len1 + len2 + 1);
    memcpy(result, str1, len1);
    memcpy(result + len1, str2, len2);
    result[len1 + len2] = '\0';
    return result;
}

ZURELIB_RET(void) __cdecl zl_qstrcpy(char* dest, const char* src)
{
    unsigned int len = zl_qstrlen(src);
    memcpy(dest, src, len);
    dest[len] = '\0';
}

ZURELIB_RET(char*) __cdecl zl_qitoa(int value)
{
    char buffer[12];
    sprintf(buffer, "%d", value);
    return strdup(buffer);
}

ZURELIB_RET(char*) __cdecl zl_qftoa(float value)
{
    char buffer[32];
    sprintf(buffer, "%f", value);
    return strdup(buffer);
}

ZURELIB_RET(char*) __cdecl zl_qdtoa(double value)
{
    char buffer[32];
    sprintf(buffer, "%f", value);
    return strdup(buffer);
}

ZURELIB_RET(int) __cdecl zl_qatoi(const char* str)
{
    return atoi(str);
}

ZURELIB_RET(float) __cdecl zl_qatof(const char* str)
{
    return atof(str);
}

ZURELIB_RET(double) __cdecl zl_qatod(const char* str)
{
    return atof(str);
}
