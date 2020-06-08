#include "IndexBuffer.h"

#ifdef RUSH_OPENGL
    #include "Platform/OpenGL/OGLIndexBuffer.h"
#else
    #error "No rendering API provided!"
#endif

namespace Rush {

IndexBuffer::IndexBuffer(const uint32_t *indices, uint32_t size) { }

IndexBuffer::~IndexBuffer() { }

Shared<IndexBuffer> IndexBuffer::Create(const uint32_t *indices, uint32_t size) {
    return CreateShared<OGLIndexBuffer>(indices,size);
}

}