#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "RenderingAPI.h"
#include "Rush/core/Core.h"
#include "Rush/resources/Material.h"
#include "Rush/scene/Camera.h"
#include "Shader.h"
#include "UniformBuffer.h"
#include "VertexArray.h"

#include <stdint.h>
#include <vector>

namespace Rush {

struct RUSH_API RenderStats {
    int drawCallCount = 0;
    int vertexCount = 0;
    int primitiveCount = 0;
    int programCount = 0;
};

class RUSH_API Renderer {
private:
    struct RenderableKey {
        uint64_t sortParam;
        uint64_t renderableIndex;
    };

    struct Renderable {
        glm::mat4 model;
        Shared<Material> material;
        Shared<VertexArray> va;
    };

    struct RenderData {
        struct SceneData {
            glm::mat4 sceneVP;
            glm::vec3 cameraPos;
        };
        SceneData sceneData;
        Shared<UniformBuffer> sceneUniformBuffer;
        Shared<Shader> shaderOverride;

        std::vector<Renderable> submittedRenderables;
        std::vector<RenderableKey> keys;
    };
    static Unique<RenderingAPI> s_API;
    static RenderData s_Data;
    static RenderStats s_Stats;

public:
    static void Init();
    static void Shutdown();
    static void OnResize(uint32_t width, uint32_t height);
    static void BeginScene(Camera &camera, glm::mat4 &view, Shared<Shader> shaderOverride = nullptr);
    static void EndScene();

    static void ResetRenderStats();
    static RenderStats GetRenderStats() { return s_Stats; }

    static void Submit(const Shared<Shader> &shader, const Shared<VertexArray> &va, const glm::mat4 &model);
    static void Submit(Shared<Material> material, const Shared<VertexArray> &va, const glm::mat4 &model);

    static void RenderCube(const Shared<Shader> &shader, const glm::mat4 &model);

    static Unique<RenderingAPI> &GetAPI();
};

} // namespace Rush

#endif // __RENDERER_H__
