#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include <glm/glm.hpp>

struct Transform {
    glm::vec3 translation   {0.0f,0.0f,0.0f};
    glm::vec3 rotation      {0.0f,0.0f,0.0f};
    glm::vec3 scale         {1.0f,1.0f,1.0f};
};

#endif // __TRANSFORM_H__
