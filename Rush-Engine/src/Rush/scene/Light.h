#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "Rush/core/Core.h"
#include <glm/glm.hpp>

enum class RUSH_API LightType {
    DIRECTIONAL,
    POINT,
    SPOTLIGHT
};

struct RUSH_API DirectionalLight {
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct RUSH_API PointLight {
    glm::vec3 position;

    float constant;
    float linear;
    float quadratic;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct RUSH_API Spotlight {
    glm::vec3 position;  
    glm::vec3 direction;

    float cutOff;
    float outerCutOff;
  
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

#endif // __LIGHT_H__
