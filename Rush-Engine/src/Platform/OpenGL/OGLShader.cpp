#include "OGLShader.h"
#include "Rush/resources/FileCacher.h"
#include "Rushpch.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <shaderc/shaderc.hpp>
#include <spirv_glsl.hpp>

namespace Rush {

OGLShader::OGLShader(std::string shaderPath)
    : Shader(shaderPath) {
    RUSH_PROFILE_FUNCTION();
    m_Shader = glCreateProgram();
    LoadOpenGLBinaries();
    LinkProgram();
}

OGLShader::~OGLShader() {
    RUSH_PROFILE_FUNCTION();
    glDeleteProgram(m_Shader);
}

void OGLShader::Bind() {
    RUSH_PROFILE_FUNCTION();
    glUseProgram(m_Shader);
}

void OGLShader::Unbind() {
    RUSH_PROFILE_FUNCTION();
    glUseProgram(0);
}

void OGLShader::SetUniform(std::string name, ShaderData type, const void *data) {
    RUSH_PROFILE_FUNCTION();
    glUseProgram(m_Shader);
    auto loc = GetUniformLocation(name);
    switch (type) {
        case ShaderData::FLOAT:
            glUniform1fv(loc, 1, static_cast<const GLfloat *>(data));
            break;
        case ShaderData::FLOAT2:
            glUniform2fv(loc, 1, static_cast<const GLfloat *>(data));
            break;
        case ShaderData::FLOAT3:
            glUniform3fv(loc, 1, static_cast<const GLfloat *>(data));
            break;
        case ShaderData::FLOAT4:
            glUniform4fv(loc, 1, static_cast<const GLfloat *>(data));
            break;
        case ShaderData::INT:
            glUniform1iv(loc, 1, static_cast<const GLint *>(data));
            break;
        case ShaderData::INT2:
            glUniform2iv(loc, 1, static_cast<const GLint *>(data));
            break;
        case ShaderData::INT3:
            glUniform3iv(loc, 1, static_cast<const GLint *>(data));
            break;
        case ShaderData::INT4:
            glUniform4iv(loc, 1, static_cast<const GLint *>(data));
            break;
        case ShaderData::BOOL:
            glUniform1iv(loc, 1, static_cast<const GLint *>(data));
            break;
        case ShaderData::MAT3:
            glUniformMatrix3fv(loc, 1, GL_FALSE, static_cast<const GLfloat *>(data));
            break;
        case ShaderData::MAT4:
            glUniformMatrix4fv(loc, 1, GL_FALSE, static_cast<const GLfloat *>(data));
            break;
        default:
            RUSH_ASSERT(false);
            break;
    }
}

void OGLShader::SetUniform(std::string name, int val) {
    RUSH_PROFILE_FUNCTION();
    glUseProgram(m_Shader);
    auto loc = GetUniformLocation(name);
    glUniform1i(loc, val);
}

void OGLShader::SetUniform(std::string name, int val1, int val2) {
    RUSH_PROFILE_FUNCTION();
    glUseProgram(m_Shader);
    auto loc = GetUniformLocation(name);
    glUniform2i(loc, val1, val2);
}

void OGLShader::SetUniform(std::string name, int val1, int val2, int val3) {
    RUSH_PROFILE_FUNCTION();
    glUseProgram(m_Shader);
    auto loc = GetUniformLocation(name);
    glUniform3i(loc, val1, val2, val3);
}

void OGLShader::SetUniform(std::string name, int val1, int val2, int val3, int val4) {
    RUSH_PROFILE_FUNCTION();
    glUseProgram(m_Shader);
    auto loc = GetUniformLocation(name);
    glUniform4i(loc, val1, val2, val3, val4);
}

void OGLShader::SetUniform(std::string name, float val) {
    RUSH_PROFILE_FUNCTION();
    glUseProgram(m_Shader);
    auto loc = GetUniformLocation(name);
    glUniform1f(loc, val);
}

void OGLShader::SetUniform(std::string name, glm::vec2 val) {
    RUSH_PROFILE_FUNCTION();
    glUseProgram(m_Shader);
    auto loc = GetUniformLocation(name);
    glUniform2f(loc, val.x, val.y);
}

void OGLShader::SetUniform(std::string name, glm::vec3 val) {
    RUSH_PROFILE_FUNCTION();
    glUseProgram(m_Shader);
    auto loc = GetUniformLocation(name);
    glUniform3f(loc, val.x, val.y, val.z);
}

void OGLShader::SetUniform(std::string name, glm::vec4 val) {
    RUSH_PROFILE_FUNCTION();
    glUseProgram(m_Shader);
    auto loc = GetUniformLocation(name);
    glUniform4f(loc, val.x, val.y, val.z, val.w);
}

void OGLShader::SetUniform(std::string name, bool val) {
    RUSH_PROFILE_FUNCTION();
    glUseProgram(m_Shader);
    auto loc = GetUniformLocation(name);
    glUniform1i(loc, val);
}

void OGLShader::SetUniform(std::string name, glm::mat3 val) {
    RUSH_PROFILE_FUNCTION();
    glUseProgram(m_Shader);
    auto loc = GetUniformLocation(name);
    glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(val));
}

