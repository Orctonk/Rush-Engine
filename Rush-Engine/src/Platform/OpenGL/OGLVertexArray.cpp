#include "OGLVertexArray.h"

#include "Rush/core/Core.h"
#include "ShaderDataUtil.h"

#include <glad/glad.h>

namespace Rush {

OGLVertexArray::OGLVertexArray() {
    m_VertexBuffers = std::vector<Shared<VertexBuffer>>();
    m_VertexIndex = 0;
    glGenVertexArrays(1,&m_VAO);
    RUSH_ASSERT(m_VAO);
}

OGLVertexArray::~OGLVertexArray(){
    glDeleteVertexArrays(1,&m_VAO);
}

void OGLVertexArray::Bind(){
    glBindVertexArray(m_VAO);
}

void OGLVertexArray::Unbind(){
    glBindVertexArray(0);
}

void OGLVertexArray::AddVertexBuffer(Shared<VertexBuffer> vb) {
    glBindVertexArray(m_VAO);

    vb->Bind();

    for(auto e: vb->GetLayout()){
        glEnableVertexAttribArray(m_VertexIndex);
        glVertexAttribPointer(
            m_VertexIndex,
            getElemCount(e.objType),
            getOpenGLType(e.objType),
            e.normalized ? GL_TRUE : GL_FALSE,
            e.stride,
            (void *)e.offset
        );

        m_VertexIndex++;
    }
    m_VertexBuffers.push_back(vb);
    Unbind();
}

void OGLVertexArray::SetIndexBuffer(Shared<IndexBuffer> ib) {
    glBindVertexArray(m_VAO);
    m_IndexBuffer = ib;
    ib->Bind();
    glBindVertexArray(0);
}

}