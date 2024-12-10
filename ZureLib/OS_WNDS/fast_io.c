#include "fast_io.h"
#include "../OS_CORE/fast_str.h"
#include <string.h>
#include <windows.h>

void zl_qwrite(const char* str)
{
    // Get length of string
    unsigned int len = zl_qstrlen(str);

    // Get the console output handle
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // use syscalls
    DWORD written;
    WriteConsoleA(hStdOut, str, len, &written, NULL);
}

ZURELIB_RET(void) __cdecl zl_qwriteln(const char* str)
{
    zl_qwrite(str);          // Write the string
    zl_qwrite("\n");         // Write a newline character
}

ZURELIB_RET(void) __cdecl zl_qread(char* str, void* buffer, unsigned int size)
{
    // Get the console input handle
    HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);

    // use syscalls
    DWORD read;
    ReadConsoleA(hStdIn, buffer, size, &read, NULL);
    memcpy(str, buffer, size);
}

ZURELIB_RET(void) __cdecl zl_qsleep(unsigned int ms)
{
    Sleep(ms);
}