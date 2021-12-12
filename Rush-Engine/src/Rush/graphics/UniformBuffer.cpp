#include "UniformBuffer.h"
#include "Rushpch.h"

#ifdef RUSH_OPENGL
    #include "Platform/OpenGL/OGLUniformBuffer.h"
#else
    #error "No rendering API provided!"
#endif

namespace Rush {

UniformBuffer::UniformBuffer() { }

UniformBuffer::~UniformBuffer() { }

Shared<UniformBuffer> UniformBuffer::Create(uint32_t size, uint16_t binding) {
    return CreateShared<OGLUniformBuffer>(size, binding);
}

} // namespace Rush