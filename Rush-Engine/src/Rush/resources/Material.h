#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "Path.h"
#include "Rush/core/Core.h"
#include "Rush/graphics/Shader.h"
#include "Rush/graphics/Texture.h"
#include "Rush/graphics/UniformBuffer.h"

#include <glm/glm.hpp>

namespace Rush {

enum class RUSH_API RenderingMode {
    Opaque,
    Transparent,
    Cutoff
};

class RUSH_API Material {
private:
    struct MaterialBuffer {
        glm::vec4 color;
        float shininess;
    };

    std::string m_Name;
    MaterialBuffer m_MaterialBuffer;
    Shared<UniformBuffer> m_MaterialUniformBuffer;

public:
    Shared<Shader> materialShader;
    RenderingMode mode{ RenderingMode::Opaque };

    glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
    Shared<Texture> diffuseTexture{ nullptr };
    Shared<Texture> specularTexture{ nullptr };
    Shared<Texture> normalTexture{ nullptr };

    float shininess{ 25.0f };

    Material();
    ~Material();

    std::string GetName() { return m_Name; }
    void Bind();

    static void Write(Shared<Material> mat, Path matPath);
    static Shared<Material> Load(Path matPath);
};

} // namespace Rush

#endif // __MATERIAL_H__
