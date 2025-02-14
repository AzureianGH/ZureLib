#include "zl_common.h"
#include "fast_str.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#ifdef _WIN32
#include <Windows.h>
#include <direct.h>
#elif __linux__
#include <unistd.h>
#endif

void __cdecl zl_zero(void* ptr, unsigned int size)
{
    memset(ptr, 0, size);
}

void* __cdecl zl_load_file(const char* path, unsigned long long size)
{
    FILE* file = fopen(path, "rb");
    if (!file) {
        
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);

    void* buffer = malloc(size);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, size, file);
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

ZL_BOOL __cdecl zl_file_exists(const char* path)
{
    FILE* file = fopen(path, "rb");
    if (!file) {
        return ZL_FALSE;
    }

    fclose(file);
    return ZL_TRUE;
}

char* __cdecl zl_get_cwd()
{
    #ifdef _WIN32
    
    char* buffer = (char*)malloc(512);
    GetCurrentDirectoryA(512, buffer);
    return buffer;
    #elif __linux__
    char* buffer = (char*)malloc(512);
    getcwd(buffer, 512);
    return buffer;
    #endif
}

char* __cdecl zl_get_executable_path()
{
    #ifdef _WIN32
    char* buffer = (char*)malloc(512);
    GetModuleFileNameA(NULL, buffer, 512);
    return buffer;
    #elif __linux__
    char* buffer = (char*)malloc(512);
    readlink("/proc/self/exe", buffer, 512);
    return buffer;
    #endif
}

char* __cdecl zl_get_executable_name()
{
    char* path = zl_get_executable_path();
    char* name = strrchr(path, '/');
    if (!name) {
        name = strrchr(path, '\\');
    }
    if (!name) {
        return NULL;
    }
    return name + 1;
}

char* __cdecl zl_get_executable_directory()
{
    char* path = zl_get_executable_path();
    char* name = strrchr(path, '/');
    if (!name) {
        name = strrchr(path, '\\');
    }
    if (!name) {
        return NULL;
    }
    *name = '\0';
    return path;
}

char* __cdecl zl_local_to_global_path(const char* localPath)
{
    char* cwd = zl_get_executable_directory();
    char* result = zl_qstrcat(cwd, "\\");
    result = zl_qstrcat(result, localPath);
    free(cwd);
    return result;
}

unsigned long long __cdecl zl_get_file_length(const char* path)
{
    FILE* file = fopen(path, "rb");
    if (!file) {
        zl_error("Failed to open file.");
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

void __cdecl zl_error(const char* message)
{
    fprintf(stderr, "Error: %s\n", message);
}

void __cdecl zl_fatal(const char* message, int code)
{
    fprintf(stderr, "Fatal Error: %s\n", message);
    exit(code);
}