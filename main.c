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
#include "headers/model.h"
#include "headers/game_string.h"


int main(int argc, char* argv []){
    bool initialized = init_game();
    //draw_line(u, v, window);
    //p1 = mat3x3_rotate((vec3){0, 180, 0}, p1);
    Model* model = model_read_file("/Users/mosesjames/Desktop/tests/game/obj/teapot.obj");
    float result = game_stof("398031.7");
    printf("%f\n", result);

    return 0;
}