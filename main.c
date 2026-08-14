#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <stdbool.h>
#include "headers/draw.h"
#include "headers/object.h"
#include "headers/Camera.h"
#include <time.h>
#include "headers/Matrix4.h"

//"/usr/local//opt/libomp/include"

/*
    GCC compiling SDL

    https://www.reddit.com/r/cpp_questions/comments/1g2vsj7/how_are_you_supposed_to_use_a_library_like_sdl2/
*/


int main(int argc, char* argv []){
    bool initialized = init_game();
    SDL_Event e;
    const float delta = 1.f/60.f;

    float dt = 0;
    float z_axis = 0;
    float current_time = SDL_GetTicks() / 1000;

    struct Object* object = init_object("diablo3_pose.obj", "rb");

    Mat4x4 M = mat4x4_identity();
    M.m[0][0] = 190; M.m[0][1] = 34; M.m[0][2] = -91; M.m[0][3] = 43;
    M.m[1][0] = 0; M.m[1][1] = -5; M.m[1][2] = .235; M.m[1][3] = 135;
    M.m[2][0] = -90.23; M.m[2][1] = -.935; M.m[2][2] = 945; M.m[2][3] = 1;
    M.m[3][0] = 0; M.m[3][1] = 283; M.m[3][2] = 10; M.m[3][3] = -3;
    Mat4x4 M_i = mat4x4_inverse(M);
    Mat4x4 I = mat4x4_mul(M_i, M);

    bool quit = (initialized) ? false : true;
    float time_now = SDL_GetTicks() / 1000;
    float time_before = time_now;
    struct Camera cam;
    init_camera(&cam, (vec3){0, 0, 0});
    while (!quit){
        //Clear screen
        clock_t before = clock();
        SDL_SetRenderDrawColor(game_data.renderer, 0, 0, 0, 255);
        SDL_RenderClear(game_data.renderer);
        //Reset z-buffer every frame.
        #pragma omp parallel for
        for (int i =0; i < game_data.width; i++){
            for (int j = 0; j < game_data.height; j++){
                game_data.z_buffer[i][j] = 0;
            }
        }

        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                quit = true;
            }
            if (e.type == SDL_KEYDOWN){
                quit = true;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN){
                quit = true;
            }
        }
        
        //draw_object(object, 150);
        SDL_RenderPresent(game_data.renderer);
        clock_t after = clock();
        double time_spent = (double)(after - before) / CLOCKS_PER_SEC;
        //printf("Time elapsed: %f\n", time_spent);
    }
    
    return 0;
}