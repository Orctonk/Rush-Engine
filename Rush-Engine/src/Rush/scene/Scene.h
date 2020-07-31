#ifndef __SCENE_H__
#define __SCENE_H__

#include "Entity.h"

#include <entt/entity/registry.hpp>

namespace Rush {
    
class Scene{
private:
    entt::registry m_SceneRegistry;

public:
    Scene();
    ~Scene();
    Scene(Scene &) = delete;
    Scene(const Scene &) = delete;


    Entity NewEntity();
    void DeleteEntity(Entity e);

    entt::registry *GetRegistry(){ return &m_SceneRegistry; }
};

} // namespace Rush


#endif // __SCENE_H__
