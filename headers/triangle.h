#include "vector3.h"


#ifndef TRINAGLE_H
#define TRAINGLE_H
typedef struct Triangle{
    vec3 A;
    vec3 B;
    vec3 C;
}Triangle;

float triangle_barycentric(Triangle t);
float signed_area(Triangle t);
float signed_triangle_area(Triangle t);

#endif