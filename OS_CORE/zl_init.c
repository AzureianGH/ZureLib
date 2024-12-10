#include "zl_init.h"
//check if windows
#if defined(_WIN32) || defined(_WIN64)
#include "../OS_WNDS/fast_io.h"
#endif
ZL_BOOL ZL_SUPPORTS_SSE = ZL_FALSE;

ZURELIB_EXTERN ZURELIB_RET(ZL_BOOL) __cdecl zl_enable_qstr_sse();

ZURELIB_RET(ZL_STATUS) __cdecl zl_check_supports_sse_()
{
    ZL_STATUS status = ZL_SUCCESS;
    //use cpuid to check if the processor supports sse
    asm volatile(
        "mov $1, %%eax\n"
        "cpuid\n"
        "test $0x2000000, %%edx\n"
        "jz .no_sse\n"
        "mov $1, %0\n"
        "jmp .end\n"
        ".no_sse:\n"
        "mov $0, %0\n"
        ".end:\n"
        : "=r"(ZL_SUPPORTS_SSE)
        :
        : "%eax", "%edx");
    //check if the processor supports sse
    if (ZL_SUPPORTS_SSE == ZL_FALSE)
    {
        status = ZL_NOT_SUPPORTED;
    }
    return status;
}

ZURELIB_RET(void) __cdecl zl_init(ZL_BOOL Vocal)
{
    //check if the processor supports sse
    if (zl_check_supports_sse_() == ZL_SUCCESS)
    {
        zl_enable_qstr_sse();
    }
    if (Vocal)
    {
        #if defined(_WIN32) || defined(_WIN64)
        zl_qwriteln("ZureLib initialized.");
        //print if sse is supported
        if (ZL_SUPPORTS_SSE)
        {
            zl_qwriteln("SSE is supported.");
        }
        else
        {
            zl_qwriteln("SSE is not supported.");
        }
        
        #endif
    }
}



ZURELIB_RET(void) __cdecl zl_deinit()
{
    // Do nothing for now
}