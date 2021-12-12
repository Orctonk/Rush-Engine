#ifndef __VERTEXARRAY_H__
#define __VERTEXARRAY_H__

#include "IndexBuffer.h"
#include "Rush/core/Core.h"
#include "VertexBuffer.h"

#include <stdint.h>
#include <vector>

namespace Rush {

class RUSH_API VertexArray {
protected:
    Shared<IndexBuffer> m_IndexBuffer;
    std::vector<Shared<VertexBuffer>> m_VertexBuffers;
    uint32_t m_InstanceCount = 0;
    VertexArray();

public:
    virtual ~VertexArray();
    VertexArray(VertexArray &) = delete;
    VertexArray(const VertexArray &) = delete;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    virtual void AddVertexBuffer(Shared<VertexBuffer> vb) = 0;
    virtual void SetIndexBuffer(Shared<IndexBuffer> ib) = 0;

    Shared<IndexBuffer> GetIndexBuffer() { return m_IndexBuffer; }
    std::vector<Shared<VertexBuffer>> GetVertexBuffers() { return m_VertexBuffers; }
    uint32_t GetInstanceCount() { return m_InstanceCount; }
    void SetInstanceCount(uint32_t count) { m_InstanceCount = count; }

    static Shared<VertexArray> Create();
};

} // namespace Rush

#endif // __VERTEXARRAY_H__
