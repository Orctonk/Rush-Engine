#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "Rush/scene/Camera.h"
#include "Rush/core/Core.h"
#include "RenderingAPI.h"
#include "VertexArray.h"
#include "Shader.h"

#include <stdint.h>

namespace Rush {

struct RUSH_API RenderStats {
    int drawCallCount = 0;
    int vertexCount = 0;
    int primitiveCount = 0;
    int programCount = 0;
};

class RUSH_API Renderer {
private:
    static Unique<RenderingAPI> s_API;
    static glm::mat4 s_SceneVP;
    static glm::vec3 s_CameraPos;
    static RenderStats s_Stats;
public:
    static void Init();
    static void Shutdown();
    static void OnResize(uint32_t width, uint32_t height);
    static void BeginScene(Camera &camera, glm::mat4 &view);
    static void EndScene();

    static void ResetRenderStats();
    static RenderStats GetRenderStats(){return s_Stats;}

    static void Submit(const Shared<Shader> &shader, const Shared<VertexArray> &va, const glm::mat4 &model);

    static void RenderCube(const Shared<Shader> &shader, const glm::mat4 &model);

    static Unique<RenderingAPI> &GetAPI() { return s_API; }
};
    
} // namespace Rush


#endif // __RENDERER_H__
