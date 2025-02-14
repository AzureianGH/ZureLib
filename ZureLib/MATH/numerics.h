#pragma once

#include "../OS_CORE/calling_conv_ret.h"

typedef struct
{
    float x;
    float y;
} zvector2f;

typedef struct
{
    float x;
    float y;
    float z;
} zvector3f;

typedef struct
{
    float x;
    float y;
    float z;
    float w;
} zvector4f;

typedef struct
{
    double x;
    double y;
} zvector2d;

typedef struct
{
    double x;
    double y;
    double z;
} zvector3d;

typedef struct
{
    double x;
    double y;
    double z;
    double w;
} zvector4d;

typedef struct
{
    int x;
    int y;
} zvector2i;

typedef struct
{
    int x;
    int y;
    int z;
} zvector3i;

typedef struct
{
    int x;
    int y;
    int z;
    int w;
} zvector4i;

typedef struct
{
    float m[4][4];
} zmatrix4f;

typedef struct
{
    double m[4][4];
} zmatrix4d;

typedef struct
{
    int m[4][4];
} zmatrix4i;

typedef struct
{
    float x;
    float y;
    float z;
    float w;
} zquaternion;

typedef struct
{
    float r;
    float g;
    float b;
    float a;
} zcolorf;

typedef struct
{
    int r;
    int g;
    int b;
    int a;
} zcolori;

typedef struct
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} zcoloruc;

typedef struct
{
    float x;
    float y;
    float width;
    float height;
} zrectf;

typedef struct
{
    double x;
    double y;
    double width;
    double height;
} zrectd;

typedef struct
{
    int x;
    int y;
    int width;
    int height;
} zrecti;
#pragma region Vector2f

ZURELIB_API ZURELIB_RET(zvector2f*) __cdecl zl_vect2add(zvector2f* v1, zvector2f* v2);

ZURELIB_API ZURELIB_RET(zvector2f*) __cdecl zl_vect2sub(zvector2f* v1, zvector2f* v2);

ZURELIB_API ZURELIB_RET(zvector2f*) __cdecl zl_vect2mul(zvector2f* v1, zvector2f* v2);

ZURELIB_API ZURELIB_RET(zvector2f*) __cdecl zl_vect2div(zvector2f* v1, zvector2f* v2);

ZURELIB_API ZURELIB_RET(zvector2f*) __cdecl zl_vect2create(float x, float y);

ZURELIB_API ZURELIB_RET(zvector2f*) __cdecl zl_vect2normalize(zvector2f* v);

#pragma endregion

#pragma region Vector3f

ZURELIB_API ZURELIB_RET(zvector3f*) __cdecl zl_vect3add(zvector3f* v1, zvector3f* v2);

ZURELIB_API ZURELIB_RET(zvector3f*) __cdecl zl_vect3sub(zvector3f* v1, zvector3f* v2);

ZURELIB_API ZURELIB_RET(zvector3f*) __cdecl zl_vect3mul(zvector3f* v1, zvector3f* v2);

ZURELIB_API ZURELIB_RET(zvector3f*) __cdecl zl_vect3div(zvector3f* v1, zvector3f* v2);

ZURELIB_API ZURELIB_RET(zvector3f*) __cdecl zl_vect3create(float x, float y, float z);

ZURELIB_API ZURELIB_RET(zvector3f*) __cdecl zl_vect3normalize(zvector3f* v);

#pragma endregion

#pragma region Vector4f

ZURELIB_API ZURELIB_RET(zvector4f*) __cdecl zl_vect4add(zvector4f* v1, zvector4f* v2);

ZURELIB_API ZURELIB_RET(zvector4f*) __cdecl zl_vect4sub(zvector4f* v1, zvector4f* v2);

ZURELIB_API ZURELIB_RET(zvector4f*) __cdecl zl_vect4mul(zvector4f* v1, zvector4f* v2);

ZURELIB_API ZURELIB_RET(zvector4f*) __cdecl zl_vect4div(zvector4f* v1, zvector4f* v2);

ZURELIB_API ZURELIB_RET(zvector4f*) __cdecl zl_vect4create(float x, float y, float z, float w);

ZURELIB_API ZURELIB_RET(zvector4f*) __cdecl zl_vect4normalize(zvector4f* v);

#pragma endregion

#pragma region Vector2d

ZURELIB_API ZURELIB_RET(zvector2d*) __cdecl zl_vect2add_d(zvector2d* v1, zvector2d* v2);

