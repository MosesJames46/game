#include "vector2.h"
#include "game_data.h"
#include <SDL2/SDL.h>

#ifndef DRAW
#define DRAW

//Draw to screen https://stackoverflow.com/questions/20579658/how-to-draw-pixels-in-sdl-2-0
//Opening a window: https://stackoverflow.com/questions/34424816/sdl-window-does-not-show

typedef struct Draw_Data{
    Game_Data window;
} Draw_Data;

void draw_line(vec2 u, vec2 v, Game_Data game_data);
vec2 screen_coordinate(vec2 u, Game_Data game_data);
void draw_point(vec2 u, Game_Data game_data);
#endif