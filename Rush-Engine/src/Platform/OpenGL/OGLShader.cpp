#include "Rushpch.h"
#include "OGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Rush {

#define VERTEX_SHADER 0
#define GEOMETRY_SHADER 1
#define FRAGMENT_SHADER 2
#define SHADER_TYPE_COUNT 3

uint32_t createShader(int8_t type, std::string source){
    RUSH_PROFILE_FUNCTION();
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
    RUSH_PROFILE_FUNCTION();
    m_Shader = glCreateProgram();
    std::ifstream fs;
    fs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    uint32_t shaders[SHADER_TYPE_COUNT] = {0,0,0};
    try{
        fs.open(shaderPath);
        if (fs.is_open()) {
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
                
            fs.close();
        } else {
            RUSH_LOG_ERROR("Failed to open file" + shaderPath);
        }

    } catch(std::ifstream::failure e){
        RUSH_LOG_ERROR("Failed to load shader '" + shaderPath + "' with error: " + strerror(errno));
    }
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
    case ShaderData::FLOAT: glUniform1fv(loc,1,static_cast<const GLfloat *>(data)); break;
    case ShaderData::FLOAT2: glUniform2fv(loc,1,static_cast<const GLfloat *>(data)); break;
    case ShaderData::FLOAT3: glUniform3fv(loc,1,static_cast<const GLfloat *>(data)); break;
    case ShaderData::FLOAT4: glUniform4fv(loc,1,static_cast<const GLfloat *>(data)); break;
    case ShaderData::INT: glUniform1iv(loc,1,static_cast<const GLint *>(data)); break;
    case ShaderData::INT2: glUniform2iv(loc,1,static_cast<const GLint *>(data)); break;
    case ShaderData::INT3: glUniform3iv(loc,1,static_cast<const GLint *>(data)); break;
    case ShaderData::INT4: glUniform4iv(loc,1,static_cast<const GLint *>(data)); break;
    case ShaderData::BOOL: glUniform1iv(loc,1,static_cast<const GLint *>(data)); break;
    case ShaderData::MAT3: glUniformMatrix3fv(loc,1,GL_FALSE,static_cast<const GLfloat *>(data)); break;
    case ShaderData::MAT4: glUniformMatrix4fv(loc,1,GL_FALSE,static_cast<const GLfloat *>(data)); break;
    default: RUSH_ASSERT(false); break;
    }
}

void OGLShader::SetUniform(std::string name, int val){
    RUSH_PROFILE_FUNCTION();
    glUseProgram(m_Shader);
    auto loc = GetUniformLocation(name);
    glUniform1i(loc,val);
}

void OGLShader::SetUniform(std::string name, int val1, int val2){
    RUSH_PROFILE_FUNCTION();
    glUseProgram(m_Shader);
    auto loc = GetUniformLocation(name);
    glUniform2i(loc,val1,val2);
}

void OGLShader::SetUniform(std::string name, int val1, int val2, int val3){
    RUSH_PROFILE_FUNCTION();
    glUseProgram(m_Shader);
    auto loc = GetUniformLocation(name);
    glUniform3i(loc,val1,val2,val3);
}

void OGLShader::SetUniform(std::string name, int val1, int val2, int val3, int val4){
    RUSH_PROFILE_FUNCTION();
    glUseProgram(m_Shader);
    auto loc = GetUniformLocation(name);
    glUniform4i(loc,val1,val2,val3,val4);
}

void OGLShader::SetUniform(std::string name, float val){
    RUSH_PROFILE_FUNCTION();
    glUseProgram(m_Shader);
    auto loc = GetUniformLocation(name);
    glUniform1f(loc,val);
}

void OGLShader::SetUniform(std::string name, glm::vec2 val){
    RUSH_PROFILE_FUNCTION();
    glUseProgram(m_Shader);
    auto loc = GetUniformLocation(name);
    glUniform2f(loc,val.x,val.y);
}

void OGLShader::SetUniform(std::string name, glm::vec3 val){
    RUSH_PROFILE_FUNCTION();
    glUseProgram(m_Shader);
    auto loc = GetUniformLocation(name);
    glUniform3f(loc,val.x,val.y,val.z);
}

void OGLShader::SetUniform(std::string name, glm::vec4 val){
    RUSH_PROFILE_FUNCTION();
    glUseProgram(m_Shader);
    auto loc = GetUniformLocation(name);
    glUniform4f(loc,val.x,val.y,val.z,val.w);
}

void OGLShader::SetUniform(std::string name, bool val){
    RUSH_PROFILE_FUNCTION();
    glUseProgram(m_Shader);
    auto loc = GetUniformLocation(name);
    glUniform1i(loc,val);
}

void OGLShader::SetUniform(std::string name, glm::mat3 val){
    RUSH_PROFILE_FUNCTION();
    glUseProgram(m_Shader);
    auto loc = GetUniformLocation(name);
    glUniformMatrix3fv(loc,1,GL_FALSE,glm::value_ptr(val));
}

void OGLShader::SetUniform(std::string name, glm::mat4 val){
    RUSH_PROFILE_FUNCTION();
    glUseProgram(m_Shader);
    auto loc = GetUniformLocation(name);
    glUniformMatrix4fv(loc,1,GL_FALSE,glm::value_ptr(val));
}

int OGLShader::GetUniformLocation(std::string name){
    RUSH_PROFILE_FUNCTION();
    if(m_UniformCache.find(name) == m_UniformCache.end()){
        auto loc = glGetUniformLocation(m_Shader,name.c_str());
        m_UniformCache[name] = loc;
        if(loc == -1) RUSH_LOG_WARNING("Uniform '" + name + "' not found in shader '" + m_DebugName + "'!");
    }
    return m_UniformCache[name];
}

}