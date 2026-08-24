#include "vector3.h"

#ifndef VECTOR4
#define VECTOR4
#define VECTOR4_OUTPUT "{%f, %f, %f, %f}\n"

typedef struct vec4{
    float x;
    float y;
    float z;
    float w;
}vec4;

vec4 vec4_scale(vec4 u, float s);
vec4 vec4_distance(vec4 u, vec4 v);
float vec4_magnitude(vec4 u);

vec4 vec4_crossproduct(vec4 u, vec4 v);
vec4 vec4_create_vecf(float x, float y, float z, float w);
vec4 vec4_create_vec4_v3(vec3 u);

void vec4_print_vector4(vec4 u);

#endif