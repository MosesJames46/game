#include "vector3.h"


#ifndef TRINAGLE_H
#define TRAINGLE_H
typedef struct Triangle{
    vec3 u;
    vec3 v;
    vec3 w;
}Triangle;

vec3 triangle_barycentric(Triangle t, vec3 p);

#endif