#include "Scene.h"

namespace Rush {

Scene::Scene(){

}

Scene::~Scene(){
    
}

Scene::EntityType Scene::NewEntity(){
    return m_SceneRegistry.create();
}

void Scene::DeleteEntity(EntityType e){
    m_SceneRegistry.remove_all(e);
    m_SceneRegistry.destroy(e);
}

}