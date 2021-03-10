#include "Rushpch.h"
#include "Texture.h"

#ifdef RUSH_OPENGL
    #include "Platform/OpenGL/OGLTexture.h"
#else
    #error "No rendering API provided!"
#endif

namespace Rush {

Texture::Texture(std::string filepath) : m_DebugPath(filepath) {}

Texture::~Texture() {}

Shared<Texture> Texture::Create(Path filepath){
    return CreateShared<OGLTexture>(filepath);
}

}