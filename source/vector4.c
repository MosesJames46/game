#include "../headers/vector4.h"
#include <stdio.h>
#include <math.h>

vec4 vec4_scale(vec4 u, float s){
    return (vec4){u.x * s,u.y * s, u.z * s, u.w * s};
}

vec4 vec4_distance(vec4 u, vec4 v){
    return (vec4){v.x - u.x, v.y - u.y, v.z - u.z, v.w - u.w};
}

float vec4_magnitude(vec4 u){
    return sqrtf((u.x * u.x +  u.y * u.y +  u.z * u.z + u.w * u.w));
}


vec4 vec4_crossproduct(vec4 u, vec4 v){
    return (vec4){v.y * u.z - v.z * u.y, 
                  v.z * u.x - v.x * u.z, 
                  v.x * u.y - v.y * u.x, 0};
}

vec4 vec4_create_vecf(float x, float y, float z, float w){
    return (vec4){x, y, z, w};
}
vec4 vec4_create_vec4_v3(vec3 u){
    return (vec4){u.x, u.y, u.z, 0};
}

void vec4_print_vector4(vec4 u){
    printf(VECTOR4_OUTPUT, u.x, u.y, u.z, u.w);
}
