#ifndef __ASSETMANAGER_H__
#define __ASSETMANAGER_H__

#include <stdint.h>

#include "Rushpch.h"
#include "Rush/core/Core.h"
#include "Rush/graphics/VertexArray.h"
#include "Rush/graphics/Shader.h"
#include "Rush/graphics/Texture.h"
#include "Rush/graphics/Cubemap.h"

#include "RootMesh.h"
#include "Material.h"

namespace Rush {

class AssetManager {
private:
    static std::unordered_map<std::string,Shared<RootMesh>> s_Meshes;
    static std::unordered_map<std::string,Shared<Material>> s_Materials;
    static std::unordered_map<std::string,Shared<Texture>> s_Textures;
    static std::unordered_map<std::string,Shared<Cubemap>> s_Cubemaps;
    static std::unordered_map<std::string,Shared<Shader>> s_Shaders;

public:
    using AssetID = uint32_t;

    AssetManager() = delete;
    
    static void Init();
    static void Shutdown();

    static Shared<RootMesh> GetMesh(const std::string &path);
    static Shared<Material> GetMaterial(const std::string &path);
    static Shared<Texture> GetTexture(const std::string &path);
    static Shared<Cubemap> GetCubemap(const std::string &path);
    static Shared<Shader> GetShader(const std::string &path);

    static void PutMaterial(const std::string &path, Shared<Material> mat);

    static bool HasMesh(const std::string &path);
    static bool HasMaterial(const std::string &path);
    static bool HasTexture(const std::string &path);
    static bool HasCubemap(const std::string &path);
    static bool HasShader(const std::string &path);

    static std::vector<std::string> GetMeshKeys();
    static std::vector<std::string> GetMaterialKeys();
    static std::vector<std::string> GetTextureKeys();
    static std::vector<std::string> GetCubemapKeys();
    static std::vector<std::string> GetShaderKeys();
};

}

#endif // __ASSETMANAGER_H__
