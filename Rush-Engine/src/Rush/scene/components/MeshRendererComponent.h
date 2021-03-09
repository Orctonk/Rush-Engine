#ifndef __MESHRENDERERCOMPONENT_H__
#define __MESHRENDERERCOMPONENT_H__

#include "Rush/core/Core.h"
#include "Rush/resources/RootMesh.h"
#include "Rush/resources/AssetManager.h"

#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>
#include <entt/core/attribute.h>

// TODO: Add mesh instances to allow renderer-specific materials on same meshes

struct ENTT_API MeshRendererComponent {
    Rush::Shared<Rush::RootMesh> mesh;

    template<typename Archive>
    void save(Archive &archive) const {
        archive(
            cereal::make_nvp("mesh",mesh->name)
        );
    }

    template<typename Archive>
    void load(Archive &archive){
        std::string meshPath;
        archive(
            cereal::make_nvp("mesh",meshPath)
        );
        mesh = Rush::AssetManager::GetMesh(meshPath);
    }
};

#endif // __MESHRENDERERCOMPONENT_H__
