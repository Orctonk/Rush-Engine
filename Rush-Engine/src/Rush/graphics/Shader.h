#ifndef __SHADER_H__
#define __SHADER_H__

#include "Rushpch.h"
#include "Rush/core/Core.h"
#include <stdint.h>

namespace Rush {

enum class RUSH_API ShaderData{
    FLOAT,
    FLOAT2,
    FLOAT3,
    FLOAT4,
    INT,
    INT2,
    INT3,
    INT4,
    BOOL,
    MAT3,
    MAT4
};

class RUSH_API Shader {
protected:
    std::string m_DebugPath;
    Shader(std::string shaderPath);
public:
    virtual ~Shader();
    Shader(Shader &) = delete;
    Shader(const Shader &) = delete;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual void SetUniform(std::string name, ShaderData type, const void *data) = 0;

    static Shared<Shader> Create(std::string shaderPath);
};
    
} // namespace Rush


#endif // __SHADER_H__
