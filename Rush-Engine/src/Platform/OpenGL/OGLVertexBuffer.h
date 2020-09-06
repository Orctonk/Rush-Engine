#ifndef __OGLVERTEXBUFFER_H__
#define __OGLVERTEXBUFFER_H__

#include "Rush/graphics/VertexBuffer.h"
#include "Rush/graphics/Shader.h"

#include <stdint.h>
#include <vector>

namespace Rush {

class OGLVertexBuffer : public VertexBuffer{
private:
    uint32_t m_VBO;
public:
    OGLVertexBuffer(uint32_t size);
    OGLVertexBuffer(const float *data, uint32_t size);
    ~OGLVertexBuffer();
    OGLVertexBuffer(OGLVertexBuffer&) = delete;
    OGLVertexBuffer(const OGLVertexBuffer&) = delete;

    void Bind() const override;
    void Unbind() const override;
    void BufferData(const void *data, uint32_t size) override;
    void SetInterleavedLayout(std::vector<ShaderData> layout) override;
    void SetBatchedLayout(std::vector<ShaderData> layout) override;
};

}

#endif // __OGLVERTEXBUFFER_H__
