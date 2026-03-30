#ifndef VECTOR2
#define VECTOR2
#define VECTOR_OUTPUT "{%f, %f}"

typedef struct vec2{
    float x;
    float y;
}vec2;

vec2 vec2_scal(vec2 u, float s);
vec2 vec2_distance(vec2 u, vec2 v);
float vec2_magnitude(vec2 u);
vec2 vec2_create(float x, float y);
float vec2_determinant(vec2 u, vec2 v);

#endif