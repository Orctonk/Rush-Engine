#include "Rush/resources/File.h"
#include "Components.h"

#include <entt/entity/snapshot.hpp>
#include <cereal/archives/json.hpp>

#define ENGINE_COMPONENTS TransformComponent, CameraComponent, TagComponent, LightComponent, MeshRendererComponent

namespace Rush {

template<typename ...Components>
void Scene::Save(const Path &scenePath){
    File sf(scenePath);
    if(sf.Exists()){
        RUSH_LOG_WARNING("'{}' already exists, overwriting.", scenePath.GetRawPath());
        if(sf.GetType() != FileType::Regular){
            RUSH_LOG_ERROR("Failed to write scene '{}': File is not regular file!", scenePath.GetRawPath());
            return;
        }
    }

    auto fs = sf.OpenFile(OpenMode::Write);
    if(!fs.is_open()){
        RUSH_LOG_ERROR("Failed to open scene file '{}'!", scenePath.GetRawPath());
        return;
    }
    
    cereal::JSONOutputArchive archive(fs);

    entt::snapshot ss(m_SceneRegistry);
    ss.entities(archive).component<ENGINE_COMPONENTS>(archive);
}

template<typename ...Components>
void Scene::Load(const Path &scenePath){
    File sf(scenePath);
    if(!sf.Exists()){
        RUSH_LOG_ERROR("'{}' doesn't exist!", scenePath.GetRawPath());
        return;
    }
    if(sf.GetType() != FileType::Regular){
        RUSH_LOG_ERROR("Failed to load scene '{}': File is not regular file!", scenePath.GetRawPath());
        return;
    }

    auto fs = sf.OpenFile(OpenMode::Read);
    if(!fs.is_open()){
        RUSH_LOG_ERROR("Failed to open scene file '{}'!", scenePath.GetRawPath());
        return;
    }
    
    cereal::JSONInputArchive archive(fs);

    m_SceneRegistry.clear();
    entt::snapshot_loader sl(m_SceneRegistry);

    sl.entities(archive)
        .component<ENGINE_COMPONENTS>(archive);
}

}