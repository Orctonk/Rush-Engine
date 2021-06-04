#ifndef __UNIFORMBUFFER_H__
#define __UNIFORMBUFFER_H__

#include "Rush/core/Core.h"

#include <stdint.h>

namespace Rush {
    
class RUSH_API UniformBuffer {
protected:
    UniformBuffer();
public:
    virtual ~UniformBuffer();
    UniformBuffer(UniformBuffer&) = delete;
    UniformBuffer(const UniformBuffer&) = delete;

    virtual void Bind(uint16_t binding) = 0;
    virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;

    static Shared<UniformBuffer> Create(uint32_t size, uint16_t binding);
};

} // namespace Rush


#endif // __UNIFORMBUFFER_H__
