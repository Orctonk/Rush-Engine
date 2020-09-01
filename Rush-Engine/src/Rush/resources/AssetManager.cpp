#include "AssetManager.h"

#include "ModelLoader.h"

#define STB_IMAGE_IMPLEMENTATION
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

Shared<RootMesh> AssetManager::GetMesh(const std::string &path) {
    RUSH_PROFILE_FUNCTION();
    auto cached = s_Meshes.find(path);
    if(cached == s_Meshes.end())
        s_Meshes.emplace(path,CreateShared<RootMesh>(ModelLoader::LoadModel(path)));

    return s_Meshes[path]; 
}

Shared<Material> AssetManager::GetMaterial(const std::string &path) { 
    RUSH_PROFILE_FUNCTION();
    auto cached = s_Materials.find(path);
    if(cached == s_Materials.end())
        s_Materials.emplace(path,Material::Load(path));

    return s_Materials[path]; 
}

Shared<Texture> AssetManager::GetTexture(const std::string &path){
    RUSH_PROFILE_FUNCTION();
    auto cachedTex = s_Textures.find(path);
    if(cachedTex != s_Textures.end()){
        return cachedTex->second;
    } else {
        Shared<Texture> tex = Texture::Create(path);
        s_Textures.emplace(path,tex);
        return tex;
    }
}

Shared<Cubemap> AssetManager::GetCubemap(const std::string &path){
    RUSH_PROFILE_FUNCTION();
    auto cachedCM = s_Cubemaps.find(path);
    if(cachedCM != s_Cubemaps.end()){
        return cachedCM->second;
    } else {
        Shared<Cubemap> cubemap = Cubemap::Create(path);
        s_Cubemaps.emplace(path,cubemap);
        return cubemap;
    }
}

Shared<Shader> AssetManager::GetShader(const std::string &path){
    RUSH_PROFILE_FUNCTION();
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
    RUSH_PROFILE_FUNCTION();
    s_Materials[path] = mat;
}

bool AssetManager::HasMesh(const std::string &path){
    RUSH_PROFILE_FUNCTION();
    return s_Meshes.find(path) != s_Meshes.end();
}

bool AssetManager::HasMaterial(const std::string &path){
    RUSH_PROFILE_FUNCTION();
    return s_Materials.find(path) != s_Materials.end();
}

bool AssetManager::HasTexture(const std::string &path){
    RUSH_PROFILE_FUNCTION();
    return s_Textures.find(path) != s_Textures.end();
}

bool AssetManager::HasCubemap(const std::string &path){
    RUSH_PROFILE_FUNCTION();
    return s_Cubemaps.find(path) != s_Cubemaps.end();
}

bool AssetManager::HasShader(const std::string &path){
    RUSH_PROFILE_FUNCTION();
    return s_Shaders.find(path) != s_Shaders.end();
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