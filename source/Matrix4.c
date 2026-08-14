#include "../headers/Matrix4.h"
#include "../headers/game_math.h"
#include "../headers/game_utility.h"
#include "../headers/Camera.h"
#include "../headers/game_data.h"
#include <math.h>
#include <stdio.h>

Mat4x4 mat4x4_mul(Mat4x4 A, Mat4x4 B){
    Mat4x4 C;
    #pragma opm parallel for
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            float result = 0;
            for(int k = 0; k < 4; k++){
                result += A.m[i][k] * B.m[k][j];
            }
            C.m[i][j] = result;
        }
    }
    return C;
}

void mat4x4_init(Mat4x4* A){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            A->m[i][j] = 0;
        }
    }
}

Mat4x4 mat4x4_identity(){
    Mat4x4 I;
    mat4x4_init(&I);
    I.m[0][0]= 1;I.m[1][1] = 1;I.m[2][2] = 1;I.m[3][3] = 1;
    return I;
}

vec3 mat4x4_rotate(vec3 axis, vec3 point){
    const float radian = 3.14159f / 180.f;
    
    //Multiply radian value by the value to rotate by for each component of axis.
    float xd = radian * axis.x;
    Mat4x4 X;

    mat4x4_init(&X);
    X.m[0][0] = 1;
    X.m[1][1] = cosf(xd);
    X.m[1][2] = -sinf(xd);
    X.m[2][1] = sinf(xd);
    X.m[2][2] = cosf(xd);
    
     
    float yd = radian * axis.y;
    Mat4x4 Y;
    mat4x4_init(&Y);
    Y.m[0][0] = cosf(yd);
    Y.m[0][2] = sinf(yd);
    Y.m[1][1] = 1;
    Y.m[2][0] = -sinf(yd);
    Y.m[2][2] = cosf(yd);

    float zd = radian * axis.z;
    Mat4x4 Z;
    mat4x4_init(&Z);
    Z.m[0][0] = cosf(zd);
    Z.m[0][1] = -sinf(zd);
    Z.m[1][0] = sinf(zd);
    Z.m[1][1] = cosf(zd);
    Z.m[2][2] = 1;

    Mat4x4 R = mat4x4_mul(Y, Z);
    R = mat4x4_mul(X, R);
    return mat4x4_mulv3(R, point);
}

vec4 mat4x4_mulv(Mat4x4 A, vec4 a){
    vec4 b;
    float br[4] = {0};
    for (int i = 0; i < 4; i++){
        float result = 0;
        result += A.m[i][0] * a.x;
        result += A.m[i][1] * a.y;
        result += A.m[i][2] * a.z;
        result += A.m[i][3] * a.w;
        br[i] = result;
    }

    b.x = br[0];
    b.y = br[1];
    b.z = br[2];
    b.w = br[3];

    return b;
}

vec3 mat4x4_mulv3(Mat4x4 A, vec3 a){
    vec3 b;
    float br[4] = {0};
    for (int i = 0; i < 4; i++){
        float result = 0;
        result += A.m[i][0] * a.x;
        result += A.m[i][1] * a.y;
        result += A.m[i][2] * a.z;
        result += A.m[i][3] * 1;
        br[i] = result;
    }

    b.x = br[0];
    b.y = br[1];
    b.z = br[2];

    return b;
}


void mat4x4_print(Mat4x4 A){
    for (int i = 0; i < 3; i++){
        for (int j =0; j < 3; j++){
            printf("%f", A.m[i][j]);
        }
        printf("\n");
    }
}


Mat4x4 mat4x4_cofactor(Mat4x4 A){
    Mat4x4 temp;
    int mp_i, mp_j, ap_i, ap_j;
    //The 
    mp_i = mp_j = 0;
    int sign = 1;

    int co_i, co_j;
    co_i = co_j = 0;
    //This quad nested for loop finds the row column that is to be excluded.
    //This will be i and j. It iterates through the input matrix to only input values that are
    //not on the row i and column j. This will be row + col - 1, since there will be overlap when
    //highlighting the row/column value. Once we find the value in the matrix, we place values into
    //the det matrix using mp_i/mp_j pointers. 
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            sign = (((i + j) % 2) == 0 )? 1 : -1; 
            Mat3x3 det_matrix;
            mp_i = 0;
            for(int k = 0; k < 4; k++){
                mp_j = 0;
                int l;
                for(l = 0; l < 4; l++){
                    //If Skip M_ij
                    if (l == j || i == k) continue;
                    //Only assign values to the determinant matrix when on correct cofactor
                    det_matrix.m[mp_i][mp_j] = A.m[k][l];
                    mp_j++;
                }
                if (k == i || l == j) continue;
                mp_i++;
            }
            //Assign the determinant of our new matrix to the cofactor matrix
            temp.m[i][j] = mat3x3_determinant(det_matrix) * sign;
        }
    }
    return temp;
}

void mat4x4_transpose(Mat4x4* A){
    swapf(&A->m[0][1], &A->m[1][0]);
    swapf(&A->m[2][0], &A->m[0][2]);
    swapf(&A->m[2][1], &A->m[1][2]);
    swapf(&A->m[3][0], &A->m[0][3]);
    swapf(&A->m[1][3], &A->m[3][1]);
    swapf(&A->m[3][2], &A->m[2][3]);
}

