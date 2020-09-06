#ifndef __VERTEXBUFFER_H__
#define __VERTEXBUFFER_H__

#include "Rush/core/Core.h"
#include "Shader.h"

#include <stdint.h>
#include <vector>

namespace Rush {

struct RUSH_API LayoutElement {
    ShaderData objType;
    uint32_t stride;
    uint32_t offset;
    bool normalized;
};

class RUSH_API VertexBuffer {
protected:
    std::vector<LayoutElement> m_Layout;
    bool m_Instanced = false;

    VertexBuffer(uint32_t size);
    VertexBuffer(const float *data, uint32_t size);
public:
    virtual ~VertexBuffer();
    VertexBuffer(VertexBuffer&) = delete;
    VertexBuffer(const VertexBuffer&) = delete;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual void BufferData(const void *data, uint32_t size) = 0;
    virtual void SetInterleavedLayout(std::vector<ShaderData> layout) = 0;
    virtual void SetBatchedLayout(std::vector<ShaderData> layout) = 0;
    void SetCustomLayout(std::vector<LayoutElement> layout) { m_Layout = layout; }
    std::vector<LayoutElement> GetLayout() { return m_Layout; }
    bool IsInstanced(){ return m_Instanced; }
    void SetInstanced(bool instanced){ m_Instanced = instanced; }

    static Shared<VertexBuffer> Create(const float *data, uint32_t size);
    static Shared<VertexBuffer> Create(uint32_t size);
};

}

#endif // __VERTEXBUFFER_H__
