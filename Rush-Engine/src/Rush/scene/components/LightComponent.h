#ifndef __LIGHTCOMPONENT_H__
#define __LIGHTCOMPONENT_H__

#include "Rush/core/Core.h"

#include <glm/glm.hpp>

enum class RUSH_API LightType {
    DIRECTIONAL,
    POINT,
    SPOTLIGHT
};

struct RUSH_API LightComponent {
    LightType type      {LightType::POINT};
    bool manualAtten    {false};
    float range         {7.0f};
    float constant      {1.0f};
    float linear        {0.7f};
    float quadratic     {1.8f};

    float cutOff        {35.0f};
    float outerCutOff   {45.0f};

    glm::vec3 ambient   {0.1f,0.1f,0.1f};
    glm::vec3 diffuse   {1.0f,1.0f,1.0f};
    glm::vec3 specular  {1.0f,1.0f,1.0f};
};

#endif // __LIGHTCOMPONENT_H__
