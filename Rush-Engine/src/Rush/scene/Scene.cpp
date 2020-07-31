#include "Scene.h"

namespace Rush {

Scene::Scene(){

}

Scene::~Scene(){
    
}

Entity Scene::NewEntity(){
    return {&m_SceneRegistry, m_SceneRegistry.create()};
}

void Scene::DeleteEntity(Entity e){
    m_SceneRegistry.remove_all(e.m_Entity);
    m_SceneRegistry.destroy(e.m_Entity);
}

}