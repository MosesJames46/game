#include "vector3.h"

#ifndef MATRIX3x3_H
#define MATRIX3x3_H

typedef float matrix3x3[3][3];
typedef struct Mat3x3{
    matrix3x3 m;
}Mat3x3;

Mat3x3 mat3x3_mul(Mat3x3 A, Mat3x3 B);

Mat3x3 mat3x3_inverse(Mat3x3 A);

Mat3x3 mat3x3_cofactor(Mat3x3 A);

//Helper functions for inverse operations on 3x3 matrix
Mat3x3 mat3x3_scal(Mat3x3 A, float s);
float mat3x3_determinant(Mat3x3 A);
void mat3x3_transpose(Mat3x3* A);

vec3 mat3x3_rotate(vec3 axis, vec3 point);
void mat3x3_print(Mat3x3 A);
vec3 mat3x3_mulv(Mat3x3 A, vec3 a);
void mat3x3_init(Mat3x3* A);

Mat3x3 mat3x3_create_matrix(vec3 a, vec3 b, vec3 c);

void mat3x3_print_mat3x3(Mat3x3 A);
#endif