#ifndef __INDEXBUFFER_H__
#define __INDEXBUFFER_H__

#include <stdint.h>
#include "Rush/core/Core.h"

namespace Rush {
    
class RUSH_API IndexBuffer {
protected:
    IndexBuffer(const uint32_t *indices, uint32_t size);
public:
    virtual ~IndexBuffer();
    IndexBuffer(IndexBuffer &) = delete;
    IndexBuffer(const IndexBuffer &) = delete;

    virtual void Bind() = 0; 
    virtual void Unbind() = 0;

    static Shared<IndexBuffer> Create(const uint32_t *indices, uint32_t size);
};

} // namespace Rush


#endif // __INDEXBUFFER_H__
