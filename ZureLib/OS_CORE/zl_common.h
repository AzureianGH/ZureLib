#ifndef ZURELIB_OS_CORE_ZL_COMMON_H
#define ZURELIB_OS_CORE_ZL_COMMON_H

#include "../zl_types.h"
#include "calling_conv_ret.h"

typedef enum
{
    ZL_COLOR_WHITE = 0xFFFFFF,
    ZL_COLOR_BLACK = 0x000000,
    ZL_COLOR_RED = 0xFF0000,
    ZL_COLOR_GREEN = 0x00FF00,
    ZL_COLOR_BLUE = 0x0000FF,
    ZL_COLOR_YELLOW = 0xFFFF00,
    ZL_COLOR_CYAN = 0x00FFFF,
    ZL_COLOR_MAGENTA = 0xFF00FF
} ZL_COLOR;



#define ZL_FILE_HANDLE void*

/// @brief Zero-out a block of memory
ZURELIB_API ZURELIB_RET(void) __cdecl zl_zero(void* ptr, unsigned int size);

/// @brief Load a file into memory
ZURELIB_API ZURELIB_RET(void*) __cdecl zl_load_file(const char* path, unsigned long long* size);

/// @brief Free memory allocated by zl_load_file
ZURELIB_API ZURELIB_RET(ZL_BOOL) __cdecl zl_free_file(void* ptr);

/// @brief Save a block of memory to a file
ZURELIB_API ZURELIB_RET(ZL_BOOL) __cdecl zl_save_file(const char* path, void* ptr, unsigned long long size);

/// @brief Get the length of a file
ZURELIB_API ZURELIB_RET(unsigned long long) __cdecl zl_get_file_length(const char* path);

/// @brief Convert Big Endian to Host 32
ZURELIB_API ZURELIB_RET(unsigned int) __cdecl zl_big_endian_to_host32(const unsigned char* data);

/// @brief Convert Big Endian to Host 16
ZURELIB_API ZURELIB_RET(unsigned short) __cdecl zl_big_endian_to_host16(const unsigned char* data);

/// @brief Get the current time (double)
ZURELIB_API ZURELIB_RET(double) __cdecl zl_get_time();

#endif // ZURELIB_OS_CORE_ZL_COMMON_H