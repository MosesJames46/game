#include "vector3.h"
#include "game_data.h"
#include "../headers/triangle.h"
#include <SDL2/SDL.h>

#ifndef DRAW
#define DRAW
#define POINTS2D "%f, %f"

extern struct Object;

//Draw to screen https://stackoverflow.com/questions/20579658/how-to-draw-pixels-in-sdl-2-0
//Opening a window: https://stackoverflow.com/questions/34424816/sdl-window-does-not-show

typedef struct Draw_Data{
    Game_Data window;
} Draw_Data;

void draw_line(vec3 u, vec3 v);
void draw_lines(Triangle t);
vec3 display_coordinate(vec3 u);
void draw_pointc(vec3 u, vec3 color);
void draw_point(vec3 u);
vec3 basic_projection(vec3 u);
vec3 screen_coordinate(vec3 point);
vec3 NDC_coordinate(vec3 point);
vec3 world_coordinate(vec3 point);

vec3 project(vec3 A);
//Functions used to fill lines
void draw_bounding_box2D(Triangle t, float bound_box[4]);
void draw_rasterize(Triangle t, float bounding_box[4]);
void draw_triangle(Triangle t);
void draw_object(struct Object* object, float size);
#endif