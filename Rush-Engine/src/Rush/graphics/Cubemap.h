#ifndef __CUBEMAP_H__
#define __CUBEMAP_H__

#include "Rushpch.h"
#include "Rush/core/Core.h"

namespace Rush {

struct CubemapProperties {
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
