#include "OGLTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>

#include "Rush/core/Logger.h"

namespace Rush {

OGLTexture::OGLTexture(uint32_t width, uint32_t height, uint8_t precision)
: Texture("Created texture") {
    glGenTextures(1,&m_Texture);
    glBindTexture(GL_TEXTURE_2D,m_Texture);
    uint32_t format, type;
    switch(precision){
        case 8: format = GL_RGBA; type = GL_UNSIGNED_BYTE; break;
        case 16: format = GL_RGBA16F; type = GL_FLOAT; break;
        default: RUSH_ASSERT(false);
    }
    glTexImage2D(GL_TEXTURE_2D,0,format,width,height,0,GL_RGBA,type,NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

OGLTexture::OGLTexture(std::string filepath)
: Texture(filepath) {
    glGenTextures(1,&m_Texture);
    glBindTexture(GL_TEXTURE_2D,m_Texture);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(1);
    unsigned char *data = stbi_load(filepath.c_str(),&width,&height,&nrChannels,0);
    if(data){
        if(nrChannels == 3)
            glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        else if(nrChannels == 4)
            glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        Logger::Error("Failed to load texture '" + filepath + "'");
    }
    stbi_image_free(data);
}

OGLTexture::~OGLTexture(){
    glDeleteTextures(1,&m_Texture);
}

void OGLTexture::Bind(uint8_t textureSlot){
    glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
}

}