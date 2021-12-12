#ifndef __OGLVERTEXARRAY_H__
#define __OGLVERTEXARRAY_H__

#include "Rush/core/Core.h"
#include "Rush/graphics/IndexBuffer.h"
#include "Rush/graphics/VertexArray.h"
#include "Rush/graphics/VertexBuffer.h"

namespace Rush {

class OGLVertexArray : public VertexArray {
private:
    uint32_t m_VAO;
    uint8_t m_VertexIndex;

public:
    OGLVertexArray();
    ~OGLVertexArray();
    OGLVertexArray(OGLVertexArray &) = delete;
    OGLVertexArray(const OGLVertexArray &) = delete;

    void Bind() override;
    void Unbind() override;
    void AddVertexBuffer(Shared<VertexBuffer> vb) override;
    void SetIndexBuffer(Shared<IndexBuffer> ib) override;
};

} // namespace Rush

#endif // __OGLVERTEXARRAY_H__
