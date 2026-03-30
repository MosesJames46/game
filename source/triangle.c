#include "../headers/triangle.h"
#include "../headers/Matrix3x3.h"
#include "../headers/vector2.h"
#include <stdio.h>

vec3 triangle_barycentric(Triangle t, vec3 p){
    /*
        Obtains weighted values that determine how much a point P lies withing triangle t.

        Barycentric coordinates are typically expressed as the following : a + b + c = 1 where 
        a, b, and c represent alpha, beta, and gamma respectively.
    */

   //One mistake that kept confusing me was using 3D vectors to obtain the weighted values. 
   //It's simpler to make the points of the triangle each 2D vectors. 
    vec2 u = vec2_create(t.u.x, t.u.y);
    vec2 v = vec2_create(t.v.x, t.v.y);
    vec2 w = vec2_create(t.w.x, t.w.y);
    vec2 p2 = vec2_create(p.x, p.y);

    //Obtaining the distance between points yields the length of the triangles edges.
    //Here I obtain edge AB and BC.
    vec2 AB = vec2_distance(u, v);
    vec2 BC = vec2_distance(v, w);

    //Can't just use A = bh/2 because you might not have a 45 degree angle which corresponds to the height.
    //Giving this warning due to failed attempt at using standard area for a triangle.

    //The Method for obtaining the determinant of the edges of a triangle obtains the correct signed area for a parallelogram.
    //Halfing this value obtains the signed area for a triangle. 
    float ABC_area = vec2_determinant(AB, BC) * .5;

    vec2 AP = vec2_distance(p2, u);
    vec2 BP = vec2_distance(p2, v);
    vec2 CP = vec2_distance(p2, w);

    //Through constant debugging, I discovered that to find the corrected weight value variable, 
    //whihch ever edges we calculate the determinant for yields the component not incuded in the edge.
    //i.e BCP yields alpha, as alpha is not included. I am just not sure why.
    float BCP = vec2_determinant(BP, CP) * .5;
    float alpha = BCP / ABC_area;

    float ACP = vec2_determinant(CP, AP) * .5;
    float beta = ACP / ABC_area;

    float ABP_area = vec2_determinant(AP, BP) * .5;
    float gamma = ABP_area / ABC_area;
    
    return (vec3){alpha, beta, gamma};

}