void OGLShader::SetUniform(std::string name, glm::mat4 val) {
    RUSH_PROFILE_FUNCTION();
    glUseProgram(m_Shader);
    auto loc = GetUniformLocation(name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(val));
}

int OGLShader::GetUniformLocation(std::string name) {
    RUSH_PROFILE_FUNCTION();
    if (m_UniformCache.find(name) == m_UniformCache.end()) {
        auto loc = glGetUniformLocation(m_Shader, name.c_str());
        m_UniformCache[name] = loc;
        if (loc == -1) RUSH_LOG_WARNING("Uniform '{}' not found in shader '{}'!", name, m_DebugName);
    }
    return m_UniformCache[name];
}

int OGLShader::GetGLShaderEnum(ShaderType type) {
    switch (type) {
        case ShaderType::Vertex:
            return GL_VERTEX_SHADER;
        case ShaderType::Geometry:
            return GL_GEOMETRY_SHADER;
        case ShaderType::Fragment:
            return GL_FRAGMENT_SHADER;
        default:
            RUSH_ASSERT(false);
            return -1;
    }
}

void OGLShader::LoadOpenGLBinaries() {
    shaderc::Compiler compiler;
    shaderc::CompileOptions options;
    options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);

    m_OpenGLSources.clear();
    m_OpenGLBinaries.clear();

    int pushCIndex = 0;

    for (auto &&[type, binary] : m_SPIRVBinaries) {
        Path cachedBinaryPath = Path(m_DebugName).GetFileName() + ".glsl." + TypeToString(type);

        if (!FileCache::GetCachedBinaryFile(cachedBinaryPath.GetFullFileName(), m_OpenGLBinaries[type], m_SourceModTime)) {
            spirv_cross::CompilerGLSL glslCompiler(binary);
            spirv_cross::ShaderResources res = glslCompiler.get_shader_resources();
            for (const auto &pushC : res.push_constant_buffers) {
                glslCompiler.set_decoration(pushC.id, spv::DecorationLocation, pushCIndex++);
            }
            m_OpenGLSources[type] = glslCompiler.compile();

            Path cachedSourcePath = Path(m_DebugName).GetFileName() + ".glslsource." + TypeToString(type);
            FileCache::CacheTextFile(cachedSourcePath.GetFullFileName(), m_OpenGLSources[type]);

            shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(m_OpenGLSources[type], TypeToShaderc(type), m_DebugName.c_str(), options);
            if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
                RUSH_LOG_ERROR("({}) {}", TypeToString(type), module.GetErrorMessage());
                RUSH_ASSERT(false);
            }

            m_OpenGLBinaries[type] = std::vector<uint32_t>(module.cbegin(), module.cend());

            FileCache::CacheBinaryFile(cachedBinaryPath.GetFullFileName(), m_OpenGLBinaries[type]);
        }
    }
}

void OGLShader::LinkProgram() {
    RUSH_PROFILE_FUNCTION();

    std::vector<GLuint> shaderIDs;
    for (auto &&[type, binary] : m_OpenGLBinaries) {
        GLuint shaderID = shaderIDs.emplace_back(glCreateShader(GetGLShaderEnum(type)));
        glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, binary.data(), binary.size() * sizeof(uint32_t));
        if (glSpecializeShader)
            glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
        else if (glSpecializeShaderARB)
            glSpecializeShaderARB(shaderID, "main", 0, nullptr, nullptr);
        else {
            RUSH_LOG_ERROR("Cannot load binary shaders!");
            RUSH_ASSERT(false);
        }
        glAttachShader(m_Shader, shaderID);
    }

    glLinkProgram(m_Shader);

    GLint linkStatus;
    glGetProgramiv(m_Shader, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE) {
        GLint logLength;
        glGetProgramiv(m_Shader, GL_INFO_LOG_LENGTH, &logLength);

        char *log = new char[logLength];
        glGetProgramInfoLog(m_Shader, logLength, &logLength, log);
        RUSH_LOG_ERROR("Failed to link shader '{}' with error: {}", m_DebugName, log);
        glDeleteProgram(m_Shader);
    }

    for (auto id : shaderIDs) {
        glDetachShader(m_Shader, id);
        glDeleteShader(id);
    }
}

} // namespace Rush