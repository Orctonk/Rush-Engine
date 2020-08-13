#ifndef __OGLRENDERINGAPI_H__
#define __OGLRENDERINGAPI_H__

#include "Rush/graphics/RenderingAPI.h"

namespace Rush {

class RUSH_API OGLRenderingAPI : public RenderingAPI {
public:
    void Init() override;
    void Shutdown() override;

    void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
    void SetClearColor(glm::vec4 color) override;
    void Clear() override;

    void DrawIndexed(const Unique<VertexArray> &va) override;
    void DrawIndexed(const Unique<VertexArray> &va, uint32_t count) override;
};

}

#endif // __OGLRENDERINGAPI_H__
