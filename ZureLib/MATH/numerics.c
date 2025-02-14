#include "numerics.h"
#include <malloc.h>
#include <math.h>

#pragma region Vector2f

zvector2f* __cdecl zl_vect2add(zvector2f* v1, zvector2f* v2)
{
    zvector2f* result = (zvector2f*)malloc(sizeof(zvector2f));
    result->x = v1->x + v2->x;
    result->y = v1->y + v2->y;
    return result;
}

zvector2f* __cdecl zl_vect2sub(zvector2f* v1, zvector2f* v2)
{
    zvector2f* result = (zvector2f*)malloc(sizeof(zvector2f));
    result->x = v1->x - v2->x;
    result->y = v1->y - v2->y;
    return result;
}

zvector2f* __cdecl zl_vect2mul(zvector2f* v1, zvector2f* v2)
{
    zvector2f* result = (zvector2f*)malloc(sizeof(zvector2f));
    result->x = v1->x * v2->x;
    result->y = v1->y * v2->y;
    return result;
}

zvector2f* __cdecl zl_vect2div(zvector2f* v1, zvector2f* v2)
{
    zvector2f* result = (zvector2f*)malloc(sizeof(zvector2f));
    result->x = v1->x / v2->x;
    result->y = v1->y / v2->y;
    return result;
}

zvector2f* __cdecl zl_vect2create(float x, float y)
{
    zvector2f* result = (zvector2f*)malloc(sizeof(zvector2f));
    result->x = x;
    result->y = y;
    return result;
}

zvector2f* __cdecl zl_vect2normalize(zvector2f* v)
{
    float length = sqrtf(v->x * v->x + v->y * v->y);
    zvector2f* result = (zvector2f*)malloc(sizeof(zvector2f));
    result->x = v->x / length;
    result->y = v->y / length;
    return result;
}

#pragma endregion

#pragma region Vector3f

zvector3f* __cdecl zl_vect3add(zvector3f* v1, zvector3f* v2)
{
    zvector3f* result = (zvector3f*)malloc(sizeof(zvector3f));
    result->x = v1->x + v2->x;
    result->y = v1->y + v2->y;
    result->z = v1->z + v2->z;
    return result;
}

zvector3f* __cdecl zl_vect3sub(zvector3f* v1, zvector3f* v2)
{
    zvector3f* result = (zvector3f*)malloc(sizeof(zvector3f));
    result->x = v1->x - v2->x;
    result->y = v1->y - v2->y;
    result->z = v1->z - v2->z;
    return result;
}

zvector3f* __cdecl zl_vect3mul(zvector3f* v1, zvector3f* v2)
{
    zvector3f* result = (zvector3f*)malloc(sizeof(zvector3f));
    result->x = v1->x * v2->x;
    result->y = v1->y * v2->y;
    result->z = v1->z * v2->z;
    return result;
}

zvector3f* __cdecl zl_vect3div(zvector3f* v1, zvector3f* v2)
{
    zvector3f* result = (zvector3f*)malloc(sizeof(zvector3f));
    result->x = v1->x / v2->x;
    result->y = v1->y / v2->y;
    result->z = v1->z / v2->z;
    return result;
}

zvector3f* __cdecl zl_vect3create(float x, float y, float z)
{
    zvector3f* result = (zvector3f*)malloc(sizeof(zvector3f));
    result->x = x;
    result->y = y;
    result->z = z;
    return result;
}

zvector3f* __cdecl zl_vect3normalize(zvector3f* v)
{
    float length = sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
    zvector3f* result = (zvector3f*)malloc(sizeof(zvector3f));
    result->x = v->x / length;
    result->y = v->y / length;
    result->z = v->z / length;
    return result;
}

#pragma endregion

#pragma region Vector4f

zvector4f* __cdecl zl_vect4add(zvector4f* v1, zvector4f* v2)
{
    zvector4f* result = (zvector4f*)malloc(sizeof(zvector4f));
    result->x = v1->x + v2->x;
    result->y = v1->y + v2->y;
    result->z = v1->z + v2->z;
    result->w = v1->w + v2->w;
    return result;
}

