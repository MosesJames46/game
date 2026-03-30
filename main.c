#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <stdbool.h>
#include "headers/draw.h"
#include "headers/game_utility.h"
#include "headers/vector3.h"
#include "headers/Matrix3x3.h"
#include "headers/game_math.h"


int main(int argc, char* argv []){
    bool initialized = init_game();
    //draw_line(u, v, window);
    //p1 = mat3x3_rotate((vec3){0, 180, 0}, p1);
    
    Mat3x3 A;
    vec3 a = {5, 0, 0};

    vec3 ma = {-20, 0, 0};
    vec3 mb = {20, 0, 0};
    vec3 mc = {0, 20, 0};

    Mat3x3 B = mat3x3_create_matrix(ma, mb, mc);
    float det = mat3x3_determinant(B);

    //triangle_barycentric((Triangle){ma, mb, mc}, a);  
    //float n = gm_roundf(9.595);
    draw_triangle((Triangle){ma, mb, mc});
    
    return 0;
}