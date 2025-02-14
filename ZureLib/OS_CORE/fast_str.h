#ifndef ZURELIB_OS_CORE_FAST_STR_H
#define ZURELIB_OS_CORE_FAST_STR_H
#include "calling_conv_ret.h"
#include "zl_common.h"
/// @brief Quick-get the length of a string
ZURELIB_API ZURELIB_RET(unsigned int) __cdecl zl_qstrlen(const char* str);
/// @brief Quick-get the length of a string using memchr
ZURELIB_API ZURELIB_RET(unsigned int) __cdecl zl_qget_string_width(const char* str);
/// @brief Quick-concatenate two strings
ZURELIB_API ZURELIB_RET(char*) __cdecl zl_qstrcat(const char* str1, const char* str2);
/// @brief Quick-Copy a string
ZURELIB_API ZURELIB_RET(void) __cdecl zl_qstrcpy(char* dest, const char* src);
/// @brief Quick-shift a integer to a string
ZURELIB_API ZURELIB_RET(char*) __cdecl zl_qitoa(int value);
/// @brief Quick-shift a float to a string
ZURELIB_API ZURELIB_RET(char*) __cdecl zl_qftoa(float value);
/// @brief Quick-shift a double to a string
ZURELIB_API ZURELIB_RET(char*) __cdecl zl_qdtoa(double value);
/// @brief Quick-shift a string to a integer
ZURELIB_API ZURELIB_RET(int) __cdecl zl_qatoi(const char* str);
/// @brief Quick-shift a string to a float
ZURELIB_RET(float) __cdecl zl_qatof(const char* str);
/// @brief Quick-shift a string to a double
ZURELIB_RET(double) __cdecl zl_qatod(const char* str);
#endif // ZURELIB_OS_CORE_FAST_STR_H