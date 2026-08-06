#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "../headers/game_utility.h"
#include "../headers/game_math.h"
#include "../headers/draw.h"
#include "../headers/Matrix3x3.h"
#include "../headers/object.h"



struct Game_Data game_data;

void draw_line(vec3 u, vec3 v){
    

    bool steep = gm_roundf(fabsf(v.x - u.x)) < gm_roundf(fabsf(v.y - u.y));

    if (steep){
        swapf(&u.x, &u.y);
        swapf(&v.x, &v.y);
    }

    if (u.x > v.x){
        swapf(&u.x, &v.x);
        swapf(&u.y, &v.y);
    }
    

    float length_of_x = v.x - u.x;
    float length_of_y = v.y - u.y;

    for (float x = u.x; x < v.x; x++){
        float t = (x - u.x) / length_of_x;
        float y = gm_roundf(u.y + length_of_y * t);
        if (steep){
            draw_point((vec3){y, x, 0});
        }else{
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
    u = basic_projection(u);
    u = NDC_coordinate(u);
    
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
    
    return u;
}

void draw_triangle(Triangle t){
    /*
        First obtain the bounding box of the triandle.
    */
    //Before actually drawing anything, we should first project our points within the space they'll be in.
    //t.u = basic_projection(t.u);
    //t.v = basic_projection(t.v);
    //t.w = basic_projection(t.w);

    float bounding_box[4];
    draw_bounding_box2D(t, bounding_box);

    /*
        Obtain the barycentric coodinate of every point within the bounding box.
        If weight is non-negative draw point.
    */
   draw_rasterize(t, bounding_box);
}

void draw_object(struct Object* object, float scale){

    for (int i = 0; i < object->vb_size; i+=9){
        vec3 a = (vec3){object->vertex_buffer[i] * scale, object->vertex_buffer[i + 1] * scale, object->vertex_buffer[i + 2]};
        vec3 b = (vec3){object->vertex_buffer[i + 3] * scale, object->vertex_buffer[i + 4] * scale, object->vertex_buffer[i + 5]};
        vec3 c = (vec3){object->vertex_buffer[i + 6] * scale, object->vertex_buffer[i + 7] * scale, object->vertex_buffer[i + 8]};

        Triangle t = (Triangle){a, b, c};
        draw_triangle(t);
    }
}

void draw_bounding_box2D(Triangle t, float bound_box[4]){
    /*
        Gets bounding box given a triangle object.
    */
    //x
    float min_x;
    min_x = gm_minf(t.A.x, t.B.x);
    min_x = gm_minf(min_x, t.C.x);

    float max_x;
    max_x = gm_maxf(t.A.x, t.B.x);
    max_x = gm_maxf(max_x, t.C.x);

    //y
    float min_y;
    min_y = gm_minf(t.A.y, t.B.y);
    min_y = gm_minf(min_y, t.C.y);

    float max_y;
    max_y = gm_maxf(t.A.y, t.B.y);
    max_y = gm_maxf(max_y, t.C.y);
    bound_box[0] = min_x;
    bound_box[1] = max_x;
    bound_box[2] = min_y;
    bound_box[3] = max_y;
    //printf("x values: "POINTS2D "y values: "POINTS2D"\n", min_x, max_x, min_y, max_y );
}

void draw_rasterize(Triangle t, float bounding_box[4]){
    // draw_line(t.A, t.B);
    // draw_line(t.B, t.C);
    // draw_line(t.C, t.A);
    int x_min = gm_maxi(bounding_box[0], -(game_data.width/2));
    int x_max = gm_mini(bounding_box[1], game_data.width / 2);
    int y_min = gm_maxi(bounding_box[2], -game_data.height / 2);
    int y_max = gm_mini(bounding_box[3], game_data.height/2);

    //if (area < 1) return;

    vec3 temp;
    float area = signed_area(t);
    float z;
    for (int x = x_min; x <= x_max; x++){
        for (int y = y_min; y <= y_max; y++){
            vec3 p = (vec3){x, y, 0};

            // float ABP = signed_area((Triangle){t.A, t.B, p});
            // float BCP = signed_area((Triangle){t.B, t.C, p});
            // float CAP = signed_area((Triangle){t.C, t.A, p});

            float ABP = signed_triangle_area((Triangle){t.A, t.B, p});
            float BCP = signed_triangle_area((Triangle){t.B, t.C, p});
            float CAP = signed_triangle_area((Triangle){t.C, t.A, p});
            //z = alpha * t.A.z + beta * t.B.z + gamma * t.C.z;
            if (ABP >= 0 && BCP >= 0 && CAP >= 0){
                float weight_A = BCP / area;
                float weight_B = CAP / area;
                float weight_C = ABP / area;
                vec3 color = {255, 255, 255};
                z = t.A.z * weight_A + t.B.z * weight_B + t.C.z * weight_C;
                color.x *= z; color.y *= z; color.z *=z;
                temp = NDC_coordinate((vec3){x, y, 0});
                temp = basic_projection(temp);
                temp = screen_coordinate(temp);
                //printf("%f\n", z);
                //draw_pointc((vec3){x, y, 1}, color);
                //draw_pointc((vec3){x, y, 1}, color);
                //printf("%f %f\n", temp.x, temp.y);
                if (z > 0 && z >= game_data.z_buffer[(int)temp.x][(int)temp.y]) {
                    draw_pointc((vec3){x, y, 1}, color);
                    game_data.z_buffer[(int)temp.x][(int)temp.y] = z;
                }
            }
        }
    }

    for (int x = x_min; x <= x_max; x++){
        for (int y = y_min; y <= y_max; y++){
            //printf("%f\n", game_data.z_buffer[(int)temp.x][(int)temp.y]);
            temp = NDC_coordinate((vec3){x, y, 0});
            temp = basic_projection(temp);
            temp = screen_coordinate(temp);
            game_data.z_buffer[(int)temp.x][(int)temp.y] = -1;
        }
    }
}
