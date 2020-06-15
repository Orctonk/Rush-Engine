#ifndef __VERTEXARRAY_H__
#define __VERTEXARRAY_H__

#include "Rushpch.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "Rush/core/Core.h"

namespace Rush {

class RUSH_API VertexArray {
protected:
    Shared<IndexBuffer> m_IndexBuffer;
    std::vector<Shared<VertexBuffer>> m_VertexBuffers;
    VertexArray();
public:
    virtual ~VertexArray();
    VertexArray(VertexArray&) = delete;
    VertexArray(const VertexArray&) = delete;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    virtual void AddVertexBuffer(Shared<VertexBuffer> vb) = 0;
    virtual void SetIndexBuffer(Shared<IndexBuffer> ib) = 0;

    Shared<IndexBuffer> GetIndexBuffer() { return m_IndexBuffer; }
    std::vector<Shared<VertexBuffer>> GetVertexBuffers() { return m_VertexBuffers; }

    static Unique<VertexArray> Create();
};
    
} // namespace Rush

#endif // __VERTEXARRAY_H__
