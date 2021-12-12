#ifndef __RENDERINGAPI_H__
#define __RENDERINGAPI_H__

#include "Rush/core/Core.h"
#include "Shader.h"
#include "VertexArray.h"

#include <glm/glm.hpp>
#include <stdint.h>

namespace Rush {

enum class CullFace {
    None,
    Clockwise,
    CounterClockwise,
    Both
};

enum class PolygonMode {
    Fill,
    Wireframe
};

enum class BlendMode {
    None,
    Add,
    Subtract,
    Alpha
};

enum class Multisampling {
    On,
    Off
};

enum class DepthTest {
    None,
    Always,
    Never,
    Less,
    LessOrEqual,
    Greater,
    GreaterOrEqual
};

enum class Primitive {
    Point,
    Line,
    Triangle
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

    virtual void SetPointSize(float size) = 0;
    virtual void SetLineWidth(float width) = 0;

    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
    virtual void SetClearColor(glm::vec4 color) = 0;
    virtual void Clear() = 0;

    virtual void DrawIndexed(const Shared<VertexArray> &va, Primitive type = Primitive::Triangle) = 0;
    virtual void DrawIndexed(const Shared<VertexArray> &va, uint32_t count, Primitive type = Primitive::Triangle) = 0;

    static Unique<RenderingAPI> Create();
};

} // namespace Rush

#endif // __RENDERINGAPI_H__
