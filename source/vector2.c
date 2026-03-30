#include "../headers/vector2.h"
#include <math.h>

vec2 vec2_scal(vec2 u, float s){
    return (vec2){u.x * s, u.y * s};
};
vec2 vec2_distance(vec2 u, vec2 v){
    return (vec2){v.x - u.x, v.y - u.y};
}
float vec2_magnitude(vec2 u){
    return sqrtf(u.x * u.x + u.y * u.y);  
}

vec2 vec2_create(float x, float y){
    return (vec2){x, y};
}

float vec2_determinant(vec2 u, vec2 v){
    return u.x * v.y - u.y * v.x;
}