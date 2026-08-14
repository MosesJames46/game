#include "vector3.h"

#ifndef CAMERA_H
#define CAMERA_H

struct Camera{
    vec3 position;
    vec3 up;
    vec3 forward;
    vec3 right;
};

extern struct Camera camera;

void camera_lookat(struct Camera* camera, vec3 lookat);
void init_camera(struct Camera* camera, vec3 position);


#endif