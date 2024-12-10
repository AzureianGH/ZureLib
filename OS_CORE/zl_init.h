#ifndef ZURELIB_OS_CORE_INIT_H
#define ZURELIB_OS_CORE_INIT_H
#define ZURELIB_OS_ENABLE_SSE
#include "calling_conv_ret.h"
#include "../zl_types.h"

/// @brief Initialize the ZureLib library
ZURELIB_RET(void) __cdecl zl_init(ZL_BOOL Vocal);

/// @brief Deinitialize the ZureLib library
ZURELIB_RET(void) __cdecl zl_deinit();

#endif // ZURELIB_OS_CORE_INIT_H