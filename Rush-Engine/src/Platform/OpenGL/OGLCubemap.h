#ifndef __OGLTEXTURE_H__
#define __OGLTEXTURE_H__

#include <stdint.h>

#include "Rush/graphics/Cubemap.h"

namespace Rush {

class OGLCubemap : public Cubemap {
private:
    uint32_t m_Cubemap;

public:
    OGLCubemap(uint32_t size, uint8_t precision);
    OGLCubemap(std::string filepath);
    ~OGLCubemap();
    OGLCubemap(OGLCubemap &) = delete;
    OGLCubemap(const OGLCubemap &) = delete;

    void Bind(uint8_t textureSlot) override;
    uint32_t GetID() override { return m_Cubemap; }
};
    
} // namespace Rush

#endif // __OGLTEXTURE_H__
