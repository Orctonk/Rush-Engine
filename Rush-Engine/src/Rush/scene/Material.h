#include "Rush/core/Core.h"
#include "Rush/graphics/Texture.h"
#include "Rush/graphics/Shader.h"

namespace Rush{

struct Material {
    Shared<Texture> diffuseTexture {nullptr};
    Shared<Texture> specularTexture {nullptr};
    Shared<Texture> normalTexture {nullptr};
    
    float shininess {1.0f};
};

}