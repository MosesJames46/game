#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "../headers/game_utility.h"
#include "../headers/game_math.h"
#include "../headers/draw.h"

/*
    
*/



void draw_line(vec2 u, vec2 v, Game_Data game_data){
    /*
        Because we want to perform the drawing operation left to right, we swap our points if and only if our first point's
        x position is larger than our second point's x position.
    */
    if (u.x > v.x){
        swapf(&u.x, &v.x);
        swapf(&u.y, &v.y);
    }

    bool steep = (v.x - u.x) < (v.y - u.y);
    /*
        Our draw function iterates over the range of x values. Because of this, if the slope is steep, that means that
        the line on the y axis for our 2 points is larger than the line on the x-axis for our 2 points. Iterating over x 
        will not give us enough unqiue points to match our y line. Therefore, we mimic a transpose, by swapping our x and y
        for the corresponsing points. i.e. taking our x-line and making it our y-line and vice versa. However, we must remember
        that if our slope is steep, we must place the respective x/y component back into the buffer/drawing order.  
    */
    if (steep){
        //Think of swap the maginitude of each points component.
        swapf(&u.x, &u.y);
        swapf(&v.x, &v.y);
    }
    

    float length_of_x = v.x - u.x;
    float length_of_y = v.y - u.y;

    for (float x = u.x; x < v.x; x++){
        //We obtain the length of each point's corresponding compenent with respect to their axis.
        //The length of the line on the x-axis is: v.x - u.x
        //The length of the line on the y-axis is: v.y - u.y
        //t represents the amount currently traversed on the x line. This is basically linear interpolation.
        /*
            x - u.x will equal v.x - u.x only when x == u.x. But since x < v.x is the termination factor of our for loop,
            this will never happen.
        */
        float t = (x - u.x) / length_of_x;

        /*
            Start at u.y and increase only by percentages of our y line. 
        */
        float y = gm_roundf(u.y + length_of_y * t);
        if (steep){
            //Draw points
            draw_point((vec2){y, x}, game_data);
        }else{
            //Draw points
            draw_point((vec2){x, y}, game_data);
        }
    }

    //printf(VECTOR_OUTPUT " " VECTOR_OUTPUT "\n", u.x, u.y, v.x, v.y);
}

vec2 screen_coordinate(vec2 u, Game_Data game_data){
    //(width / 2) + point
    int screen_position_x = u.x + (game_data.width / 2.f);
    int screen_position_y = -u.y + (game_data.height / 2.f);
    return (vec2){screen_position_x, screen_position_y};
}

void draw_point(vec2 u, Game_Data game_data){
    //Set to screen coordinates.
    vec2 sc = screen_coordinate(u, game_data);
    //set point color. Call SDL_SetRenderDrawColor.
    SDL_SetRenderDrawColor(game_data.renderer, 255, 0, 0, 255);
    //draw point on screen. Call SDL_RenderDrawPoint.
    SDL_RenderDrawPoint(game_data.renderer, sc.x, sc.y);
}