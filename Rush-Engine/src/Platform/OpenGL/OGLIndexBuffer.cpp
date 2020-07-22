#include "OGLIndexBuffer.h"

#include "Rush/core/Core.h"

#include <glad/glad.h>

namespace Rush {

OGLIndexBuffer::OGLIndexBuffer(const uint32_t *indices, uint32_t count)
    : IndexBuffer(indices,count) {
    glGenBuffers(1,&m_EBO);
    RUSH_ASSERT(m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,count*sizeof(uint32_t),indices,GL_STATIC_DRAW);
}

OGLIndexBuffer::~OGLIndexBuffer() {
    glDeleteBuffers(1,&m_EBO);
}

void OGLIndexBuffer::Bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_EBO);
}

void OGLIndexBuffer::Unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

}