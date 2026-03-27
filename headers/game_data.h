#include <SDL2/SDL.h>
#include <stdbool.h>
#ifndef GAME_DATA_H
#define GAME_DATA_H

#define WINDOW_WIDTH 800;
#define WINDOW_HEIGHT 600;

typedef struct Game_Data{
    int width;
    int height;
    SDL_Window* window;
    SDL_Renderer* renderer;
}Game_Data;

bool init_game(Game_Data* game_data);


#endif