zvector4f* __cdecl zl_vect4sub(zvector4f* v1, zvector4f* v2)
{
    zvector4f* result = (zvector4f*)malloc(sizeof(zvector4f));
    result->x = v1->x - v2->x;
    result->y = v1->y - v2->y;
    result->z = v1->z - v2->z;
    result->w = v1->w - v2->w;
    return result;
}

zvector4f* __cdecl zl_vect4mul(zvector4f* v1, zvector4f* v2)
{
    zvector4f* result = (zvector4f*)malloc(sizeof(zvector4f));
    result->x = v1->x * v2->x;
    result->y = v1->y * v2->y;
    result->z = v1->z * v2->z;
    result->w = v1->w * v2->w;
    return result;
}

zvector4f* __cdecl zl_vect4div(zvector4f* v1, zvector4f* v2)
{
    zvector4f* result = (zvector4f*)malloc(sizeof(zvector4f));
    result->x = v1->x / v2->x;
    result->y = v1->y / v2->y;
    result->z = v1->z / v2->z;
    result->w = v1->w / v2->w;
    return result;
}

zvector4f* __cdecl zl_vect4create(float x, float y, float z, float w)
{
    zvector4f* result = (zvector4f*)malloc(sizeof(zvector4f));
    result->x = x;
    result->y = y;
    result->z = z;
    result->w = w;
    return result;
}

zvector4f* __cdecl zl_vect4normalize(zvector4f* v)
{
    float length = sqrtf(v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w);
    zvector4f* result = (zvector4f*)malloc(sizeof(zvector4f));
    result->x = v->x / length;
    result->y = v->y / length;
    result->z = v->z / length;
    result->w = v->w / length;
    return result;
}

#pragma endregion

#pragma region Vector2d

zvector2d* __cdecl zl_vect2add_d(zvector2d* v1, zvector2d* v2)
{
    zvector2d* result = (zvector2d*)malloc(sizeof(zvector2d));
    result->x = v1->x + v2->x;
    result->y = v1->y + v2->y;
    return result;
}

zvector2d* __cdecl zl_vect2sub_d(zvector2d* v1, zvector2d* v2)
{
    zvector2d* result = (zvector2d*)malloc(sizeof(zvector2d));
    result->x = v1->x - v2->x;
    result->y = v1->y - v2->y;
    return result;
}

zvector2d* __cdecl zl_vect2mul_d(zvector2d* v1, zvector2d* v2)
{
    zvector2d* result = (zvector2d*)malloc(sizeof(zvector2d));
    result->x = v1->x * v2->x;
    result->y = v1->y * v2->y;
    return result;
}

zvector2d* __cdecl zl_vect2div_d(zvector2d* v1, zvector2d* v2)
{
    zvector2d* result = (zvector2d*)malloc(sizeof(zvector2d));
    result->x = v1->x / v2->x;
    result->y = v1->y / v2->y;
    return result;
}

zvector2d* __cdecl zl_vect2create_d(double x, double y)
{
    zvector2d* result = (zvector2d*)malloc(sizeof(zvector2d));
    result->x = x;
    result->y = y;
    return result;
}

zvector2d* __cdecl zl_vect2normalize_d(zvector2d* v)
{
    double length = sqrt(v->x * v->x + v->y * v->y);
    zvector2d* result = (zvector2d*)malloc(sizeof(zvector2d));
    result->x = v->x / length;
    result->y = v->y / length;
    return result;
}

#pragma endregion

#pragma region Vector3d

zvector3d* __cdecl zl_vect3add_d(zvector3d* v1, zvector3d* v2)
{
    zvector3d* result = (zvector3d*)malloc(sizeof(zvector3d));
    result->x = v1->x + v2->x;
    result->y = v1->y + v2->y;
    result->z = v1->z + v2->z;
    return result;
}

zvector3d* __cdecl zl_vect3sub_d(zvector3d* v1, zvector3d* v2)
{
    zvector3d* result = (zvector3d*)malloc(sizeof(zvector3d));
    result->x = v1->x - v2->x;
    result->y = v1->y - v2->y;
    result->z = v1->z - v2->z;
    return result;
}

zvector3d* __cdecl zl_vect3mul_d(zvector3d* v1, zvector3d* v2)
{
    zvector3d* result = (zvector3d*)malloc(sizeof(zvector3d));
    result->x = v1->x * v2->x;
    result->y = v1->y * v2->y;
    result->z = v1->z * v2->z;
    return result;
}

