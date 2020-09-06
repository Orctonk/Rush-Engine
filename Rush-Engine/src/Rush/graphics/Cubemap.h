#ifndef __CUBEMAP_H__
#define __CUBEMAP_H__

#include "Rush/core/Core.h"

#include <string>
#include <stdint.h>

namespace Rush {

struct RUSH_API CubemapProperties {
    int size;
    int channels = 4;
    int bpp = 8;
};

class RUSH_API Cubemap {
private:
    std::string m_DebugPath;
protected:
    CubemapProperties m_Props;

    Cubemap(std::string filepath);

public:
    virtual ~Cubemap();

    Cubemap(Cubemap &) = delete;
    Cubemap(const Cubemap &) = delete;

    virtual void Bind(uint8_t textureSlot) = 0;
    virtual uint32_t GetID() = 0;

    static Shared<Cubemap> Create(std::string filepath);

    std::string GetDebugPath() { return m_DebugPath; }
    CubemapProperties GetProperties(){ return m_Props; }
};
    
} // namespace Rush

#endif // __CUBEMAP_H__
