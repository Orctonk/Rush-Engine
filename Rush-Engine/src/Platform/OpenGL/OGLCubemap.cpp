#include "Rushpch.h"
#include "OGLCubemap.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace Rush {

OGLCubemap::OGLCubemap(uint32_t size, uint8_t precision)
: Cubemap("Created cubemap") {
    RUSH_PROFILE_FUNCTION();
    glGenTextures(1,&m_Cubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP,m_Cubemap);
    m_Props.size = size;
    m_Props.channels = 4;
    m_Props.bpp = precision;
    uint32_t format, type;
    switch(precision){
        case 8: format = GL_RGBA; type = GL_UNSIGNED_BYTE; break;
        case 16: format = GL_RGBA16F; type = GL_FLOAT; break;
        default: RUSH_ASSERT(false);
    }
    for(int i = 0; i < 6; i++){
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0,format,m_Props.size,m_Props.size,0,GL_RGBA,type,NULL);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

OGLCubemap::OGLCubemap(std::string filepath)
: Cubemap(filepath) {
    constexpr const char *names[6] = {
        "right.jpg",
        "left.jpg",
        "top.jpg",
        "bottom.jpg",
        "front.jpg",
        "back.jpg"
    };

    RUSH_PROFILE_FUNCTION();
    glGenTextures(1,&m_Cubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP,m_Cubemap);

    m_Props.channels = 0;
    int width, height, channels;
    stbi_set_flip_vertically_on_load(0);
    for(int i = 0; i < 6; i++){
        unsigned char *data = stbi_load((filepath + "/" + names[i]).c_str(),&width,&height,&channels,0);
        if(m_Props.channels == 0 && width == height){
            m_Props.channels = channels;
            m_Props.size = width;
        }
        if(width != height || width != m_Props.size || channels != m_Props.channels)
            RUSH_LOG_WARNING("Cubemap '" + filepath + "' is not consistent across textures!");

        if(data){
            if(channels == 3)
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
            else if(channels == 4)
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
        } else {
            Logger::Error("Failed to load texture '" + filepath + "'");
        }
        stbi_image_free(data);
    }
    stbi_set_flip_vertically_on_load(1);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

OGLCubemap::~OGLCubemap(){
    RUSH_PROFILE_FUNCTION();
    glDeleteTextures(1,&m_Cubemap);
}

void OGLCubemap::Bind(uint8_t textureSlot){
    RUSH_PROFILE_FUNCTION();
    glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_Cubemap);
}

}