zvector3d* __cdecl zl_vect3div_d(zvector3d* v1, zvector3d* v2)
{
    zvector3d* result = (zvector3d*)malloc(sizeof(zvector3d));
    result->x = v1->x / v2->x;
    result->y = v1->y / v2->y;
    result->z = v1->z / v2->z;
    return result;
}

zvector3d* __cdecl zl_vect3create_d(double x, double y, double z)
{
    zvector3d* result = (zvector3d*)malloc(sizeof(zvector3d));
    result->x = x;
    result->y = y;
    result->z = z;
    return result;
}

zvector3d* __cdecl zl_vect3normalize_d(zvector3d* v)
{
    double length = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
    zvector3d* result = (zvector3d*)malloc(sizeof(zvector3d));
    result->x = v->x / length;
    result->y = v->y / length;
    result->z = v->z / length;
    return result;
}

#pragma endregion

#pragma region Vector4d

zvector4d* __cdecl zl_vect4add_d(zvector4d* v1, zvector4d* v2)
{
    zvector4d* result = (zvector4d*)malloc(sizeof(zvector4d));
    result->x = v1->x + v2->x;
    result->y = v1->y + v2->y;
    result->z = v1->z + v2->z;
    result->w = v1->w + v2->w;
    return result;
}

zvector4d* __cdecl zl_vect4sub_d(zvector4d* v1, zvector4d* v2)
{
    zvector4d* result = (zvector4d*)malloc(sizeof(zvector4d));
    result->x = v1->x - v2->x;
    result->y = v1->y - v2->y;
    result->z = v1->z - v2->z;
    result->w = v1->w - v2->w;
    return result;
}

zvector4d* __cdecl zl_vect4mul_d(zvector4d* v1, zvector4d* v2)
{
    zvector4d* result = (zvector4d*)malloc(sizeof(zvector4d));
    result->x = v1->x * v2->x;
    result->y = v1->y * v2->y;
    result->z = v1->z * v2->z;
    result->w = v1->w * v2->w;
    return result;
}

zvector4d* __cdecl zl_vect4div_d(zvector4d* v1, zvector4d* v2)
{
    zvector4d* result = (zvector4d*)malloc(sizeof(zvector4d));
    result->x = v1->x / v2->x;
    result->y = v1->y / v2->y;
    result->z = v1->z / v2->z;
    result->w = v1->w / v2->w;
    return result;
}

zvector4d* __cdecl zl_vect4create_d(double x, double y, double z, double w)
{
    zvector4d* result = (zvector4d*)malloc(sizeof(zvector4d));
    result->x = x;
    result->y = y;
    result->z = z;
    result->w = w;
    return result;
}

zvector4d* __cdecl zl_vect4normalize_d(zvector4d* v)
{
    double length = sqrt(v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w);
    zvector4d* result = (zvector4d*)malloc(sizeof(zvector4d));
    result->x = v->x / length;
    result->y = v->y / length;
    result->z = v->z / length;
    result->w = v->w / length;
    return result;
}

#pragma endregion

#pragma region Vector2i

zvector2i* __cdecl zl_vect2add_i(zvector2i* v1, zvector2i* v2)
{
    zvector2i* result = (zvector2i*)malloc(sizeof(zvector2i));
    result->x = v1->x + v2->x;
    result->y = v1->y + v2->y;
    return result;
}

zvector2i* __cdecl zl_vect2sub_i(zvector2i* v1, zvector2i* v2)
{
    zvector2i* result = (zvector2i*)malloc(sizeof(zvector2i));
    result->x = v1->x - v2->x;
    result->y = v1->y - v2->y;
    return result;
}

zvector2i* __cdecl zl_vect2mul_i(zvector2i* v1, zvector2i* v2)
{
    zvector2i* result = (zvector2i*)malloc(sizeof(zvector2i));
    result->x = v1->x * v2->x;
    result->y = v1->y * v2->y;
    return result;
}

zvector2i* __cdecl zl_vect2div_i(zvector2i* v1, zvector2i* v2)
{
    zvector2i* result = (zvector2i*)malloc(sizeof(zvector2i));
    result->x = v1->x / v2->x;
    result->y = v1->y / v2->y;
    return result;
}

