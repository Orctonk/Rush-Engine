#ifndef __SHADER_H__
#define __SHADER_H__

#include "Rush/core/Core.h"
#include "Rush/Resources/Path.h"

#include <string>
#include <stdint.h>
#include <filesystem> 
#include <glm/glm.hpp>
#include <shaderc/shaderc.hpp>

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

enum class RUSH_API ShaderType {
    Vertex,
    Geometry,
    Fragment,
    None
};

class RUSH_API Shader {
private: 
    using SourceMap = std::unordered_map<ShaderType, std::string>;


    SourceMap LoadSource(Path shaderPath);
    void CompileSPIRV(SourceMap sources);
    void Reflect();

protected:
    std::string m_DebugName;
    std::filesystem::file_time_type m_SourceModTime;
    std::unordered_map<ShaderType, std::vector<uint32_t>> m_SPIRVBinaries;

    Shader(std::string shaderPath);
    shaderc_shader_kind TypeToShaderc(ShaderType type);
    std::string TypeToString(ShaderType type);
public:
    virtual ~Shader();
    Shader(Shader &) = delete;
    Shader(const Shader &) = delete;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual void SetUniform(std::string name, ShaderData type, const void *data) = 0;
    virtual void SetUniform(std::string name, int val) = 0;
    virtual void SetUniform(std::string name, int val1, int val2) = 0;
    virtual void SetUniform(std::string name, int val1, int val2, int val3) = 0;
    virtual void SetUniform(std::string name, int val1, int val2, int val3, int val4) = 0;
    virtual void SetUniform(std::string name, float val) = 0;
    virtual void SetUniform(std::string name, glm::vec2 val) = 0;
    virtual void SetUniform(std::string name, glm::vec3 val) = 0;
    virtual void SetUniform(std::string name, glm::vec4 val) = 0;
    virtual void SetUniform(std::string name, bool val) = 0;
    virtual void SetUniform(std::string name, glm::mat3 val) = 0;
    virtual void SetUniform(std::string name, glm::mat4 val) = 0;

    static Shared<Shader> Create(Path shaderPath);
};
    
} // namespace Rush


#endif // __SHADER_H__
