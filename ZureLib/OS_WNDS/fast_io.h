#ifndef ZURELIB_OS_WNDS_FAST_IO_H
#define ZURELIB_OS_WNDS_FAST_IO_H
#include "../OS_CORE/calling_conv_ret.h"
/// @brief Quick-write a string to the console
ZURELIB_API ZURELIB_RET(void) __cdecl zl_qwrite(const char* str);
/// @brief Quick-write a string to the console (newline)
ZURELIB_API ZURELIB_RET(void) __cdecl zl_qwriteln(const char* str);
/// @brief Quick-read a string from the console
ZURELIB_API ZURELIB_RET(void) __cdecl zl_qread(char* str, void* buffer, unsigned int size);
/// @brief Sleep for a specified number of milliseconds
ZURELIB_API ZURELIB_RET(void) __cdecl zl_qsleep(unsigned int ms);
#endif // ZURELIB_OS_WNDS_FAST_IO_H