#include "../headers/Camera.h"

void camera_lookat(struct Camera* camera, vec3 lookat){
    vec3 forward_vec = vec3_distance(lookat, camera->position);
    camera->forward = vec3_normalize(forward_vec);
    //vec3_print_vector3(camera->forward);

    camera->right = vec3_normalize(vec3_cross_product(camera->up, camera->forward));
    camera->up = vec3_normalize(vec3_cross_product(camera->forward, camera->right));
}

void init_camera(struct Camera* camera, vec3 position){
    camera->position = position;
    camera->up = (vec3){0, 1, 0};
}