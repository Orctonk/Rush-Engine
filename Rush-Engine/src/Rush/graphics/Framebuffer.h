#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include "Rushpch.h"
#include "Rush/core/Core.h"
#include "Texture.h"

namespace Rush {

struct FramebufferOptions {
    uint32_t width,height;
    std::vector<uint8_t> texturePrecisions;
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
    virtual void Blit(Unique<Framebuffer> &target) = 0;
    
    std::vector<Shared<Texture>> &GetTextures(){ return m_Textures; }

    static Unique<Framebuffer> Create(FramebufferOptions options);
};

}

#endif // __FRAMEBUFFER_H__
