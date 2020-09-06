#include "Rushpch.h"
#include "VertexBuffer.h"

#ifdef RUSH_OPENGL
    #include "Platform/OpenGL/OGLVertexBuffer.h"
#else
    #error "No rendering API provided!"
#endif

namespace Rush {

VertexBuffer::VertexBuffer(uint32_t size) {}
VertexBuffer::VertexBuffer(const float *data, uint32_t size) {}

VertexBuffer::~VertexBuffer() { }

Shared<VertexBuffer> VertexBuffer::Create(const float *data, uint32_t size) {
    return CreateShared<OGLVertexBuffer>(data,size);
}

Shared<VertexBuffer> VertexBuffer::Create(uint32_t size) {
    return CreateShared<OGLVertexBuffer>(size);
}

}