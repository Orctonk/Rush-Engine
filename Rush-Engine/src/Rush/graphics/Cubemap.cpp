#include "Rushpch.h"
#include "Cubemap.h"

#ifdef RUSH_OPENGL
    #include "Platform/OpenGL/OGLCubemap.h"
#else
    #error "No rendering API provided!"
#endif

namespace Rush {

Cubemap::Cubemap(std::string filepath) : m_DebugPath(filepath) {}

Cubemap::~Cubemap() {}

Shared<Cubemap> Cubemap::Create(std::string filepath){
    return CreateShared<OGLCubemap>(filepath);
}

}