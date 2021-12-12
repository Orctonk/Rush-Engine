#include "Cubemap.h"
#include "Rushpch.h"

#ifdef RUSH_OPENGL
    #include "Platform/OpenGL/OGLCubemap.h"
#else
    #error "No rendering API provided!"
#endif

namespace Rush {

Cubemap::Cubemap(std::string filepath) : m_DebugName(filepath) { }

Cubemap::~Cubemap() { }

Shared<Cubemap> Cubemap::Create(Path filepath) {
    return CreateShared<OGLCubemap>(filepath);
}

} // namespace Rush