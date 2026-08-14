#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "../headers/game_utility.h"
#include "../headers/game_math.h"
#include "../headers/draw.h"
#include "../headers/object.h"
#include <time.h>
#include "../headers/Camera.h"
#include "../headers/Matrix4.h"
#include "/usr/local/opt/libomp/include/omp.h"


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
    //u = basic_projection(u);
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

void draw_object(struct Object* object, float t){
    //PRINT("%f ", object->vertex_buffer, object->vb_size);
    static float current = 0;

    float axis[3] = {0, 1, 0};
    //rotate_object(object, axis);
    clock_t before  = clock();
    #pragma omp parallel for
    struct Camera c;
    vec3 camera_position = (vec3){1, 0, -1};
    vec3 center = (vec3){0, 0, 0};
    vec3 up = (vec3){0, 1, 0};
    Mat4x4 view = mat4x4_view(camera_position, center);
    // Mat4x4 I =mat4x4_identity();
    // I.m[0][3] = -center.x; I.m[1][3] = -center.y; I.m[2][3] = -center.z;
    // view = mat4x4_mul(view, I);

    vec3 obj_position = (vec3){0, 0, 0};
    Mat4x4 model = mat4x4_identity();
    //Mat4x4 view = mat4x4_model_view(camera_position, center);
    //Mat4x4 view = mat4x4_diablo_viewport();
    Mat4x4 p = mat4x4_perspective(3, vec3_magnitude(vec3_distance(camera_position, center)));
    //Mat4x4 pv = mat4x4_mul(p, v);
    //mat4x4_print_mat4x4(m);
    //vec3_print_vector3(c.right); vec3_print_vector3(c.up); vec3_print_vector3(c.forward);
    float scale = 100;
    for (int i = 0; i < object->vb_size; i+=9){
        vec3 a = (vec3){object->vertex_buffer[i], object->vertex_buffer[i + 1], object->vertex_buffer[i + 2]};
        vec3 b = (vec3){object->vertex_buffer[i + 3], object->vertex_buffer[i + 4], object->vertex_buffer[i + 5]};
        vec3 c = (vec3){object->vertex_buffer[i + 6], object->vertex_buffer[i + 7], object->vertex_buffer[i + 8]};
        

        a = mat4x4_scale(a, scale);
        b = mat4x4_scale(b, scale);
        c = mat4x4_scale(c, scale);

        a.z /= scale;
        b.z /= scale;
        c.z /= scale;

        // a = project(a);
        // b = project(b);
        // c = project(c);

        a = mat4x4_translate(a, obj_position);
        b = mat4x4_translate(b, obj_position);
        c = mat4x4_translate(c, obj_position);


        a = mat4x4_mulv3(view, a);
        b = mat4x4_mulv3(view, b);
        c = mat4x4_mulv3(view, c);
        
        Triangle t = (Triangle){a, b, c};
        draw_triangle(t);
        //draw_lines(t);
    }
    clock_t after = clock();
    printf("TIme elapsed in drawing: %f\n", (double)(after - before) / CLOCKS_PER_SEC);
}

vec3 project(vec3 p){
    float c = 3.f;

    float x = p.x / (1 - p.z / c);
    float y = p.y / (1 - p.z / c);
    float z = p.z / (1 - p.z / c);
    return (vec3){x, y, p.z};
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
    float area = signed_triangle_area(t);

    //If the area is less than 1, then the triangle is small. It could also be negative which means
    //its a backwards facing triangle.
    //if (area < 1) return;

    int x_min = gm_maxi(bounding_box[0], -(game_data.width/2));
    int x_max = gm_mini(bounding_box[1], game_data.width / 2);
    int y_min = gm_maxi(bounding_box[2], -game_data.height / 2);
    int y_max = gm_mini(bounding_box[3], game_data.height/2);

    
    vec3 screen_coordinates;
    float z;
    #pragma omp parallel for
    clock_t before = clock();
    for (int x = x_min; x <= x_max; x++){
        for (int y = y_min; y <= y_max; y++){
            vec3 p = (vec3){x, y, 0};

            float alpha = signed_triangle_area((Triangle){t.B, t.C, p}) / area;
            float beta = signed_triangle_area((Triangle){t.C, t.A, p}) / area;
            float gamma = signed_triangle_area((Triangle){t.A, t.B, p}) / area;

            if (alpha < 0 || beta < 0 || gamma < 0) continue;
            //if (alpha >= 0 || beta >= 0 || gamma >= 0) continue;

            vec3 color = {255, 255, 255};
            //Interpolated z-value applied to color
            z = t.A.z * alpha + t.B.z * beta + t.C.z * gamma;
            color.x *= z; color.y *= z; color.z *= z;

            //Get screen coordinate position which relates to z-buffer value.
            screen_coordinates = NDC_coordinate((vec3){x, y, 0});
            screen_coordinates = basic_projection(screen_coordinates);
            screen_coordinates = screen_coordinate(screen_coordinates);

            //Ensure screen_coordinate is within range of viewport
            if ((int)screen_coordinates.x >= game_data.width || (int)screen_coordinates.y >= game_data.height) continue;
            if (z >= game_data.z_buffer[(int)screen_coordinates.x][(int)screen_coordinates.y] && z < 1){
                draw_pointc((vec3){x, y, 1}, color);
                game_data.z_buffer[(int)screen_coordinates.x][(int)screen_coordinates.y] = z;
            }
        }
    }
    clock_t after = clock();
    //printf("Time elapsed in drawing: %f\n", ((double)after - before) / CLOCKS_PER_SEC);
    
}

void draw_lines(Triangle t){
    draw_line(t.A, t.B);
    draw_line(t.B, t.C);
    draw_line(t.C, t.A);
}