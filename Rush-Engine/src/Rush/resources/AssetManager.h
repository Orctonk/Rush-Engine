#ifndef __ASSETMANAGER_H__
#define __ASSETMANAGER_H__

#include <stdint.h>

#include "Rushpch.h"
#include "Rush/core/Core.h"
#include "Rush/graphics/VertexArray.h"
#include "Rush/graphics/Shader.h"
#include "Rush/graphics/Texture.h"

#include "MeshInstance.h"
#include "MaterialInstance.h"
#include "RootMesh.h"
#include "Material.h"

namespace Rush {

class AssetManager {
private:
    static std::unordered_map<std::string,Shared<RootMesh>> s_Meshes;
    static std::unordered_map<std::string,Shared<Material>> s_Materials;
    static std::unordered_map<std::string,Shared<Texture>> s_Textures;
    static std::unordered_map<std::string,Shared<Shader>> s_Shaders;

public:
    using AssetID = uint32_t;

    AssetManager() = delete;
    
    static void Init();
    static void Shutdown();

    static MeshInstance GetMeshInstance(const std::string &path);
    static MaterialInstance GetMaterialInstance(const std::string &path);
    static Shared<Texture> GetTexture(const std::string &path);
    static Shared<Shader> GetShader(const std::string &path);
};

}

#endif // __ASSETMANAGER_H__
