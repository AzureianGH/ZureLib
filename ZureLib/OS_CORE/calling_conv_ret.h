#ifndef ZURELIB_OS_CORE_CALLING_CONV_RET_H
#define ZURELIB_OS_CORE_CALLING_CONV_RET_H

/// @brief Used to hard-declare the return type of a function
#define ZURELIB_RET(x) x
#define ZURELIB_DLL_EXPORT
#define ZURELIB_EXTERN extern
#ifdef _WIN32
    #ifdef ZURELIB_DLL_EXPORT
        #define ZURELIB_API __declspec(dllexport)
    #else
        #define ZURELIB_API __declspec(dllimport)
    #endif
#else
    #define ZURELIB_API
#endif
#endif // ZURELIB_OS_CORE_CALLING_CONV_RET_H