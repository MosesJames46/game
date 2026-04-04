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
#include "headers/gamevf_vector.h"

#include <limits.h>

int main(int argc, char* argv []){
    bool initialized = init_game();
    //draw_line(u, v, window);
    //p1 = mat3x3_rotate((vec3){0, 180, 0}, p1);
    Model* model = model_read_file("/Users/mosesjames/Desktop/tests/game/obj/teapot.obj");
    // game_vectorf vector = gamevf_init(0);
    // for (int i = 0; i < 2000; i++){
    //     gamevf_push_back(&vector, 20);
    //      //printf("\n");
    // //     //gamevf_size(vector);
    // //     //printf("\n");
    // //     //printf("%d", vector.capacity);
    // //     //gamevf_print(&vector);
    // //     //printf("\n");
    //  }
    // gamevf_print(&vector);
    //game_stringy string = game_create_stringy("-.9897217");
    //game_print_stringy(string);printf("\n");
    //float ans = game_stof(string);
    //printf("%f", ans);
    return 0;
}