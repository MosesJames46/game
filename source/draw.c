#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "../headers/game_utility.h"
#include "../headers/game_math.h"
#include "../headers/draw.h"
#include "../headers/Matrix3x3.h"



struct Game_Data game_data;

void draw_line(vec3 u, vec3 v){

    /*
        Because we want to perform the drawing operation left to right, we swap our points if and only if our first point's
        x position is larger than our second point's x position.
    */

    if (u.x > v.x){
        swapf(&u.x, &v.x);
        swapf(&u.y, &v.y);
        //swapf(&u.z, &v.z);
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
    //float length_of_z = u.z - v.z;

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
            draw_point((vec3){y, x, 0});
        }else{
            //Draw points
            draw_point((vec3){x, y, 0});
        }
    }
}

vec3 display_coordinate(vec3 u){
    //(width / 2) + point
    int screen_position_x = u.x + (game_data.width / 2.f);
    int screen_position_y = -u.y + (game_data.height / 2.f);
    return (vec3){screen_position_x, screen_position_y, u.z};
}

vec3 screen_coordinate(vec3 point){
    /*
        NDC coordinates range from -1 -> 1.
        Given a point in NDC to screen coordinates, we can add 1 to any component giving our range: 0 -> 2
        Next, we divide that result by 2 and multiplying by our width, we obtain our x.
        Y is a bit different. Since it ranges from 0 at the top to width at the bottom.
        If we want to be at the very top, say NDC 1, 1 - 1, we get 0. 
        If we want to be at the bottom, NDC -1, 1 - 0 becomes 1.
    */

   //Very huge deal to actually round the values being sent into the screen. Fixes any rasterization issues.
    float x = gm_roundf(((1 + point.x) / 2) * game_data.width);
    float y = gm_roundf((1 - (point.y + 1) / 2 ) * game_data.height);
    //printf(VECTOR3_OUTPUT"\n", x, y, point.z);
    return (vec3){x, y, point.z};
}

vec3 NDC_coordinate(vec3 point){
    float game_width = game_data.width / 2.f;
    float game_height = game_data.height / 2.f;
    float x = point.x / game_width;
    float y = point.y / game_height;
    return (vec3){x, y, point.z};
}

void draw_pointc(vec3 u, vec3 color){
    //Set to screen coordinates.
    u = NDC_coordinate(u);
    u = basic_projection(u);
    u = screen_coordinate(u);
    //set point color. Call SDL_SetRenderDrawColor.
    SDL_SetRenderDrawColor(game_data.renderer, color.x, color.y, color.z, 255);
    //draw point on screen. Call SDL_RenderDrawPoint.
    SDL_RenderDrawPoint(game_data.renderer, u.x, u.y);
}

void draw_point(vec3 u){
    //Set to screen coordinates.
    u = NDC_coordinate(u);
    u = basic_projection(u);
    u = screen_coordinate(u);
    //set point color. Call SDL_SetRenderDrawColor.
    SDL_SetRenderDrawColor(game_data.renderer, 255, 255, 255, 255);
    //draw point on screen. Call SDL_RenderDrawPoint.
    SDL_RenderDrawPoint(game_data.renderer, u.x, u.y);
}

vec3 world_coordinate(vec3 point){
    point.x *= (game_data.height / 2.f);
    point.y *= (game_data.width / 2.f);
    return (vec3){point.x, point.y, point.z};
}

vec3 basic_projection(vec3 u){
    if (u.z != 0){
        u.x /= u.z;
        u.y /= u.z;
    }
    //printf(VECTOR3_OUTPUT"\n", u.x, u.y, u.z);
    return u;
}

void draw_triangle(Triangle t){
    /*
        First obtain the bounding box of the triandle.
    */
    //draw_line(t.u, t.v);
    //draw_line(t.v, t.w);
    //draw_line(t.w, t.u);
    float bounding_box[4];
    draw_bounding_box2D(t, bounding_box);

    /*
        Obtain the barycentric coodinate of every point within the bounding box.
        If weight is non-negative draw point.
    */
   draw_rasterize(t, bounding_box);
}

void draw_bounding_box2D(Triangle t, float bound_box[4]){
    /*
        Gets bounding box given a triangle object.
    */
    //x
    float min_x;
    min_x = gm_minf(t.u.x, t.v.x);
    min_x = gm_minf(min_x, t.w.x);

    float max_x;
    max_x = gm_maxf(t.u.x, t.v.x);
    max_x = gm_maxf(max_x, t.w.x);

    //y
    float min_y;
    min_y = gm_minf(t.u.y, t.v.y);
    min_y = gm_minf(min_y, t.w.y);

    float max_y;
    max_y = gm_maxf(t.u.y, t.v.y);
    max_y = gm_maxf(max_y, t.w.y);
    bound_box[0] = min_x;
    bound_box[1] = max_x;
    bound_box[2] = min_y;
    bound_box[3] = max_y;
    //printf("x values: "POINTS2D "y values: "POINTS2D"\n", min_x, max_x, min_y, max_y );
}

void draw_rasterize(Triangle t, float bounding_box[4]){
    int x_min = bounding_box[0];
    int x_max = bounding_box[1];
    int y_min = bounding_box[2];
    int y_max = bounding_box[3];
    float epsilon = .1f;
    for (int x = x_min; x < x_max; x++){
        for (int y = y_min; y <= y_max; y++){
            vec3 p = triangle_barycentric(t, (vec3){x, y, 1});
            vec3 color = {255, 255, 255};
            float result = p.x + p.y + p.z;
            color.x *= p.x; color.y *= p.y; color.z *= p.z;
            //printf("%d, %d\n", x, y);
            if (p.x >= 0 && p.y >= 0 && p.z >= 0){
                draw_pointc((vec3){x, y, 0}, color);
            }
        }
    }
    //printf("Count is: %d", count);
}
