#ifndef __OGLSHADER_H__
#define __OGLSHADER_H__

#include "Rush/graphics/Shader.h"

#include "Rushpch.h"
#include <stdint.h>


namespace Rush {

class OGLShader : public Shader {
private:
    uint32_t m_Shader;
public:
    OGLShader(std::string shaderPath);
    ~OGLShader();
    OGLShader(OGLShader &) = delete;
    OGLShader(const OGLShader &) = delete;

    void Bind() override;
    void Unbind() override;
    void SetUniform(std::string name, ShaderData type, const void *data) override;
};
    
} // namespace Rush


#endif // __OGLSHADER_H__
