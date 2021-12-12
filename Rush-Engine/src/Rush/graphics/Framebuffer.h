#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include "Rush/core/Core.h"
#include "Texture.h"

#include <stdint.h>
#include <vector>

namespace Rush {

struct RUSH_API FramebufferOptions {
    uint32_t width, height;
    std::vector<TextureFormat> textureFormats;
};

class RUSH_API Framebuffer {
protected:
    std::vector<Shared<Texture>> m_Textures;
    FramebufferOptions m_Options;

public:
    Framebuffer(FramebufferOptions options);
    virtual ~Framebuffer();

    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    virtual void Resize(uint32_t width, uint32_t height) = 0;
    virtual void Blit(Unique<Framebuffer> &target) = 0;

    std::vector<Shared<Texture>> &GetTextures() { return m_Textures; }

    static Unique<Framebuffer> Create(FramebufferOptions options);
};

} // namespace Rush

#endif // __FRAMEBUFFER_H__
