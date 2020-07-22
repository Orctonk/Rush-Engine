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

MaterialInstance AssetManager::GetMaterialInstance(const std::string &path) { 
    auto cached = s_Materials.find(path);
    if(cached == s_Materials.end())
        RUSH_ASSERT(false); // Asset manager currently relies on models loading their mats
        //s_Materials.emplace(path,CreateShared<RootMesh>(ModelLoader::LoadModel(path)));

    return {s_Materials[path]}; 
}

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

void AssetManager::PutMaterial(const std::string &path, Shared<Material> mat) {
    s_Materials[path] = mat;
}

bool AssetManager::HasMesh(const std::string &path){
    return s_Meshes.find(path) != s_Meshes.end();
}

bool AssetManager::HasMaterial(const std::string &path){
    return s_Materials.find(path) != s_Materials.end();
}

bool AssetManager::HasTexture(const std::string &path){
    return s_Textures.find(path) != s_Textures.end();
}

bool AssetManager::HasShader(const std::string &path){
    return s_Shaders.find(path) != s_Shaders.end();
}

std::vector<std::string> AssetManager::GetMeshKeys(){
    std::vector<std::string> keys;
    keys.reserve(s_Meshes.size());
    for(auto &val : s_Meshes)
        keys.push_back(val.first);
    return keys;
}

std::vector<std::string> AssetManager::GetMaterialKeys(){
    std::vector<std::string> keys;
    keys.reserve(s_Materials.size());
    for(auto &val : s_Materials)
        keys.push_back(val.first);
    return keys;
}

std::vector<std::string> AssetManager::GetTextureKeys(){
    std::vector<std::string> keys;
    keys.reserve(s_Textures.size());
    for(auto &val : s_Textures)
        keys.push_back(val.first);
    return keys;
}

std::vector<std::string> AssetManager::GetShaderKeys(){
    std::vector<std::string> keys;
    keys.reserve(s_Shaders.size());
    for(auto &val : s_Shaders)
        keys.push_back(val.first);
    return keys;
}


}