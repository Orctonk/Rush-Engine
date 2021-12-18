#ifndef __SCENE_H__
#define __SCENE_H__

#include "Entity.h"
#include "Rush/core/Core.h"
#include "Rush/graphics/Shader.h"
#include "Rush/graphics/UniformBuffer.h"
#include "Rush/resources/Path.h"
#include "components/LightComponent.h"

#include <entt/entity/registry.hpp>
namespace Rush {

class RUSH_API Scene {
private:
    struct LightBuffer {
        std::array<PackedLight, 15> lights;
        int32_t lightCount;
        int32_t dirLightCount;
    };

    LightBuffer m_LightBuffer;
    entt::registry m_SceneRegistry;
    Shared<Shader> m_SceneShader;
    Shared<Shader> m_SkyboxShader;
    Shared<UniformBuffer> m_LightUniformBuffer;

    bool m_Playing = false;

    void SetLightData(Shared<Shader> shader);

public:
    Scene();
    ~Scene();
    Scene(Scene &) = delete;
    Scene(const Scene &) = delete;

    Entity NewEntity(std::string name = "");
    void DeleteEntity(Entity e);

    entt::registry *GetRegistry() { return &m_SceneRegistry; }

    void OnUpdate();
    void Render();
    void StartPlay();
    void StopPlay();
    bool IsPlaying() { return m_Playing; }

    template <typename... Components>
    void Save(const Path &scenePath);

    template <typename... Components>
    void Load(const Path &scenePath);
};

} // namespace Rush

#include "Scene.inl"

#endif // __SCENE_H__
