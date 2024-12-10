#ifndef ZURELIB_H
#define ZURELIB_H

#include "OS_CORE/zl_init.h"
#include "OS_CORE/fast_str.h"

#if defined(_WIN32) || defined(_WIN64)
#include "OS_WNDS/fast_io.h"
#include "OS_WNDS/fast_window.h"
#endif
#endif // ZURELIB_H