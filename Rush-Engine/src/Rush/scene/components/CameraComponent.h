#ifndef __CAMERACOMPONENT_H__
#define __CAMERACOMPONENT_H__

#include "../Camera.h"
#include "Rush/graphics/Cubemap.h"

#include <glm/glm.hpp>

struct CameraComponent {
    bool main;
    glm::vec4 clearColor;
    Rush::Shared<Rush::Cubemap> skybox;
    Rush::Camera camera;

    CameraComponent() 
        : main(false), clearColor(0.0f,0.0f,0.0f,1.0f), skybox(nullptr), camera(1024.0f/720.0f,45.0f) { }

    CameraComponent(bool main, Rush::Camera cam) 
        : main(main), clearColor(0.0f,0.0f,0.0f,1.0f), skybox(nullptr), camera(cam) { }
};
#endif // __CAMERACOMPONENT_H__