zvector2i* __cdecl zl_vect2create_i(int x, int y)
{
    zvector2i* result = (zvector2i*)malloc(sizeof(zvector2i));
    result->x = x;
    result->y = y;
    return result;
}

zvector2i* __cdecl zl_vect2normalize_i(zvector2i* v)
{
    int length = sqrt(v->x * v->x + v->y * v->y);
    zvector2i* result = (zvector2i*)malloc(sizeof(zvector2i));
    result->x = v->x / length;
    result->y = v->y / length;
    return result;
}

#pragma endregion

#pragma region Vector3i

zvector3i* __cdecl zl_vect3add_i(zvector3i* v1, zvector3i* v2)
{
    zvector3i* result = (zvector3i*)malloc(sizeof(zvector3i));
    result->x = v1->x + v2->x;
    result->y = v1->y + v2->y;
    result->z = v1->z + v2->z;
    return result;
}

zvector3i* __cdecl zl_vect3sub_i(zvector3i* v1, zvector3i* v2)
{
    zvector3i* result = (zvector3i*)malloc(sizeof(zvector3i));
    result->x = v1->x - v2->x;
    result->y = v1->y - v2->y;
    result->z = v1->z - v2->z;
    return result;
}

zvector3i* __cdecl zl_vect3mul_i(zvector3i* v1, zvector3i* v2)
{
    zvector3i* result = (zvector3i*)malloc(sizeof(zvector3i));
    result->x = v1->x * v2->x;
    result->y = v1->y * v2->y;
    result->z = v1->z * v2->z;
    return result;
}

zvector3i* __cdecl zl_vect3div_i(zvector3i* v1, zvector3i* v2)
{
    zvector3i* result = (zvector3i*)malloc(sizeof(zvector3i));
    result->x = v1->x / v2->x;
    result->y = v1->y / v2->y;
    result->z = v1->z / v2->z;
    return result;
}

zvector3i* __cdecl zl_vect3create_i(int x, int y, int z)
{
    zvector3i* result = (zvector3i*)malloc(sizeof(zvector3i));
    result->x = x;
    result->y = y;
    result->z = z;
    return result;
}

zvector3i* __cdecl zl_vect3normalize_i(zvector3i* v)
{
    int length = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
    zvector3i* result = (zvector3i*)malloc(sizeof(zvector3i));
    result->x = v->x / length;
    result->y = v->y / length;
    result->z = v->z / length;
    return result;
}

#pragma endregion

#pragma region Vector4i

zvector4i* __cdecl zl_vect4add_i(zvector4i* v1, zvector4i* v2)
{
    zvector4i* result = (zvector4i*)malloc(sizeof(zvector4i));
    result->x = v1->x + v2->x;
    result->y = v1->y + v2->y;
    result->z = v1->z + v2->z;
    result->w = v1->w + v2->w;
    return result;
}

zvector4i* __cdecl zl_vect4sub_i(zvector4i* v1, zvector4i* v2)
{
    zvector4i* result = (zvector4i*)malloc(sizeof(zvector4i));
    result->x = v1->x - v2->x;
    result->y = v1->y - v2->y;
    result->z = v1->z - v2->z;
    result->w = v1->w - v2->w;
    return result;
}

zvector4i* __cdecl zl_vect4mul_i(zvector4i* v1, zvector4i* v2)
{
    zvector4i* result = (zvector4i*)malloc(sizeof(zvector4i));
    result->x = v1->x * v2->x;
    result->y = v1->y * v2->y;
    result->z = v1->z * v2->z;
    result->w = v1->w * v2->w;
    return result;
}

zvector4i* __cdecl zl_vect4div_i(zvector4i* v1, zvector4i* v2)
{
    zvector4i* result = (zvector4i*)malloc(sizeof(zvector4i));
    result->x = v1->x / v2->x;
    result->y = v1->y / v2->y;
    result->z = v1->z / v2->z;
    result->w = v1->w / v2->w;
    return result;
}

zvector4i* __cdecl zl_vect4create_i(int x, int y, int z, int w)
{
    zvector4i* result = (zvector4i*)malloc(sizeof(zvector4i));
    result->x = x;
    result->y = y;
    result->z = z;
    result->w = w;
    return result;
}