float mat4x4_determinant(Mat4x4 A){
    //tracks the temporary pointers
    int t_i, t_j;
    int sign;
    float r = 0;
    
    //The determinant of a 4x4 matrix is calculate by taking to coefficients of the first row 
    //multiplying these values by the determinant of the minor obtained by removing the corresponding column
    //and applying to correct sign.
    for (int i = 0; i < 4; i++){
        Mat3x3 temp;
        t_i = 0;
        for (int j = 0; j < 4; j++){
            t_j = 0;
            int k;
            for (k = 0; k < 4; k++){
                //The index i represents our column.
                if (j == 0 || k == i) continue;
                temp.m[t_i][t_j++] = A.m[j][k];
            }
            if (j == 0 || k == i) continue;
            ++t_i;
        }
        //mat3x3_print_mat3x3(temp);
        sign = ((i % 2) == 0) ? 1 : -1;
        r += sign * A.m[0][i] * mat3x3_determinant(temp);
    }
    return r;
}

Mat4x4 mat4x4_inverse(Mat4x4 A){
    float determinant = mat4x4_determinant(A);
    //printf("Determinant: %f\n", determinant);
    A = mat4x4_cofactor(A);
    
    mat4x4_transpose(&A);
    //mat4x4_print_mat4x4(A);
    determinant = 1.f / determinant;

    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            A.m[i][j] *= determinant;
        }
    }
    return A;
}

vec3 mat4x4_scale(vec3 p, float s){
    Mat4x4 I = mat4x4_identity();
    I.m[0][0] *= s;
    I.m[1][1] *= s;
    I.m[2][2] *= s;
    return mat4x4_mulv3(I, p);
}


vec3 mat4x4_translate(vec3 p, vec3 x){
    //Defines how far to translate the object.
    Mat4x4 m = mat4x4_create_matrixv4((vec4){1, 0, 0, x.x}, (vec4){0, 1, 0, x.y}, (vec4){0, 0, 1, x.z});
    vec4 result = mat4x4_mulv(m, (vec4){p.x, p.y, p.z, 1});
    return (vec3){result.x, result.y, result.z};
}

Mat4x4 mat4x4_create_matrixv3(vec3 a, vec3 b, vec3 c){
    Mat4x4 C;
    C.m[0][0] = a.x; C.m[1][0] = b.x; C.m[2][0] = c.x; C.m[3][0] = 0;
    C.m[0][1] = a.y; C.m[1][1] = b.y; C.m[2][1] = c.y; C.m[3][1] = 0;
    C.m[0][2] = a.z; C.m[1][2] = b.z; C.m[2][2] = c.z; C.m[3][2] = 0;
    C.m[0][3] = 0; C.m[1][3] = 0; C.m[2][3] = 0; C.m[3][3] = 1.f;
    return C;
}

Mat4x4 mat4x4_create_matrixv4(vec4 a, vec4 b, vec4 c){
    Mat4x4 C;
    C.m[0][0] = a.x; C.m[1][0] = b.x; C.m[2][0] = c.x; C.m[3][0] = 0;
    C.m[0][1] = a.y; C.m[1][1] = b.y; C.m[2][1] = c.y; C.m[3][1] = 0;
    C.m[0][2] = a.z; C.m[1][2] = b.z; C.m[2][2] = c.z; C.m[3][2] = 0;
    C.m[0][3] = a.w; C.m[1][3] = b.w; C.m[2][3] = c.w; C.m[3][3] = 1.f;

    return C;
}

void mat4x4_print_mat4x4(Mat4x4 A){
    for (int  i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            printf("%f ", A.m[i][j]);
        }
        printf("\n");
    }
}

Mat4x4 mat4x4_lookat(vec3 eye, vec3 center, vec3 up){
    vec3 forward = vec3_normalize(vec3_distance(eye, center));
    vec3 right = vec3_normalize(vec3_cross_product(up, forward));
    up = vec3_normalize(vec3_cross_product(forward, right));

    Mat4x4 m = mat4x4_create_matrixv3(right, up, forward);
    return m;
}

Mat4x4 mat4x4_diablo_viewport(){
    Mat4x4 I = mat4x4_identity();
    float width = game_data.width / 2.f;
    float height = game_data.height / 2.f;
    I.m[0][0] = width;
    I.m[0][3] = width;

    I.m[1][1] = height;
    I.m[1][3] = height;

    return I;
}

Mat4x4 mat4x4_perspective(float fov, float dist){
    Mat4x4 I = mat4x4_identity();
    I.m[3][2] = -1 / fov;
    return I;
}

vec3 mat4x4_project(Mat4x4 projection, vec3 p){
    vec4 point = vec4_create_vec4_v3(p);
    point.w = 1;
    point = mat4x4_mulv(projection, point);
    point.x /= point.w; point.y /= point.w; point.z /= point.w;
    //printf("%f\n", point.z);
    return (vec3){point.x, point.y, point.z};
}

Mat4x4 mat4x4_view(vec3 camera_position, vec3 look_at){
    struct Camera c;
    init_camera(&c, camera_position);
    camera_lookat(&c, look_at);
    // vec3_print_vector3(c.right);
    // vec3_print_vector3(c.up);
    // vec3_print_vector3(c.forward);

    Mat4x4 m = mat4x4_create_matrixv4(
        vec4_create_vec4_v3(c.right), 
        vec4_create_vec4_v3(c.up), 
        vec4_create_vec4_v3(c.forward));
    
    Mat4x4 t = mat4x4_identity();
    t.m[0][3] = -camera_position.x;
    t.m[1][3] = -camera_position.y;
    t.m[2][3] = -camera_position.z;
    Mat4x4 k = mat4x4_mul(m, t);
    // mat4x4_print_mat4x4(m);
    // mat4x4_print_mat4x4(t);
    // mat4x4_print_mat4x4(k);

    return k;
}