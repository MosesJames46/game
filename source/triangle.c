#include "../headers/triangle.h"
#include "../headers/Matrix3x3.h"
#include <stdio.h>


vec3 triangle_barycentric(Triangle t, vec3 p){
    /*
        Obtains weighted values that determine how much a point P lies withing triangle t.

        Barycentric coordinates are typically expressed as the following : a + b + c = 1 where 
        a, b, and c represent alpha, beta, and gamma respectively.
    */
    //printf("area for ABC: %f\n", ABC_area);

    /*
        We can obtain individual areas of the triangles made by the point and the trinagle's points.
    */

    Mat3x3 M = mat3x3_create_matrix(t.u, t.v, t.w);
    //mat3x3_print_mat3x3(M);
    M = mat3x3_inverse(M);
    //mat3x3_print_mat3x3(M);
    printf("\n\n\n");
    vec3 r = mat3x3_mulv(M, p);
    vec3_print_vector3(r);
    printf("\n\n");
    return r;
}