zvector4i* __cdecl zl_vect4normalize_i(zvector4i* v)
{
    int length = sqrt(v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w);
    zvector4i* result = (zvector4i*)malloc(sizeof(zvector4i));
    result->x = v->x / length;
    result->y = v->y / length;
    result->z = v->z / length;
    result->w = v->w / length;
    return result;
}

#pragma endregion

#pragma region Matrix4f

zmatrix4f* __cdecl zl_mat4add(zmatrix4f* m1, zmatrix4f* m2)
{
    zmatrix4f* result = (zmatrix4f*)malloc(sizeof(zmatrix4f));
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result->m[i][j] = m1->m[i][j] + m2->m[i][j];
        }
    }
    return result;
}

zmatrix4f* __cdecl zl_mat4sub(zmatrix4f* m1, zmatrix4f* m2)
{
    zmatrix4f* result = (zmatrix4f*)malloc(sizeof(zmatrix4f));
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result->m[i][j] = m1->m[i][j] - m2->m[i][j];
        }
    }
    return result;
}

zmatrix4f* __cdecl zl_mat4mul(zmatrix4f* m1, zmatrix4f* m2)
{
    zmatrix4f* result = (zmatrix4f*)malloc(sizeof(zmatrix4f));
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result->m[i][j] = m1->m[i][j] * m2->m[i][j];
        }
    }
    return result;
}

zmatrix4f* __cdecl zl_mat4div(zmatrix4f* m1, zmatrix4f* m2)
{
    zmatrix4f* result = (zmatrix4f*)malloc(sizeof(zmatrix4f));
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result->m[i][j] = m1->m[i][j] / m2->m[i][j];
        }
    }
    return result;
}

zmatrix4f* __cdecl zl_mat4create(float m[4][4])
{
    zmatrix4f* result = (zmatrix4f*)malloc(sizeof(zmatrix4f));
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result->m[i][j] = m[i][j];
        }
    }
    return result;
}

zmatrix4f* __cdecl zl_mat4normalize(zmatrix4f* m)
{
    float length = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            length += m->m[i][j] * m->m[i][j];
        }
    }
    length = sqrtf(length);
    zmatrix4f* result = (zmatrix4f*)malloc(sizeof(zmatrix4f));
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result->m[i][j] = m->m[i][j] / length;
        }
    }
    return result;
}

#pragma endregion

#pragma region Matrix4d

zmatrix4d* __cdecl zl_mat4add_d(zmatrix4d* m1, zmatrix4d* m2)
{
    zmatrix4d* result = (zmatrix4d*)malloc(sizeof(zmatrix4d));
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result->m[i][j] = m1->m[i][j] + m2->m[i][j];
        }
    }
    return result;
}

zmatrix4d* __cdecl zl_mat4sub_d(zmatrix4d* m1, zmatrix4d* m2)
{
    zmatrix4d* result = (zmatrix4d*)malloc(sizeof(zmatrix4d));
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result->m[i][j] = m1->m[i][j] - m2->m[i][j];
        }
    }
    return result;
}

zmatrix4d* __cdecl zl_mat4mul_d(zmatrix4d* m1, zmatrix4d* m2)
{
    zmatrix4d* result = (zmatrix4d*)malloc(sizeof(zmatrix4d));
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result->m[i][j] = m1->m[i][j] * m2->m[i][j];
        }
    }
    return result;
}

zmatrix4d* __cdecl zl_mat4div_d(zmatrix4d* m1, zmatrix4d* m2)
{
    zmatrix4d* result = (zmatrix4d*)malloc(sizeof(zmatrix4d));
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result->m[i][j] = m1->m[i][j] / m2->m[i][j];
        }
    }
    return result;
}

zmatrix4d* __cdecl zl_mat4create_d(double m[4][4])
{
    zmatrix4d* result = (zmatrix4d*)malloc(sizeof(zmatrix4d));
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result->m[i][j] = m[i][j];
        }
    }
    return result;
}

zmatrix4d* __cdecl zl_mat4normalize_d(zmatrix4d* m)
{
    double length = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            length += m->m[i][j] * m->m[i][j];
        }
    }
    length = sqrt(length);
    zmatrix4d* result = (zmatrix4d*)malloc(sizeof(zmatrix4d));
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result->m[i][j] = m->m[i][j] / length;
        }
    }
    return result;
}

