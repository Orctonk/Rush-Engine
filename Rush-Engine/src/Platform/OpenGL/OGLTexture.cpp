#include "Rushpch.h"
#include "OGLTexture.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace Rush {

OGLTexture::OGLTexture(uint32_t width, uint32_t height, TextureFormat format, uint8_t* data, std::string name)
: Texture(name) {
    RUSH_PROFILE_FUNCTION();
    glGenTextures(1,&m_Texture);
    glBindTexture(GL_TEXTURE_2D,m_Texture);
    m_Props.width = width;
    m_Props.height = height;
    m_Props.format = format;
    uint32_t glformat, intformat, type;
    switch(format){
        case TextureFormat::RGBA8: 
            glformat = GL_RGBA; 
            intformat = GL_RGBA;
            type = GL_UNSIGNED_BYTE; 
            m_Props.channels = 4;
            m_Props.bpp = 8;
            break;
        case TextureFormat::RGBA16F: 
            glformat = GL_RGBA16F; 
            intformat = GL_RGBA;
            type = GL_FLOAT; 
            m_Props.channels = 4;
            m_Props.bpp = 16;
            break;
        case TextureFormat::DEPTH32: 
            glformat = GL_DEPTH_COMPONENT32; 
            intformat = GL_DEPTH_COMPONENT;
            type = GL_FLOAT; 
            m_Props.channels = 1;
            m_Props.bpp = 32;
            break;
        case TextureFormat::DEPTH24STENCIL8: 
            glformat = GL_DEPTH24_STENCIL8; 
            intformat = GL_DEPTH_STENCIL;
            type = GL_UNSIGNED_INT_24_8; 
            m_Props.channels = 2;
            m_Props.bpp = 16;
            break;
        default: RUSH_ASSERT(false);
    }
    glTexImage2D(GL_TEXTURE_2D,0,glformat,m_Props.width,m_Props.height,0,intformat,type,data);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

OGLTexture::OGLTexture(std::string filepath)
: Texture(filepath) {
    RUSH_PROFILE_FUNCTION();
    glGenTextures(1,&m_Texture);
    glBindTexture(GL_TEXTURE_2D,m_Texture);

    unsigned char *data = stbi_load(filepath.c_str(),&m_Props.width,&m_Props.height,&m_Props.channels,0);
    if(data){
        if(m_Props.channels == 1){
            GLint swizzleMask[] = {GL_RED,GL_RED,GL_RED,GL_ONE};
            glTexImage2D(GL_TEXTURE_2D,0,GL_RED,m_Props.width,m_Props.height,0,GL_RED,GL_UNSIGNED_BYTE,data);
            glTexParameteriv(GL_TEXTURE_2D,GL_TEXTURE_SWIZZLE_RGBA,swizzleMask);
        }
        else if(m_Props.channels == 2){

            GLint swizzleMask[] = {GL_RED,GL_RED,GL_RED,GL_GREEN};
            glTexImage2D(GL_TEXTURE_2D,0,GL_RG,m_Props.width,m_Props.height,0,GL_RG,GL_UNSIGNED_BYTE,data);
            glTexParameteriv(GL_TEXTURE_2D,GL_TEXTURE_SWIZZLE_RGBA,swizzleMask);
        }
        else if(m_Props.channels == 3)
            glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,m_Props.width,m_Props.height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        else if(m_Props.channels == 4)
            glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,m_Props.width,m_Props.height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        Logger::Error("Failed to load texture '" + filepath + "'");
    }
    stbi_image_free(data);
}

OGLTexture::~OGLTexture(){
    RUSH_PROFILE_FUNCTION();
    glDeleteTextures(1,&m_Texture);
}

void OGLTexture::Bind(uint8_t textureSlot){
    RUSH_PROFILE_FUNCTION();
    glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
}

void OGLTexture::GetTextureData(unsigned char *buffer, unsigned int bufsize) {
    RUSH_PROFILE_FUNCTION();
    GLenum format, type;
    if(m_Props.bpp == 8){
        type = GL_UNSIGNED_BYTE;
        if(m_Props.channels == 3) format = GL_RGB;
        else if(m_Props.channels == 4) format = GL_RGBA;
        else RUSH_ASSERT(false);
    } else if (m_Props.bpp == 16){
        type = GL_FLOAT;
        if(m_Props.channels == 4) format = GL_RGBA16F;
        else RUSH_ASSERT(false);
    }
    else RUSH_ASSERT(false);

    glBindTexture(GL_TEXTURE_2D,m_Texture);
    glGetTexImage(GL_TEXTURE_2D,0,format,type,buffer);
}

}