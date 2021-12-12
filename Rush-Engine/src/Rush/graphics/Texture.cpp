#include "Texture.h"
#include "Rushpch.h"

#ifdef RUSH_OPENGL
    #include "Platform/OpenGL/OGLTexture.h"
#else
    #error "No rendering API provided!"
#endif

namespace Rush {

Texture::Texture(std::string filepath) : m_DebugName(filepath) { }

Texture::~Texture() { }

Shared<Texture> Texture::Create(Path filepath) {
    return CreateShared<OGLTexture>(filepath);
}

Shared<Texture> Texture::Create(uint32_t width, uint32_t height, TextureFormat format, uint8_t *data, std::string name) {
    return CreateShared<OGLTexture>(width, height, format, data, name);
}

} // namespace Rush