#ifndef __COMPONENTS_H__
#define __COMPONENTS_H__

#include "Camera.h"
#include <glm/glm.hpp>

enum class RUSH_API LightType {
    DIRECTIONAL,
    POINT,
    SPOTLIGHT
};

struct TransformComponent {
    glm::vec3 translation   {0.0f,0.0f,0.0f};
    glm::vec3 rotation      {0.0f,0.0f,0.0f};
    glm::vec3 scale         {1.0f,1.0f,1.0f};
};

struct CameraComponent {
    bool main;
    glm::vec4 clearColor;
    Rush::Camera camera;

    CameraComponent() 
        : main(false), clearColor(0.0f,0.0f,0.0f,1.0f), camera(1024.0f/720.0f,45.0f) { }

    CameraComponent(bool main, Rush::Camera cam) 
        : main(main), clearColor(0.0f,0.0f,0.0f,1.0f), camera(cam) { }
};

struct RUSH_API LightComponent {
    LightType type      {LightType::POINT};
    float constant      {1.0f};
    float linear        {0.7f};
    float quadratic     {1.8f};

    float cutOff        {35.0f};
    float outerCutOff   {45.0f};

    glm::vec3 ambient   {0.1f,0.1f,0.1f};
    glm::vec3 diffuse   {1.0f,1.0f,1.0f};
    glm::vec3 specular  {1.0f,1.0f,1.0f};
};

#endif // __COMPONENTS_H__
