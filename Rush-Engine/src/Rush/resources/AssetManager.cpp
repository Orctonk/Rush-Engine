#include "Rushpch.h"
#include "AssetManager.h"

#include "ModelLoader.h"

#ifdef RUSH_SHARED
    #define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb_image.h>

namespace Rush {

std::unordered_map<std::string,Shared<RootMesh>>    AssetManager::s_Meshes;
std::unordered_map<std::string,Shared<Material>>    AssetManager::s_Materials;
std::unordered_map<std::string,Shared<Texture>>     AssetManager::s_Textures;
std::unordered_map<std::string,Shared<Cubemap>>     AssetManager::s_Cubemaps;
std::unordered_map<std::string,Shared<Shader>>      AssetManager::s_Shaders;

void AssetManager::Init(){
    RUSH_PROFILE_FUNCTION();
    stbi_set_flip_vertically_on_load(1);
}

void AssetManager::Shutdown(){
    RUSH_PROFILE_FUNCTION();
    
}

Shared<RootMesh> AssetManager::GetMesh(const Path &path) {
    RUSH_PROFILE_FUNCTION();
    auto cached = s_Meshes.find(path.GetRawPath());
    if(cached == s_Meshes.end())
        s_Meshes.emplace(path.GetRawPath(),CreateShared<RootMesh>(ModelLoader::LoadModel(path)));

    return s_Meshes[path.GetRawPath()]; 
}

Shared<Material> AssetManager::GetMaterial(const Path &path) { 
    RUSH_PROFILE_FUNCTION();
    auto cached = s_Materials.find(path.GetRawPath());
    if(cached == s_Materials.end())
        s_Materials.emplace(path.GetRawPath(),Material::Load(path));

    return s_Materials[path.GetRawPath()]; 
}

Shared<Texture> AssetManager::GetTexture(const Path &path){
    RUSH_PROFILE_FUNCTION();
    auto cachedTex = s_Textures.find(path.GetRawPath());
    if(cachedTex != s_Textures.end()){
        return cachedTex->second;
    } else {
        Shared<Texture> tex = Texture::Create(path);
        s_Textures.emplace(path.GetRawPath(),tex);
        return tex;
    }
}

Shared<Cubemap> AssetManager::GetCubemap(const Path &path){
    RUSH_PROFILE_FUNCTION();
    auto cachedCM = s_Cubemaps.find(path.GetRawPath());
    if(cachedCM != s_Cubemaps.end()){
        return cachedCM->second;
    } else {
        Shared<Cubemap> cubemap = Cubemap::Create(path);
        s_Cubemaps.emplace(path.GetRawPath(),cubemap);
        return cubemap;
    }
}

Shared<Shader> AssetManager::GetShader(const Path &path){
    RUSH_PROFILE_FUNCTION();
    auto cachedShader = s_Shaders.find(path.GetRawPath());
    if(cachedShader != s_Shaders.end()){
        return cachedShader->second;
    } else {
        Shared<Shader> shader = Shader::Create(path);
        s_Shaders.emplace(path.GetRawPath(),shader);
        return shader;
    }
}

void AssetManager::PutMaterial(const std::string &path, Shared<Material> mat) {
    RUSH_PROFILE_FUNCTION();
    s_Materials[path] = mat;
}

bool AssetManager::HasMesh(const Path &path){
    RUSH_PROFILE_FUNCTION();
    return s_Meshes.find(path.GetRawPath()) != s_Meshes.end();
}

bool AssetManager::HasMaterial(const Path &path){
    RUSH_PROFILE_FUNCTION();
    return s_Materials.find(path.GetRawPath()) != s_Materials.end();
}

bool AssetManager::HasTexture(const Path &path){
    RUSH_PROFILE_FUNCTION();
    return s_Textures.find(path.GetRawPath()) != s_Textures.end();
}

bool AssetManager::HasCubemap(const Path &path){
    RUSH_PROFILE_FUNCTION();
    return s_Cubemaps.find(path.GetRawPath()) != s_Cubemaps.end();
}

bool AssetManager::HasShader(const Path &path){
    RUSH_PROFILE_FUNCTION();
    return s_Shaders.find(path.GetRawPath()) != s_Shaders.end();
}

std::vector<std::string> AssetManager::GetMeshKeys(){
    RUSH_PROFILE_FUNCTION();
    std::vector<std::string> keys;
    keys.reserve(s_Meshes.size());
    for(auto &val : s_Meshes)
        keys.push_back(val.first);
    return keys;
}

std::vector<std::string> AssetManager::GetMaterialKeys(){
    RUSH_PROFILE_FUNCTION();
    std::vector<std::string> keys;
    keys.reserve(s_Materials.size());
    for(auto &val : s_Materials)
        keys.push_back(val.first);
    return keys;
}

std::vector<std::string> AssetManager::GetTextureKeys(){
    RUSH_PROFILE_FUNCTION();
    std::vector<std::string> keys;
    keys.reserve(s_Textures.size());
    for(auto &val : s_Textures)
        keys.push_back(val.first);
    return keys;
}

std::vector<std::string> AssetManager::GetCubemapKeys(){
    RUSH_PROFILE_FUNCTION();
    std::vector<std::string> keys;
    keys.reserve(s_Cubemaps.size());
    for(auto &val : s_Cubemaps)
        keys.push_back(val.first);
    return keys;
}

std::vector<std::string> AssetManager::GetShaderKeys(){
    RUSH_PROFILE_FUNCTION();
    std::vector<std::string> keys;
    keys.reserve(s_Shaders.size());
    for(auto &val : s_Shaders)
        keys.push_back(val.first);
    return keys;
}

}