#pragma endregion

#pragma region Matrix4i

zmatrix4i* __cdecl zl_mat4add_i(zmatrix4i* m1, zmatrix4i* m2)
{
    zmatrix4i* result = (zmatrix4i*)malloc(sizeof(zmatrix4i));
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result->m[i][j] = m1->m[i][j] + m2->m[i][j];
        }
    }
    return result;
}

zmatrix4i* __cdecl zl_mat4sub_i(zmatrix4i* m1, zmatrix4i* m2)
{
    zmatrix4i* result = (zmatrix4i*)malloc(sizeof(zmatrix4i));
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result->m[i][j] = m1->m[i][j] - m2->m[i][j];
        }
    }
    return result;
}

zmatrix4i* __cdecl zl_mat4mul_i(zmatrix4i* m1, zmatrix4i* m2)
{
    zmatrix4i* result = (zmatrix4i*)malloc(sizeof(zmatrix4i));
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result->m[i][j] = m1->m[i][j] * m2->m[i][j];
        }
    }
    return result;
}

zmatrix4i* __cdecl zl_mat4div_i(zmatrix4i* m1, zmatrix4i* m2)
{
    zmatrix4i* result = (zmatrix4i*)malloc(sizeof(zmatrix4i));
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result->m[i][j] = m1->m[i][j] / m2->m[i][j];
        }
    }
    return result;
}

zmatrix4i* __cdecl zl_mat4create_i(int m[4][4])
{
    zmatrix4i* result = (zmatrix4i*)malloc(sizeof(zmatrix4i));
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result->m[i][j] = m[i][j];
        }
    }
    return result;
}

zmatrix4i* __cdecl zl_mat4normalize_i(zmatrix4i* m)
{
    int length = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            length += m->m[i][j] * m->m[i][j];
        }
    }
    length = sqrt(length);
    zmatrix4i* result = (zmatrix4i*)malloc(sizeof(zmatrix4i));
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result->m[i][j] = m->m[i][j] / length;
        }
    }
    return result;
}

#pragma endregion

#pragma region Quaternion

zquaternion* __cdecl zl_quatadd(zquaternion* q1, zquaternion* q2)
{
    zquaternion* result = (zquaternion*)malloc(sizeof(zquaternion));
    result->x = q1->x + q2->x;
    result->y = q1->y + q2->y;
    result->z = q1->z + q2->z;
    result->w = q1->w + q2->w;
    return result;
}

zquaternion* __cdecl zl_quatsub(zquaternion* q1, zquaternion* q2)
{
    zquaternion* result = (zquaternion*)malloc(sizeof(zquaternion));
    result->x = q1->x - q2->x;
    result->y = q1->y - q2->y;
    result->z = q1->z - q2->z;
    result->w = q1->w - q2->w;
    return result;
}

zquaternion* __cdecl zl_quatmul(zquaternion* q1, zquaternion* q2)
{
    zquaternion* result = (zquaternion*)malloc(sizeof(zquaternion));
    result->x = q1->x * q2->x;
    result->y = q1->y * q2->y;
    result->z = q1->z * q2->z;
    result->w = q1->w * q2->w;
    return result;
}

zquaternion* __cdecl zl_quatdiv(zquaternion* q1, zquaternion* q2)
{
    zquaternion* result = (zquaternion*)malloc(sizeof(zquaternion));
    result->x = q1->x / q2->x;
    result->y = q1->y / q2->y;
    result->z = q1->z / q2->z;
    result->w = q1->w / q2->w;
    return result;
}

zquaternion* __cdecl zl_quatcreate(float x, float y, float z, float w)
{
    zquaternion* result = (zquaternion*)malloc(sizeof(zquaternion));
    result->x = x;
    result->y = y;
    result->z = z;
    result->w = w;
    return result;
}

zquaternion* __cdecl zl_quatnormalize(zquaternion* q)
{
    float length = sqrtf(q->x * q->x + q->y * q->y + q->z * q->z + q->w * q->w);
    zquaternion* result = (zquaternion*)malloc(sizeof(zquaternion));
    result->x = q->x / length;
    result->y = q->y / length;
    result->z = q->z / length;
    result->w = q->w / length;
    return result;
}

#pragma endregion