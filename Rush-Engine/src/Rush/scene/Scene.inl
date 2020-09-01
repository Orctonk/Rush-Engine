#include "Scene.h"

#include "Rush/resources/File.h"
#include "Rush/core/Logger.h"
#include "Components.h"

#include <entt/entity/snapshot.hpp>
#include <cereal/archives/json.hpp>

namespace Rush {

template<typename Archive>
void serialize(Archive &archive, TransformComponent &tc){
    archive(
        cereal::make_nvp("PosX",tc.m_Translation.x),
        cereal::make_nvp("PosY",tc.m_Translation.y),
        cereal::make_nvp("PosZ",tc.m_Translation.z)
    );
}

template<typename ...Components>
void Scene::Save(const Path &scenePath){
    File sf(scenePath);
    if(sf.Exists()){
        RUSH_LOG_WARNING("'" + scenePath.GetRawPath() + "' already exists, overwriting.");
        if(sf.GetType() != FileType::Regular){
            RUSH_LOG_ERROR("Failed to load scene '" + scenePath.GetRawPath() + "': File is not regular file!");
            return;
        }
    }

    auto fs = sf.OpenFile(OpenMode::Write);
    if(!fs.is_open()){
        RUSH_LOG_ERROR("Failed to open scene file '" + scenePath.GetRawPath() + "'!");
        return;
    }
    
    fs << "Test";
}

template<typename ...Components>
Scene Scene::Load(const Path &scenePath){
    return Scene();
}

}