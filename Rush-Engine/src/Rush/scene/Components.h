#ifndef __COMPONENTS_H__
#define __COMPONENTS_H__

#include "Camera.h"
#include <glm/glm.hpp>

struct TransformComponent {
    glm::vec3 translation   {0.0f,0.0f,0.0f};
    glm::vec3 rotation      {0.0f,0.0f,0.0f};
    glm::vec3 scale         {1.0f,1.0f,1.0f};
};

struct CameraComponent {
    bool main;
    Rush::Camera camera;

    CameraComponent() 
        : main(false), camera(1024.0f/720.0f,45.0f) { }

    CameraComponent(bool main, Rush::Camera cam) 
        : main(main), camera(cam) { }
};

#endif // __COMPONENTS_H__
