#include "Texture.h"

#ifdef RUSH_OPENGL
    #include "Platform/OpenGL/OGLTexture.h"
#else
    #error "No rendering API provided!"
#endif

namespace Rush {

Texture::Texture() {}

Texture::~Texture() {}

Shared<Texture> Texture::Create(std::string filepath){
    return CreateShared<OGLTexture>(filepath);
}

}