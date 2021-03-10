#include "Rushpch.h"
#include "Shader.h"

#ifdef RUSH_OPENGL
    #include "Platform/OpenGL/OGLShader.h"
#else
    #error "No rendering API provided!"
#endif

namespace Rush {

Shader::Shader(std::string shaderPath) : m_DebugPath(shaderPath) { }

Shader::~Shader() { }

Shared<Shader> Shader::Create(Path shaderPath) {
    return CreateShared<OGLShader>(shaderPath);
}

}