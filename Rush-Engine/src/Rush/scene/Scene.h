#ifndef __SCENE_H__
#define __SCENE_H__

#include "Rush/core/Core.h"
#include "Rush/resources/Path.h"
#include "Rush/graphics/Shader.h"
#include "Entity.h"

#include <entt/entity/registry.hpp>

namespace Rush {
    
class Scene{
private:
    entt::registry m_SceneRegistry;
    Shared<Shader> m_SceneShader;
    Shared<Shader> m_SkyboxShader;

    void SetLightData(Shared<Shader> shader);
public:
    Scene();
    ~Scene();
    Scene(Scene &) = delete;
    Scene(const Scene &) = delete;

    Entity NewEntity();
    void DeleteEntity(Entity e);

    entt::registry *GetRegistry(){ return &m_SceneRegistry; }

    void OnUpdate();
    void Render();

    template<typename ...Components>
    void Save(const Path &scenePath);

    template<typename ...Components>
    Scene Load(const Path &scenePath);
};

} // namespace Rush

#include "Scene.inl"

#endif // __SCENE_H__
