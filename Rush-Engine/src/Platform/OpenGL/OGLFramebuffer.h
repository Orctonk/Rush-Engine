#ifndef __OGLFRAMEBUFFER_H__
#define __OGLFRAMEBUFFER_H__

#include "Rush/graphics/Framebuffer.h"

#include <stdint.h>

namespace Rush {

class OGLFramebuffer : public Framebuffer {
private:
    uint32_t m_FBO;
    uint32_t m_DRO;

public:
    OGLFramebuffer(FramebufferOptions options);
    ~OGLFramebuffer();

    void Bind() override;
    void Unbind() override;
    void Resize(uint32_t width, uint32_t height) override;
    void Blit(Unique<Framebuffer> &other) override;
};

} // namespace Rush

#endif // __OGLFRAMEBUFFER_H__
