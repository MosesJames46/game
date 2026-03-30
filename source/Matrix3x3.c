#include "../headers/Matrix3x3.h"
#include "../headers/game_math.h"
#include "../headers/game_utility.h"
#include <math.h>
#include <stdio.h>

Mat3x3 mat3x3_mul(Mat3x3 A, Mat3x3 B){
    Mat3x3 C;
    
    for(int i = 0; i < 3; i++){
        int k = 0;
        float result = 0;
        for (int j = 0; j < 3; j++){
            result += A.m[i][j] * B.m[j][k];
        }

        C.m[i][k] = result;
        k++;
        result = 0;
        for (int j = 0; j < 3; j++){
            result += A.m[i][j] * B.m[j][k];
        }

        C.m[i][k] = result;
        k++;
        result = 0;
        for (int j = 0; j < 3; j++){
            result += A.m[i][j] * B.m[j][k];
        }

        C.m[i][k] = result;
    }
    return C;
}

void mat3x3_init(Mat3x3* A){
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            A->m[i][j] = 0;
        }
    }
}

vec3 mat3x3_rotate(vec3 axis, vec3 point){
    const float radian = 3.14159f / 180.f;
    
    //Multiply radian value by the value to rotate by for each component of axis.
    float xd = radian * axis.x;
    Mat3x3 X;

    mat3x3_init(&X);
    X.m[0][0] = 1;
    X.m[1][1] = cosf(xd);
    X.m[1][2] = -sinf(xd);
    X.m[2][1] = sinf(xd);
    X.m[2][2] = cosf(xd);
    
     
    float yd = radian * axis.y;
    Mat3x3 Y;
    mat3x3_init(&Y);
    Y.m[0][0] = cosf(yd);
    Y.m[0][2] = sinf(yd);
    Y.m[1][1] = 1;
    Y.m[2][0] = -sinf(yd);
    Y.m[2][2] = cosf(yd);

    float zd = radian * axis.z;
    Mat3x3 Z;
    mat3x3_init(&Z);
    Z.m[0][0] = cosf(zd);
    Z.m[0][1] = -sinf(zd);
    Z.m[1][0] = sinf(zd);
    Z.m[1][1] = cosf(zd);
    Z.m[2][2] = 1;

    Mat3x3 R = mat3x3_mul(Y, Z);
    R = mat3x3_mul(X, R);
    return mat3x3_mulv(R, point);
}

vec3 mat3x3_mulv(Mat3x3 A, vec3 a){
    vec3 b;
    float br[3] = {0};
    for (int i = 0; i < 3; i++){
        float result = 0;
        result += A.m[i][0] * a.x;
        result += A.m[i][1] * a.y;
        result += A.m[i][2] * a.z;
        br[i] = result;
    }

    b.x = br[0];
    b.y = br[1];
    b.z = br[2];

    return b;
}

void mat3x3_print(Mat3x3 A){
    for (int i = 0; i < 3; i++){
        for (int j =0; j < 3; j++){
            printf("%f", A.m[i][j]);
        }
        printf("\n");
    }
}


Mat3x3 mat3x3_cofactor(Mat3x3 A){
    float Af = A.m[1][1] * A.m[2][2] - A.m[1][2] * A.m[2][1];
	float B = -(A.m[1][0] * A.m[2][2] - A.m[1][2] * A.m[2][0]);
	float C = A.m[1][0] * A.m[2][1] - A.m[1][1] * A.m[2][0];
	float D = -(A.m[0][1] * A.m[2][2] - A.m[0][2] * A.m[2][1]);
	float E = A.m[0][0] * A.m[2][2] - A.m[0][2] * A.m[2][0];
	float F = -(A.m[0][0] * A.m[2][1] - A.m[0][1] * A.m[2][0]);
	float G = A.m[0][1] * A.m[1][2] - A.m[1][1] * A.m[0][2];
	float H = -(A.m[0][0] * A.m[1][2] - A.m[0][2] * A.m[1][0]);
	float I = A.m[0][0] * A.m[1][1] - A.m[0][1] * A.m[1][0];

    vec3 a = {Af, B, C};
    vec3 b = {D, E, F};
    vec3 c = {G, H, I};

    Mat3x3 r = mat3x3_create_matrix(a , b, c);

    return r;
}

void mat3x3_transpose(Mat3x3* A){
    swapf(&A->m[0][1], &A->m[1][0]);
    swapf(&A->m[2][0], &A->m[0][2]);
    swapf(&A->m[2][1], &A->m[1][2]);
}

float mat3x3_determinant(Mat3x3 A){
    float r = 
			A.m[0][0] * A.m[1][1] * A.m[2][2] +
			A.m[0][1] * A.m[1][2] * A.m[2][0] +
			A.m[0][2] * A.m[1][0] * A.m[2][1] -
			A.m[0][2] * A.m[1][1] * A.m[2][0] -
			A.m[0][1] * A.m[1][0] * A.m[2][2] -
			A.m[0][0] * A.m[1][2] * A.m[2][1];
    return r;
}

Mat3x3 mat3x3_inverse(Mat3x3 A){
    float determinant = mat3x3_determinant(A);
    A = mat3x3_cofactor(A);
    mat3x3_transpose(&A);
    //printf("Cofactor: ");
    //mat3x3_print_mat3x3(A);
    //printf("\n\n");
    //mat3x3_print_mat3x3(A);
    //mat3x3_transpose(&A);
    //mat3x3_print_mat3x3(A);
    
    //printf("Determinant: %f\n", determinant);
    determinant = 1.f / determinant;
    
    A = mat3x3_scal(A, determinant);
    //mat3x3_print_mat3x3(A);
    return A;
}

Mat3x3 mat3x3_scal(Mat3x3 A, float s){
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            A.m[i][j] *= s;
        }
    }
    return A;
}

Mat3x3 mat3x3_create_matrix(vec3 a, vec3 b, vec3 c){
    Mat3x3 C;
    C.m[0][0] = a.x; C.m[0][1] = a.y; C.m[0][2] = a.z;
    C.m[1][0] = b.x; C.m[1][1] = b.y; C.m[1][2] = b.z;
    C.m[2][0] = c.x; C.m[2][1] = c.y; C.m[2][2] = c.z;
    return C;
}

void mat3x3_print_mat3x3(Mat3x3 A){
    for (int  i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            printf("%f ", A.m[i][j]);
        }
        printf("\n");
    }
}