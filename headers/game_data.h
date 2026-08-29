#include <SDL2/SDL.h>
#include <stdbool.h>
#ifndef GAME_DATA_H
#define GAME_DATA_H

#define WINDOW_WIDTH 300;
#define WINDOW_HEIGHT 300;

//Using extern: https://www.linkedin.com/pulse/back-basics-how-properlly-extern-struct-cc-gurajapu-raja-sumant
//Using extern: https://stackoverflow.com/questions/3041797/how-to-use-a-defined-struct-from-another-source-file

typedef struct Game_Data{
    int width;
    int height;
    float** z_buffer;
    SDL_Window* window;
    SDL_Renderer* renderer;
}Game_Data;

extern struct Game_Data game_data;

bool init_game();


#endif