#include "../headers/triangle.h"
#include "../headers/Matrix3x3.h"
#include "../headers/vector2.h"
#include <stdio.h>

float signed_area(Triangle t){
    return ((t.B.x - t.A.x)*(t.C.y - t.A.y) - (t.B.y - t.A.y)*(t.C.x - t.A.x));
}

float signed_triangle_area(Triangle t){
    vec3 A = t.A;
    vec3 B = t.B;
    vec3 C = t.C;
    
    //Look up shoelace method for performing signed area of a polygon.
    return ((B.y - A.y)*(B.x + A.x) + (C.y - B.y)*(C.x + B.x) + (A.y - C.y)*(A.x + C.x));
}