#include "OGLTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>

#include "Rush/core/Logger.h"

namespace Rush {

OGLTexture::OGLTexture(std::string filepath) 
:   Texture(filepath) {
    glGenTextures(1,&m_Texture);
    glBindTexture(GL_TEXTURE_2D,m_Texture);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(filepath.c_str(),&width,&height,&nrChannels,0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        Logger::Error("Failed to load texture '" + filepath + "'");
    }
    stbi_image_free(data);
}

OGLTexture::~OGLTexture(){
    glDeleteTextures(1,&m_Texture);
}

void OGLTexture::Bind(){
    glBindTexture(GL_TEXTURE_2D, m_Texture);
}

}