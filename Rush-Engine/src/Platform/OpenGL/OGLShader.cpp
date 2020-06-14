#include "OGLShader.h"

#include "Rush/core/Logger.h"
#include "Rushpch.h"
#include <glad/glad.h>

namespace Rush {

#define VERTEX_SHADER 0
#define GEOMETRY_SHADER 1
#define FRAGMENT_SHADER 2
#define SHADER_TYPE_COUNT 3

uint32_t createShader(int8_t type, std::string source){
    GLenum shaderType;
    switch(type){
        case VERTEX_SHADER:     shaderType = GL_VERTEX_SHADER;      break;
        case GEOMETRY_SHADER:   shaderType = GL_GEOMETRY_SHADER;    break;
        case FRAGMENT_SHADER:   shaderType = GL_FRAGMENT_SHADER;    break;
        default: RUSH_ASSERT(false); 
    }

    uint32_t shader = glCreateShader(shaderType);
    const char *csource = source.c_str();
    glShaderSource(shader,1,&csource,nullptr);
    glCompileShader(shader);

    int success;
    char log[512];
    
    glGetShaderiv(shader,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(shader,512,nullptr,log);
        RUSH_LOG_ERROR("Failed to compile shader with error: " + std::string(log));
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

OGLShader::OGLShader(std::string shaderPath)
    :   Shader(shaderPath) {
    m_Shader = glCreateProgram();
    std::ifstream fs;
    fs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    uint32_t shaders[SHADER_TYPE_COUNT];
    try{
        fs.open(shaderPath);
        std::stringstream ss;
        std::string line;
        int8_t currentType = -1;
        while(!fs.eof()){
            ss = std::stringstream();
            getline(fs,line);
            while(!fs.eof() && line.substr(0,5) != "#type"){
                if(currentType == -1)
                    RUSH_LOG_WARNING("No '#type' read! Discarding shader line '" + line + "'");
                else
                    ss << line << "\n";
                getline(fs,line);
            }
            if(fs.eof())
                ss << line << "\n";

            if(currentType != -1)
                shaders[currentType] = createShader(currentType,ss.str());

            if(line.substr(0,5) == "#type"){
                if(line.substr(6,6) == "vertex")
                    currentType = VERTEX_SHADER;
                else if (line.substr(6,8) == "geometry")
                    currentType = GEOMETRY_SHADER;
                else if (line.substr(6,8) == "fragment")
                    currentType = FRAGMENT_SHADER;
                else
                    RUSH_ASSERT(false);
            }
        }

        for(int i = 0; i < SHADER_TYPE_COUNT; i++)
            if(shaders[i] != 0)
                glAttachShader(m_Shader,shaders[i]);

        glLinkProgram(m_Shader);

        int success;
        char log[512];
        
        glGetProgramiv(m_Shader,GL_LINK_STATUS,&success);
        if(!success){
            glGetProgramInfoLog(m_Shader,512,nullptr,log);
            RUSH_LOG_ERROR("Failed to link shader '" + shaderPath + "' with error: " + std::string(log));
            glDeleteShader(m_Shader);
        }

        for(int i = 0; i < SHADER_TYPE_COUNT; i++)
            if(shaders[i] != 0)
                glDeleteShader(shaders[i]);
                
    } catch(std::ifstream::failure e){
        RUSH_LOG_ERROR("Failed to load shader '" + shaderPath + "' with error: " + e.what());
    }
}

OGLShader::~OGLShader() {
    glDeleteProgram(m_Shader);
}

void OGLShader::Bind() {
    glUseProgram(m_Shader);
}

void OGLShader::Unbind() {
    glUseProgram(0);
}

void OGLShader::SetUniform(std::string name, ShaderData type, void *data) {
    auto loc = glGetUniformLocation(m_Shader,name.c_str());
    switch (type) {
    case ShaderData::FLOAT: glUniform1fv(loc,1,static_cast<GLfloat *>(data)); break;
    case ShaderData::FLOAT2: glUniform2fv(loc,2,static_cast<GLfloat *>(data)); break;
    case ShaderData::FLOAT3: glUniform3fv(loc,3,static_cast<GLfloat *>(data)); break;
    case ShaderData::FLOAT4: glUniform4fv(loc,4,static_cast<GLfloat *>(data)); break;
    case ShaderData::INT: glUniform1iv(loc,1,static_cast<GLint *>(data)); break;
    case ShaderData::INT2: glUniform2iv(loc,1,static_cast<GLint *>(data)); break;
    case ShaderData::INT3: glUniform3iv(loc,1,static_cast<GLint *>(data)); break;
    case ShaderData::INT4: glUniform4iv(loc,1,static_cast<GLint *>(data)); break;
    case ShaderData::BOOL: RUSH_ASSERT(false); break;
    case ShaderData::MAT3: glUniformMatrix3fv(loc,1,GL_FALSE,static_cast<GLfloat *>(data)); break;
    case ShaderData::MAT4: glUniformMatrix4fv(loc,1,GL_FALSE,static_cast<GLfloat *>(data)); break;
    default: RUSH_ASSERT(false); break;
    }
}

}