#ifndef ZURELIB_OS_CORE_FAST_STR_H
#define ZURELIB_OS_CORE_FAST_STR_H
#include "calling_conv_ret.h"
#include "zl_common.h"
/// @brief Quick-get the length of a string
ZURELIB_API ZURELIB_RET(unsigned int) __cdecl zl_qstrlen(const char* str);
/// @brief Quick-get the length of a string using memchr
ZURELIB_API ZURELIB_RET(unsigned int) __cdecl zl_qget_string_width(const char* str);
#endif // ZURELIB_OS_CORE_FAST_STR_H