ZURELIB_API ZURELIB_RET(zvector2d*) __cdecl zl_vect2sub_d(zvector2d* v1, zvector2d* v2);

ZURELIB_API ZURELIB_RET(zvector2d*) __cdecl zl_vect2mul_d(zvector2d* v1, zvector2d* v2);

ZURELIB_API ZURELIB_RET(zvector2d*) __cdecl zl_vect2div_d(zvector2d* v1, zvector2d* v2);

ZURELIB_API ZURELIB_RET(zvector2d*) __cdecl zl_vect2create_d(double x, double y);

ZURELIB_API ZURELIB_RET(zvector2d*) __cdecl zl_vect2normalize_d(zvector2d* v);

#pragma endregion

#pragma region Vector3d

ZURELIB_API ZURELIB_RET(zvector3d*) __cdecl zl_vect3add_d(zvector3d* v1, zvector3d* v2);

ZURELIB_API ZURELIB_RET(zvector3d*) __cdecl zl_vect3sub_d(zvector3d* v1, zvector3d* v2);

ZURELIB_API ZURELIB_RET(zvector3d*) __cdecl zl_vect3mul_d(zvector3d* v1, zvector3d* v2);

ZURELIB_API ZURELIB_RET(zvector3d*) __cdecl zl_vect3div_d(zvector3d* v1, zvector3d* v2);

ZURELIB_API ZURELIB_RET(zvector3d*) __cdecl zl_vect3create_d(double x, double y, double z);

ZURELIB_API ZURELIB_RET(zvector3d*) __cdecl zl_vect3normalize_d(zvector3d* v);

#pragma endregion

#pragma region Vector4d

ZURELIB_API ZURELIB_RET(zvector4d*) __cdecl zl_vect4add_d(zvector4d* v1, zvector4d* v2);

ZURELIB_API ZURELIB_RET(zvector4d*) __cdecl zl_vect4sub_d(zvector4d* v1, zvector4d* v2);

ZURELIB_API ZURELIB_RET(zvector4d*) __cdecl zl_vect4mul_d(zvector4d* v1, zvector4d* v2);

ZURELIB_API ZURELIB_RET(zvector4d*) __cdecl zl_vect4div_d(zvector4d* v1, zvector4d* v2);

ZURELIB_API ZURELIB_RET(zvector4d*) __cdecl zl_vect4create_d(double x, double y, double z, double w);

ZURELIB_API ZURELIB_RET(zvector4d*) __cdecl zl_vect4normalize_d(zvector4d* v);

#pragma endregion

#pragma region Vector2i

ZURELIB_API ZURELIB_RET(zvector2i*) __cdecl zl_vect2add_i(zvector2i* v1, zvector2i* v2);

ZURELIB_API ZURELIB_RET(zvector2i*) __cdecl zl_vect2sub_i(zvector2i* v1, zvector2i* v2);

ZURELIB_API ZURELIB_RET(zvector2i*) __cdecl zl_vect2mul_i(zvector2i* v1, zvector2i* v2);

ZURELIB_API ZURELIB_RET(zvector2i*) __cdecl zl_vect2div_i(zvector2i* v1, zvector2i* v2);

ZURELIB_API ZURELIB_RET(zvector2i*) __cdecl zl_vect2create_i(int x, int y);

ZURELIB_API ZURELIB_RET(zvector2i*) __cdecl zl_vect2normalize_i(zvector2i* v);

#pragma endregion

#pragma region Vector3i

ZURELIB_API ZURELIB_RET(zvector3i*) __cdecl zl_vect3add_i(zvector3i* v1, zvector3i* v2);

ZURELIB_API ZURELIB_RET(zvector3i*) __cdecl zl_vect3sub_i(zvector3i* v1, zvector3i* v2);

ZURELIB_API ZURELIB_RET(zvector3i*) __cdecl zl_vect3mul_i(zvector3i* v1, zvector3i* v2);

ZURELIB_API ZURELIB_RET(zvector3i*) __cdecl zl_vect3div_i(zvector3i* v1, zvector3i* v2);

ZURELIB_API ZURELIB_RET(zvector3i*) __cdecl zl_vect3create_i(int x, int y, int z);

ZURELIB_API ZURELIB_RET(zvector3i*) __cdecl zl_vect3normalize_i(zvector3i* v);

#pragma endregion

#pragma region Vector4i

ZURELIB_API ZURELIB_RET(zvector4i*) __cdecl zl_vect4add_i(zvector4i* v1, zvector4i* v2);

