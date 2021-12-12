#include "Components.h"
#include "Rush/resources/File.h"

#include <cereal/archives/json.hpp>
#include <entt/entity/snapshot.hpp>

#define ENGINE_COMPONENTS    ENGINE_COMPONENTS_V2
#define ENGINE_COMPONENTS_V2 TransformComponent, CameraComponent, TagComponent, LightComponent, MeshRendererComponent, ScriptComponent
#define ENGINE_COMPONENTS_V1 TransformComponent, CameraComponent, TagComponent, LightComponent, MeshRendererComponent

namespace Rush {

template <typename... Components>
void Scene::Save(const Path &scenePath) {
    File sf(scenePath);
    if (sf.Exists()) {
        RUSH_LOG_WARNING("'{}' already exists, overwriting.", scenePath.GetRawPath());
        if (sf.GetType() != FileType::Regular) {
            RUSH_LOG_ERROR("Failed to write scene '{}': File is not regular file!", scenePath.GetRawPath());
            return;
        }
    }

    auto fs = sf.OpenFile(OpenMode::Write);
    if (!fs.is_open()) {
        RUSH_LOG_ERROR("Failed to open scene file '{}'!", scenePath.GetRawPath());
        return;
    }

    cereal::JSONOutputArchive archive(fs);

    archive(cereal::make_nvp("version", 2));

    entt::snapshot ss(m_SceneRegistry);
    ss.entities(archive).component<ENGINE_COMPONENTS>(archive);
}

template <typename... Components>
void Scene::Load(const Path &scenePath) {
    File sf(scenePath);
    if (!sf.Exists()) {
        RUSH_LOG_ERROR("'{}' doesn't exist!", scenePath.GetRawPath());
        return;
    }
    if (sf.GetType() != FileType::Regular) {
        RUSH_LOG_ERROR("Failed to load scene '{}': File is not regular file!", scenePath.GetRawPath());
        return;
    }

    auto fs = sf.OpenFile(OpenMode::Read);
    if (!fs.is_open()) {
        RUSH_LOG_ERROR("Failed to open scene file '{}'!", scenePath.GetRawPath());
        return;
    }
    cereal::JSONInputArchive archive(fs);
    int version = 0;
    try {
        archive(cereal::make_nvp("version", version));
    } catch (...) {
        RUSH_LOG_WARNING("No version in scene file, assuming version 1");
        version = 1;
    }

    m_SceneRegistry.clear();
    entt::snapshot_loader sl(m_SceneRegistry);
    RUSH_LOG_INFO("Loading scene '{}' (v{})", scenePath.GetRawPath(), version);

    if (version == 1)
        sl.entities(archive).component<ENGINE_COMPONENTS_V1>(archive);
    else if (version == 2)
        sl.entities(archive).component<ENGINE_COMPONENTS_V2>(archive);
}

} // namespace Rush