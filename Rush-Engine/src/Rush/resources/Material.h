#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "Rush/core/Core.h"
#include "Rush/graphics/Texture.h"
#include "Rush/graphics/Shader.h"
#include "Path.h"

#include <glm/glm.hpp>

namespace Rush{

enum class RenderingMode{
    Opaque,
    Transparent,
    Cutoff
};

class Material {
private:
    std::string m_Name;
public:
    Shared<Shader> materialShader;
    RenderingMode mode              {RenderingMode::Opaque};
    
    glm::vec4 color                 {1.0f,1.0f,1.0f,1.0f};
    Shared<Texture> diffuseTexture  {nullptr};
    Shared<Texture> specularTexture {nullptr};
    Shared<Texture> normalTexture   {nullptr};
    
    float shininess                 {1.0f};

    std::string GetName(){ return m_Name; }
    void Bind();

    static void Write(Shared<Material> mat, Path matPath);
    static Shared<Material> Load(Path matPath);
};

}

#endif // __MATERIAL_H__
