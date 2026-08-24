#ifndef VECTOR3
#define VECTOR3
#define VECTOR3_OUTPUT "{%f, %f, %f}\n"

typedef struct vec3{
    float x;
    float y;
    float z;
}vec3;

vec3 vec3_scale(vec3 u, float s);
vec3 vec3_distance(vec3 u, vec3 v);
float vec3_magnitude(vec3 u);
vec3 vec3_normalize(vec3 u);
float vec3_dot(vec3 u,vec3 v);

vec3 vec3_cross_product(vec3 u, vec3 v);
vec3 vec3_create_vector3(float x, float y, float z);

void vec3_print_vector3(vec3 u);

#endif