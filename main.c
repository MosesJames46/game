#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <stdbool.h>
#include "headers/draw.h"
#include "headers/object.h"
#include "headers/Matrix3x3.h"

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

    bool quit = (initialized) ? false : true;
    float time_now = SDL_GetTicks() / 1000;
    float time_before = time_now;
    while (!quit){
        //Clear screen
        SDL_SetRenderDrawColor(game_data.renderer, 0, 0, 0, 255);
        SDL_RenderClear(game_data.renderer);

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
        
        draw_object(object, 100);
        for (int i = 0; i < object->vb_size; i+=3){
            vec3 r = mat3x3_rotate((vec3){.5, .5, .5}, (vec3){object->vertex_buffer[i], object->vertex_buffer[i + 1], object->vertex_buffer[i + 2]});
            object->vertex_buffer[i] = r.x;
            object->vertex_buffer[i + 1] = r.y;
            object->vertex_buffer[i + 2] = r.z;
        }

        //if (dt > 360.f) dt = 0.0;
        SDL_RenderPresent(game_data.renderer);
    }
    
    return 0;
}