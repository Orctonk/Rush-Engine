#include "OGLTexture.h"

#include <stb_image.h>
#include <glad/glad.h>

#include "Rush/core/Logger.h"

namespace Rush {

OGLTexture::OGLTexture(uint32_t width, uint32_t height, uint8_t precision)
: Texture("Created texture") {
    RUSH_PROFILE_FUNCTION();
    glGenTextures(1,&m_Texture);
    glBindTexture(GL_TEXTURE_2D,m_Texture);
    m_Props.width = width;
    m_Props.height = height;
    m_Props.channels = 4;
    m_Props.bpp = precision;
    uint32_t format, type;
    switch(precision){
        case 8: format = GL_RGBA; type = GL_UNSIGNED_BYTE; break;
        case 16: format = GL_RGBA16F; type = GL_FLOAT; break;
        default: RUSH_ASSERT(false);
    }
    glTexImage2D(GL_TEXTURE_2D,0,format,m_Props.width,m_Props.height,0,GL_RGBA,type,NULL);
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