ZURELIB_API ZURELIB_RET(zvector4i*) __cdecl zl_vect4sub_i(zvector4i* v1, zvector4i* v2);

ZURELIB_API ZURELIB_RET(zvector4i*) __cdecl zl_vect4mul_i(zvector4i* v1, zvector4i* v2);

ZURELIB_API ZURELIB_RET(zvector4i*) __cdecl zl_vect4div_i(zvector4i* v1, zvector4i* v2);

ZURELIB_API ZURELIB_RET(zvector4i*) __cdecl zl_vect4create_i(int x, int y, int z, int w);

ZURELIB_API ZURELIB_RET(zvector4i*) __cdecl zl_vect4normalize_i(zvector4i* v);

#pragma endregion

#pragma region Matrix4f

ZURELIB_API ZURELIB_RET(zmatrix4f*) __cdecl zl_mat4add(zmatrix4f* m1, zmatrix4f* m2);

ZURELIB_API ZURELIB_RET(zmatrix4f*) __cdecl zl_mat4sub(zmatrix4f* m1, zmatrix4f* m2);

ZURELIB_API ZURELIB_RET(zmatrix4f*) __cdecl zl_mat4mul(zmatrix4f* m1, zmatrix4f* m2);

ZURELIB_API ZURELIB_RET(zmatrix4f*) __cdecl zl_mat4div(zmatrix4f* m1, zmatrix4f* m2);

ZURELIB_API ZURELIB_RET(zmatrix4f*) __cdecl zl_mat4create(float m[4][4]);

ZURELIB_API ZURELIB_RET(zmatrix4f*) __cdecl zl_mat4normalize(zmatrix4f* m);

#pragma endregion

#pragma region Matrix4d

ZURELIB_API ZURELIB_RET(zmatrix4d*) __cdecl zl_mat4add_d(zmatrix4d* m1, zmatrix4d* m2);

ZURELIB_API ZURELIB_RET(zmatrix4d*) __cdecl zl_mat4sub_d(zmatrix4d* m1, zmatrix4d* m2);

ZURELIB_API ZURELIB_RET(zmatrix4d*) __cdecl zl_mat4mul_d(zmatrix4d* m1, zmatrix4d* m2);

ZURELIB_API ZURELIB_RET(zmatrix4d*) __cdecl zl_mat4div_d(zmatrix4d* m1, zmatrix4d* m2);

ZURELIB_API ZURELIB_RET(zmatrix4d*) __cdecl zl_mat4create_d(double m[4][4]);

ZURELIB_API ZURELIB_RET(zmatrix4d*) __cdecl zl_mat4normalize_d(zmatrix4d* m);

#pragma endregion

#pragma region Matrix4i

ZURELIB_API ZURELIB_RET(zmatrix4i*) __cdecl zl_mat4add_i(zmatrix4i* m1, zmatrix4i* m2);

ZURELIB_API ZURELIB_RET(zmatrix4i*) __cdecl zl_mat4sub_i(zmatrix4i* m1, zmatrix4i* m2);

ZURELIB_API ZURELIB_RET(zmatrix4i*) __cdecl zl_mat4mul_i(zmatrix4i* m1, zmatrix4i* m2);

ZURELIB_API ZURELIB_RET(zmatrix4i*) __cdecl zl_mat4div_i(zmatrix4i* m1, zmatrix4i* m2);

ZURELIB_API ZURELIB_RET(zmatrix4i*) __cdecl zl_mat4create_i(int m[4][4]);

ZURELIB_API ZURELIB_RET(zmatrix4i*) __cdecl zl_mat4normalize_i(zmatrix4i* m);

#pragma endregion

#pragma region Quaternion

ZURELIB_API ZURELIB_RET(zquaternion*) __cdecl zl_quatadd(zquaternion* q1, zquaternion* q2);

ZURELIB_API ZURELIB_RET(zquaternion*) __cdecl zl_quatsub(zquaternion* q1, zquaternion* q2);

ZURELIB_API ZURELIB_RET(zquaternion*) __cdecl zl_quatmul(zquaternion* q1, zquaternion* q2);

ZURELIB_API ZURELIB_RET(zquaternion*) __cdecl zl_quatdiv(zquaternion* q1, zquaternion* q2);

ZURELIB_API ZURELIB_RET(zquaternion*) __cdecl zl_quatcreate(float x, float y, float z, float w);

ZURELIB_API ZURELIB_RET(zquaternion*) __cdecl zl_quatnormalize(zquaternion* q);

#pragma endregion