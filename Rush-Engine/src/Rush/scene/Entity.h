#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Rush/core/Core.h"
#include "Scene.h"

#include <entt/entt.hpp>

namespace Rush {

class Entity {
private:
    entt::registry *m_Scene = nullptr;
    entt::entity m_Entity = entt::null;

public:
    friend class Scene;

    Entity(){}

    Entity(entt::registry *scene, entt::entity entity)
    : m_Scene(scene), m_Entity(entity) {}

    ~Entity() = default;

    uint32_t GetID() { return entt::to_integral(m_Entity); }
    entt::registry *GetParentRegistry(){ return m_Scene; }

    template<typename T>
    bool HasComponent(){
        return m_Scene->has<T>(m_Entity);
    }

    template<typename T, typename ... Args>
    T &AddComponent(Args&& ...args){
        return m_Scene->emplace<T>(m_Entity, std::forward<Args>(args) ...);
    }

    template<typename T>
    T &GetComponent(){
        return m_Scene->get<T>(m_Entity);
    }

    template<typename T>
    void RemoveComponent(){
        m_Scene->remove<T>(m_Entity);
    }

    operator bool(){
        return m_Entity != entt::null;
    }
    
    operator entt::entity(){ // TODO: Remove
        return m_Entity;
    }

    bool operator==(Entity &other){
        return m_Entity == other.m_Entity;
    }

};

}

#endif // __ENTITY_H__
