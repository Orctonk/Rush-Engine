#ifndef __OGLRENDERINGAPI_H__
#define __OGLRENDERINGAPI_H__

#include "Rush/graphics/RenderingAPI.h"

namespace Rush {

class OGLRenderingAPI : public RenderingAPI {
public:
    void Init() override;
    void Shutdown() override;

    void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
    void SetClearColor(glm::vec4 color) override;
    void Clear() override;

    void SetOption(CullFace culling) override;
    void SetOption(PolygonMode polygonMode) override;
    void SetOption(BlendMode blendMode) override;
    void SetOption(Multisampling multisampling) override;
    void SetOption(DepthTest depthTest) override;

    void SetPointSize(float size) override;
    void SetLineWidth(float width) override;

    void DrawIndexed(const Shared<VertexArray> &va, Primitive type = Primitive::Triangle) override;
    void DrawIndexed(const Shared<VertexArray> &va, uint32_t count, Primitive type = Primitive::Triangle) override;
};

} // namespace Rush

#endif // __OGLRENDERINGAPI_H__
