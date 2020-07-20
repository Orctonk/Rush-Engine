#ifndef __SCENE_H__
#define __SCENE_H__

#include <entt/entity/registry.hpp>

namespace Rush {
    
class Scene{
private:
    entt::registry m_SceneRegistry;

public:
    using EntityType = entt::entity;
    
    static constexpr EntityType nullEnt = entt::null;

    Scene();
    ~Scene();

    EntityType NewEntity();
    void DeleteEntity(EntityType e);

    template<typename Func>
    void ForEach(Func func){
        m_SceneRegistry.each(func);
    }

    template<typename Comp,typename ... Args>
    Comp &Add(EntityType e, Args&& ... args){
        return m_SceneRegistry.emplace<Comp>(e,std::forward<Args>(args) ...);
    }

    template<typename Comp>
    Comp &Get(EntityType e){
        return m_SceneRegistry.get<Comp>(e);
    }

    template<typename Comp>
    void Remove(EntityType e){
        m_SceneRegistry.remove<Comp>(e);
    }

    template<typename Comp>
    bool Has(EntityType e){
        return m_SceneRegistry.has<Comp>(e);
    }

    template<typename Comp>
    auto View(){
        return m_SceneRegistry.view<Comp>();
    }

    entt::registry *GetRegistry(){ return &m_SceneRegistry; }
};

} // namespace Rush


#endif // __SCENE_H__
