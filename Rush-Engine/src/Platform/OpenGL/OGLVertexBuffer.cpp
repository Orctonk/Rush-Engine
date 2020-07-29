#include "OGLVertexBuffer.h"

#include "Rush/core/Core.h"
#include "ShaderDataUtil.h"

#include <glad/glad.h>

namespace Rush {

OGLVertexBuffer::OGLVertexBuffer(uint32_t size) 
    :   VertexBuffer(size) {
    RUSH_PROFILE_FUNCTION();
    glGenBuffers(1,&m_VBO);
    RUSH_ASSERT(m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER,m_VBO);
    glBufferData(GL_ARRAY_BUFFER,size,nullptr,GL_DYNAMIC_DRAW);
}

OGLVertexBuffer::OGLVertexBuffer(const float *data, uint32_t size)
    :   VertexBuffer(data, size) {
    RUSH_PROFILE_FUNCTION();
    glGenBuffers(1,&m_VBO);
    RUSH_ASSERT(m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER,m_VBO);
    glBufferData(GL_ARRAY_BUFFER,size,data,GL_STATIC_DRAW);
}

OGLVertexBuffer::~OGLVertexBuffer() {
    RUSH_PROFILE_FUNCTION();
    glDeleteBuffers(1,&m_VBO);
}

void OGLVertexBuffer::Bind() const {
    RUSH_PROFILE_FUNCTION();
    glBindBuffer(GL_ARRAY_BUFFER,m_VBO);
}

void OGLVertexBuffer::Unbind() const {
    RUSH_PROFILE_FUNCTION();
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

void OGLVertexBuffer::BufferData(const void *data, uint32_t size) {
    RUSH_PROFILE_FUNCTION();
    glBufferSubData(m_VBO,0,size,data);
}

void OGLVertexBuffer::SetInterleavedLayout(std::vector<ShaderData> layout) {
    RUSH_PROFILE_FUNCTION();
    m_Layout = std::vector<LayoutElement>();
    uint32_t stride = 0;
    for(auto d : layout)
        stride += getSize(d);

    uint32_t offset = 0;

    for(auto d : layout){
        LayoutElement le;
        le.offset = offset;
        le.stride = stride;
        le.objType = d;
        le.normalized = true;
        m_Layout.push_back(le);
        offset += getSize(d);
    }
}

void OGLVertexBuffer::SetBatchedLayout(std::vector<ShaderData> layout) {
    RUSH_PROFILE_FUNCTION();
    m_Layout = std::vector<LayoutElement>();

    uint32_t offset = 0;

    for(auto d : layout){
        LayoutElement le;
        le.offset = offset;
        le.stride = getSize(d) * layout.size();
        le.objType = d;
        le.normalized = true;
        m_Layout.push_back(le);
        offset += le.stride;
    }
}

}