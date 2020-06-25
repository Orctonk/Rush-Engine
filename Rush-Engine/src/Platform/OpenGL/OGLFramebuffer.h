#include "Rush/graphics/Framebuffer.h"

#include <stdint.h>

namespace Rush {

class OGLFramebuffer : public Framebuffer{
private:
    uint32_t m_FBO;
    uint32_t m_DRO;
public: 
    OGLFramebuffer(FramebufferOptions options);
    ~OGLFramebuffer();

    void Bind() override;
    void Unbind() override;
    void Blit(Unique<Framebuffer> &other) override;
};

}