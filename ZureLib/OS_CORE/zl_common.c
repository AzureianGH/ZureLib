#include "zl_common.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
void __cdecl zl_zero(void* ptr, unsigned int size)
{
    memset(ptr, 0, size);
}

void* __cdecl zl_load_file(const char* path, unsigned long long* size)
{
    FILE* file = fopen(path, "rb");
    if (!file) {
        
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    fseek(file, 0, SEEK_SET);

    void* buffer = malloc(*size);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, *size, file);
    fclose(file);

    return buffer;
}

ZL_BOOL __cdecl zl_free_file(void* ptr)
{
    free(ptr);
    return ZL_TRUE;
}

ZL_BOOL __cdecl zl_save_file(const char* path, void* ptr, unsigned long long size)
{
    FILE* file = fopen(path, "wb");
    if (!file) {
        return ZL_FALSE;
    }

    fwrite(ptr, 1, size, file);
    fclose(file);

    return ZL_TRUE;
}

unsigned long long __cdecl zl_get_file_length(const char* path)
{
    FILE* file = fopen(path, "rb");
    if (!file) {
        return 0;
    }

    fseek(file, 0, SEEK_END);
    unsigned long long length = ftell(file);
    fclose(file);

    return length;
}

#include <stdint.h>
unsigned int __cdecl zl_big_endian_to_host32(const unsigned char* data) {
    return (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
}

unsigned short __cdecl zl_big_endian_to_host16(const unsigned char* data) {
    return (data[0] << 8) | data[1];
}
#include <time.h>
double __cdecl zl_get_time()
{
    return (double)clock() / CLOCKS_PER_SEC;
}