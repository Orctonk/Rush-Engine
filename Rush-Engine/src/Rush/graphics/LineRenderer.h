#ifndef __LINERENDERER_H__
#define __LINERENDERER_H__

#include "Rush/core/Core.h"
#include "RenderingAPI.h"

#include <vector>
#include <glm/glm.hpp>

namespace Rush {

struct RUSH_API LineRenderStats {
    int drawCallCount = 0;
    int vertexCount = 0;
    int lineCount = 0;
};
    
class RUSH_API LineRenderer {
private:
    static Unique<RenderingAPI> s_API;
    static LineRenderStats s_Stats;
    static glm::mat4 s_SceneVP;

public:
    static void Init();
    static void Shutdown();
    static void BeginScene(glm::mat4 projection = glm::mat4(1.0f), glm::mat4 view = glm::mat4(1.0f));
    static void EndScene();
    static void Flush();

    static void DrawLine(glm::vec2 start, glm::vec2 end, glm::vec4 color);
    static void DrawLine(glm::vec3 start, glm::vec3 end, glm::vec4 color);
    static void DrawLines(std::vector<glm::vec3> lines, glm::vec4 color);

    static void ResetRenderStats();
    static LineRenderStats GetRenderStats() { return s_Stats; }

    static Unique<RenderingAPI> &GetAPI() { return s_API; }
};

} // namespace Rush


#endif // __LINERENDERER_H__
