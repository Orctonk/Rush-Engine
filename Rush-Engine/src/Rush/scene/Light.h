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
    glm::vec3 direction {0.0f,0.0f,0.0f};

    glm::vec3 ambient {0.1f,0.1f,0.1f};
    glm::vec3 diffuse {1.0f,1.0f,1.0f};
    glm::vec3 specular{1.0f,1.0f,1.0f};
};

struct RUSH_API PointLight {
    glm::vec3 position  {0.0f,0.0f,0.0f};

    float constant  {0.7f};
    float linear    {0.07f};
    float quadratic {0.02f};

    glm::vec3 ambient {0.1f,0.1f,0.1f};
    glm::vec3 diffuse {1.0f,1.0f,1.0f};
    glm::vec3 specular{1.0f,1.0f,1.0f};
};

struct RUSH_API Spotlight {
    glm::vec3 position {0.0f,0.0f,0.0f};  
    glm::vec3 direction{0.0f,0.0f,0.0f};

    float cutOff;
    float outerCutOff;
  
    glm::vec3 ambient {0.1f,0.1f,0.1f};
    glm::vec3 diffuse {1.0f,1.0f,1.0f};
    glm::vec3 specular{1.0f,1.0f,1.0f};
};

#endif // __LIGHT_H__
