#ifndef __OGLSHADER_H__
#define __OGLSHADER_H__

#include "Rush/graphics/Shader.h"

#include "Rushpch.h"
#include <stdint.h>


namespace Rush {

class OGLShader : public Shader {
private:
    uint32_t m_Shader;
    std::unordered_map<std::string,int> m_UniformCache;

    int GetUniformLocation(std::string name);
public:
    OGLShader(std::string shaderPath);
    ~OGLShader();
    OGLShader(OGLShader &) = delete;
    OGLShader(const OGLShader &) = delete;

    void Bind() override;
    void Unbind() override;
    void SetUniform(std::string name, ShaderData type, const void *data) override;
    void SetUniform(std::string name, int val) override;
    void SetUniform(std::string name, int val1, int val2) override;
    void SetUniform(std::string name, int val1, int val2, int val3) override;
    void SetUniform(std::string name, int val1, int val2, int val3, int val4) override;
    void SetUniform(std::string name, float val) override;
    void SetUniform(std::string name, glm::vec2 val) override;
    void SetUniform(std::string name, glm::vec3 val) override;
    void SetUniform(std::string name, glm::vec4 val) override;
    void SetUniform(std::string name, bool val) override;
    void SetUniform(std::string name, glm::mat3 val) override;
    void SetUniform(std::string name, glm::mat4 val) override;
};
    
} // namespace Rush


#endif // __OGLSHADER_H__
