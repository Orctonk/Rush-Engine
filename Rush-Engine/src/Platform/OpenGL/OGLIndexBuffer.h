#ifndef __OGLINDEXBUFFER_H__
#define __OGLINDEXBUFFER_H__

#include "Rush/graphics/IndexBuffer.h"

#include <stdint.h>

namespace Rush {

class OGLIndexBuffer : public IndexBuffer {
private:
    uint32_t m_EBO;

public:
    OGLIndexBuffer(const uint32_t *indices, uint32_t count);
    ~OGLIndexBuffer();
    OGLIndexBuffer(OGLIndexBuffer &) = delete;
    OGLIndexBuffer(const OGLIndexBuffer &) = delete;

    void Bind() override;
    void Unbind() override;
};

} // namespace Rush

#endif // __OGLINDEXBUFFER_H__
