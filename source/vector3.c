#include "../headers/vector3.h"
#include <stdio.h>
#include <math.h>

vec3 vec3_scal(vec3 u, float s){
    return (vec3){u.x * s,u.y * s, u.z * s};
}

vec3 vec3_distance(vec3 u, vec3 v){
    return (vec3){v.x - u.x, v.y - u.y, v.z - u.z};
}

float vec3_magnitude(vec3 u){
    return sqrtf((u.x * u.x +  u.y * u.y +  u.z * u.z));
}


vec3 vec3_crossproduct(vec3 u, vec3 v){
    return (vec3){-v.y * u.z + v.z * u.y, 
                  -v.z * u.x + v.x * u.z, 
                  -v.x * u.y + v.y * u.x};
}

vec3 vec3_create_vector3(float x, float y, float z){
    return (vec3){x, y, z};
}

void vec3_print_vector3(vec3 u){
    printf(VECTOR3_OUTPUT, u.x, u.y, u.z);
}
