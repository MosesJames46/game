#include "../headers/Camera.h"

void camera_lookat(struct Camera* camera, vec3 position, vec3 lookat, vec3 up){
    //https://community.khronos.org/t/confused-when-using-glm-for-projection/108548/2?utm_source=chatgpt.com

    //The handedness of a coordinate system must be considered when building a 
    //graphics pipeline. The transformation must also be aware of these transformations too.

    /*
        Camera should be looking down the -z axis. 
    */
    camera->position = position;
    camera->up = up;
    vec3 forward_vec = vec3_distance(camera->position, lookat);
    //camera->forward = vec3_normalize(forward_vec);
    camera->forward = (vec3){0, 0, -1};
    //vec3_print_vector3(camera->forward);

    camera->right = vec3_normalize(vec3_cross_product(camera->forward, camera->up));
    
    camera->up = vec3_normalize(vec3_cross_product(camera->right, camera->forward));
    //camera->forward = vec3_normalize(vec3_cross_product(camera->right, camera->forward));
    //vec3_print_vector3(camera->right);
    //vec3_print_vector3(camera->up);
}
