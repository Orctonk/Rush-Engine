#ifndef __INDEXBUFFER_H__
#define __INDEXBUFFER_H__

#include "Rush/core/Core.h"

#include <stdint.h>

namespace Rush {
    
class RUSH_API IndexBuffer {
private:
    uint32_t m_IndexCount;
protected:
    IndexBuffer(const uint32_t *indices, uint32_t count);
public:
    virtual ~IndexBuffer();
    IndexBuffer(IndexBuffer &) = delete;
    IndexBuffer(const IndexBuffer &) = delete;

    virtual void Bind() = 0; 
    virtual void Unbind() = 0;

    uint32_t GetIndexCount() { return m_IndexCount; }

    static Shared<IndexBuffer> Create(const uint32_t *indices, uint32_t count);
};

} // namespace Rush


#endif // __INDEXBUFFER_H__
