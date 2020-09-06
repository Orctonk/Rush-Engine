#include "Rushpch.h"
#include "OGLVertexArray.h"

#include "ShaderDataUtil.h"

#include <glad/glad.h>

namespace Rush {

OGLVertexArray::OGLVertexArray() {
    RUSH_PROFILE_FUNCTION();
    m_VertexBuffers = std::vector<Shared<VertexBuffer>>();
    m_VertexIndex = 0;
    glGenVertexArrays(1,&m_VAO);
    RUSH_ASSERT(m_VAO);
}

OGLVertexArray::~OGLVertexArray(){
    RUSH_PROFILE_FUNCTION();
    glDeleteVertexArrays(1,&m_VAO);
}

void OGLVertexArray::Bind(){
    RUSH_PROFILE_FUNCTION();
    glBindVertexArray(m_VAO);
}

void OGLVertexArray::Unbind(){
    RUSH_PROFILE_FUNCTION();
    glBindVertexArray(0);
}

void OGLVertexArray::AddVertexBuffer(Shared<VertexBuffer> vb) {
    RUSH_PROFILE_FUNCTION();
    glBindVertexArray(m_VAO);

    vb->Bind();

    for(auto e: vb->GetLayout()){
        switch(e.objType){
            case ShaderData::BOOL:
            case ShaderData::FLOAT:
            case ShaderData::FLOAT2:
            case ShaderData::FLOAT3:
            case ShaderData::FLOAT4:
            case ShaderData::INT:
            case ShaderData::INT2:
            case ShaderData::INT3:
            case ShaderData::INT4:
                glEnableVertexAttribArray(m_VertexIndex);
                glVertexAttribPointer(
                    m_VertexIndex,
                    getElemCount(e.objType),
                    getOpenGLType(e.objType),
                    e.normalized ? GL_TRUE : GL_FALSE,
                    e.stride,
                    (void *)(uint64_t)e.offset
                );
                if(vb->IsInstanced())
                    glVertexAttribDivisor(m_VertexIndex,1);
                m_VertexIndex++;
                break;
            case ShaderData::MAT3:
            case ShaderData::MAT4:
                for(int i = 0; i < getElemCount(e.objType); i++){
                    glEnableVertexAttribArray(m_VertexIndex);
                    glVertexAttribPointer(
                        m_VertexIndex,
                        getElemCount(e.objType),
                        getOpenGLType(e.objType),
                        e.normalized ? GL_TRUE : GL_FALSE,
                        e.stride,
                        (void *)(e.offset + sizeof(float) * getElemCount(e.objType) * i)
                    );
                    if(vb->IsInstanced())
                        glVertexAttribDivisor(m_VertexIndex,1);
                    m_VertexIndex++;
                }
                break;
        }

    }
    m_VertexBuffers.push_back(vb);
    Unbind();
}

void OGLVertexArray::SetIndexBuffer(Shared<IndexBuffer> ib) {
    RUSH_PROFILE_FUNCTION();
    glBindVertexArray(m_VAO);
    m_IndexBuffer = ib;
    ib->Bind();
    glBindVertexArray(0);
}

}