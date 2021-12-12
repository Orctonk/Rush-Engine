#ifndef __ASSETMANAGER_H__
#define __ASSETMANAGER_H__

#include "Material.h"
#include "Path.h"
#include "RootMesh.h"
#include "Rush/core/Core.h"
#include "Rush/graphics/Cubemap.h"
#include "Rush/graphics/Shader.h"
#include "Rush/graphics/Texture.h"
#include "Rush/graphics/VertexArray.h"

#include <stdint.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace Rush {

class RUSH_API AssetManager {
private:
    static std::unordered_map<std::string, Shared<RootMesh>> s_Meshes;
    static std::unordered_map<std::string, Shared<Material>> s_Materials;
    static std::unordered_map<std::string, Shared<Texture>> s_Textures;
    static std::unordered_map<std::string, Shared<Cubemap>> s_Cubemaps;
    static std::unordered_map<std::string, Shared<Shader>> s_Shaders;

public:
    using AssetID = uint32_t;

    AssetManager() = delete;

    static void Init();
    static void Shutdown();

    static Shared<RootMesh> GetMesh(const Path &path);
    static Shared<Material> GetMaterial(const Path &path);
    static Shared<Texture> GetTexture(const Path &path);
    static Shared<Cubemap> GetCubemap(const Path &path);
    static Shared<Shader> GetShader(const Path &path);

    static void PutMaterial(const std::string &path, Shared<Material> mat);

    static bool HasMesh(const Path &path);
    static bool HasMaterial(const Path &path);
    static bool HasTexture(const Path &path);
    static bool HasCubemap(const Path &path);
    static bool HasShader(const Path &path);

    static std::vector<std::string> GetMeshKeys();
    static std::vector<std::string> GetMaterialKeys();
    static std::vector<std::string> GetTextureKeys();
    static std::vector<std::string> GetCubemapKeys();
    static std::vector<std::string> GetShaderKeys();
};

} // namespace Rush

#endif // __ASSETMANAGER_H__
