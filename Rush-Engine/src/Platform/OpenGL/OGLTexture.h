#ifndef __OGLTEXTURE_H__
#define __OGLTEXTURE_H__

#include "Rush/graphics/Texture.h"

#include <stdint.h>

namespace Rush {

class OGLTexture : public Texture {
private:
    uint32_t m_Texture;

public:
    OGLTexture(uint32_t width, uint32_t height, TextureFormat format, uint8_t *data = nullptr, std::string name = "Created texture");
    OGLTexture(std::string filepath);
    ~OGLTexture();
    OGLTexture(OGLTexture &) = delete;
    OGLTexture(const OGLTexture &) = delete;

    void Bind(uint8_t textureSlot) override;
    uint32_t GetID() override { return m_Texture; }
    void GetTextureData(unsigned char *buffer, unsigned int bufsize) override;
};

} // namespace Rush

#endif // __OGLTEXTURE_H__
