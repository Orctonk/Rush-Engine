#ifndef __RENDERINGAPI_H__
#define __RENDERINGAPI_H__

#include "Rush/core/Core.h"
#include "VertexArray.h"
#include "Shader.h"

#include <stdint.h>
#include <glm/glm.hpp>

namespace Rush {

enum class CullFace{
    None,
    Clockwise,
    CounterClockwise,
    Both
};

enum class PolygonMode{
    Fill,
    Wireframe
};

enum class BlendMode{
    None,
    Add,
    Subtract
};

enum class Multisampling{
    On,
    Off
};

enum class DepthTest{
    None,
    Always,
    Never,
    Less,
    LessOrEqual,
    Greater,
    GreaterOrEqual
};

class RUSH_API RenderingAPI {
public:
    virtual void Init() = 0;
    virtual void Shutdown() = 0;

    virtual void SetOption(CullFace culling) = 0;
    virtual void SetOption(PolygonMode polygonMode) = 0;
    virtual void SetOption(BlendMode blendMode) = 0;
    virtual void SetOption(Multisampling multisampling) = 0;
    virtual void SetOption(DepthTest depthTest) = 0;

    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
    virtual void SetClearColor(glm::vec4 color) = 0;
    virtual void Clear() = 0;

    virtual void DrawIndexed(const Unique<VertexArray> &va) = 0;
    virtual void DrawIndexed(const Unique<VertexArray> &va, uint32_t count) = 0;

    static Unique<RenderingAPI> Create();
};

}

#endif // __RENDERINGAPI_H__
