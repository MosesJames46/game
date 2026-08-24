#include "Matrix3x3.h"
#include "vector4.h"

#ifndef MATRIX4x4_H
#define MATRIX4x4_H

typedef float matrix4x4[4][4];
typedef struct Mat4x4{
    matrix4x4 m;
}Mat4x4;

Mat4x4 mat4x4_mul(Mat4x4 A, Mat4x4 B);

Mat4x4 mat4x4_inverse(Mat4x4 A);

Mat4x4 mat4x4_cofactor(Mat4x4 A);

//Helper functions for inverse operations on 3x3 matrix
Mat4x4 mat4x4_scale(float s);
Mat4x4 mat4x4_translate(vec3 t);
float mat4x4_determinant(Mat4x4 A);
void mat4x4_transpose(Mat4x4* A);

Mat4x4 mat4x4_rotate(vec3 axis);
void mat4x4_print(Mat4x4 A);
vec4 mat4x4_mulv(Mat4x4 A, vec4 a);
vec3 mat4x4_mulv3(Mat4x4 A, vec3 a);
void mat4x4_init(Mat4x4* A);
Mat4x4 mat4x4_identity();
void mat4x4_print_mat4x4(Mat4x4 A);

Mat4x4 mat4x4_create_matrixv3(vec3 a, vec3 b, vec3 c);
Mat4x4 mat4x4_create_matrixv4(vec4 a, vec4 b, vec4 c);




Mat4x4 mat4x4_lookat(vec3 eye, vec3 center, vec3 up);
Mat4x4 mat4x4_view(vec3 camera_position, vec3 look_at, vec3 up);

Mat4x4 mat4x4_perspective_projection(float fov, float aspect, float znear, float zfar);

vec4 mat4x4_perspective_divide(Mat4x4 projection_matrix, vec4 vertex);

Mat4x4 mat4x4_diablo_viewport();
#endif