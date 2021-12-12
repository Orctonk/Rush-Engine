#include "IndexBuffer.h"
#include "Rushpch.h"

#ifdef RUSH_OPENGL
    #include "Platform/OpenGL/OGLIndexBuffer.h"
#else
    #error "No rendering API provided!"
#endif

namespace Rush {

IndexBuffer::IndexBuffer(const uint32_t *indices, uint32_t count) {
    m_IndexCount = count;
}

IndexBuffer::~IndexBuffer() { }

Shared<IndexBuffer> IndexBuffer::Create(const uint32_t *indices, uint32_t count) {
    return CreateShared<OGLIndexBuffer>(indices, count);
}

} // namespace Rush