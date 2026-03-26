#include "vector2.h"
#include "game_window.h"
#include <SDL2/SDL.h>

#ifndef DRAW
#define DRAW
void draw_line(vec2 u, vec2 v, game_window window, SDL_Renderer* renderer);
vec2 screen_coordinate(vec2 u, game_window window);
#endif