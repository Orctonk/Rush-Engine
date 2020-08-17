#ifndef __RENDERER2D_H__
#define __RENDERER2D_H__

#include "Rush/core/Core.h"
#include "RenderingAPI.h"
#include "Texture.h"

#include <glm/glm.hpp>

namespace Rush {

struct RUSH_API RenderStats2D {
    int drawCallCount = 0;
    int vertexCount = 0;
    int primitiveCount = 0;
};

class RUSH_API Renderer2D {
private:
    static Unique<RenderingAPI> s_API;
    static RenderStats2D s_Stats;
    static glm::mat4 s_SceneVP;
    static glm::vec3 s_CameraUp;
    static glm::vec3 s_CameraRight;

public:
    static void Init();
    static void Shutdown();
    static void BeginScene(glm::mat4 projection = glm::mat4(1.0f), glm::mat4 view = glm::mat4(1.0f));
    static void EndScene();
    static void Flush();

    static void DrawQuad(glm::mat4 transform, Shared<Texture> texture, glm::vec4 color);
    static void DrawQuad(glm::mat4 transform, glm::vec4 color);

    static void DrawQuad(glm::vec2 pos, glm::vec2 size, glm::vec4 color);
    static void DrawQuad(glm::vec3 pos, glm::vec2 size, glm::vec4 color);
    static void DrawQuad(glm::vec2 pos, glm::vec2 size, float rotation, glm::vec4 color);
    static void DrawQuad(glm::vec3 pos, glm::vec2 size, float rotation, glm::vec4 color);
    static void DrawQuad(glm::vec2 pos, glm::vec2 size, Shared<Texture> texture, glm::vec4 color);
    static void DrawQuad(glm::vec3 pos, glm::vec2 size, Shared<Texture> texture, glm::vec4 color);
    static void DrawQuad(glm::vec2 pos, glm::vec2 size, float rotation, Shared<Texture> texture, glm::vec4 color);
    static void DrawQuad(glm::vec3 pos, glm::vec2 size, float rotation, Shared<Texture> texture, glm::vec4 color);

    static void DrawBillboard(glm::vec3 pos, glm::vec2 size, glm::vec4 color);
    static void DrawBillboard(glm::vec3 pos, glm::vec2 size, Shared<Texture> texture, glm::vec4 color);

    static void ResetRenderStats();
    static RenderStats2D GetRenderStats() { return s_Stats; }

    static Unique<RenderingAPI> &GetAPI() { return s_API; }
};

}

#endif // __RENDERER2D_H__
