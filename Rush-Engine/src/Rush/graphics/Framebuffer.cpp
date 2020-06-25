#include "Framebuffer.h"

#ifdef RUSH_OPENGL
    #include "Platform/OpenGL/OGLFramebuffer.h"
#else
    #error "No rendering API provided!"
#endif

namespace Rush {

Framebuffer::Framebuffer(FramebufferOptions options)
:   m_Options(options) {}
Framebuffer::~Framebuffer(){}

Unique<Framebuffer> Framebuffer::Create(FramebufferOptions options){
    return CreateUnique<OGLFramebuffer>(options);
}

} // namespace Rush
