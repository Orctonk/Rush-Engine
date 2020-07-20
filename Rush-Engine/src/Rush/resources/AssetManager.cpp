#include "AssetManager.h"

#include "ModelLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Rush {

std::unordered_map<std::string,Shared<RootMesh>>    AssetManager::s_Meshes;
std::unordered_map<std::string,Shared<Material>>    AssetManager::s_Materials;
std::unordered_map<std::string,Shared<Texture>>     AssetManager::s_Textures;
std::unordered_map<std::string,Shared<Shader>>      AssetManager::s_Shaders;

void AssetManager::Init(){
    stbi_set_flip_vertically_on_load(1);
}

void AssetManager::Shutdown(){
    
}

MeshInstance AssetManager::GetMeshInstance(const std::string &path) {
    auto cached = s_Meshes.find(path);
    if(cached == s_Meshes.end())
        s_Meshes.emplace(path,CreateShared<RootMesh>(ModelLoader::LoadModel(path)));

    return {s_Meshes[path]}; 
}

MaterialInstance AssetManager::GetMaterialInstance(const std::string &path) { }

Shared<Texture> AssetManager::GetTexture(const std::string &path){
    auto cachedTex = s_Textures.find(path);
    if(cachedTex != s_Textures.end()){
        return cachedTex->second;
    } else {
        Shared<Texture> tex = Texture::Create(path);
        s_Textures.emplace(path,tex);
        return tex;
    }
}

Shared<Shader> AssetManager::GetShader(const std::string &path){
    auto cachedShader = s_Shaders.find(path);
    if(cachedShader != s_Shaders.end()){
        return cachedShader->second;
    } else {
        Shared<Shader> shader = Shader::Create(path);
        s_Shaders.emplace(path,shader);
        return shader;
    }
}

}