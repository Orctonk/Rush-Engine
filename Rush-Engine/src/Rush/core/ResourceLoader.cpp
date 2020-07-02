#include "ResourceLoader.h"

namespace Rush {

std::unordered_map<std::string,Shared<Texture>> ResourceLoader::s_TexCache;
std::unordered_map<std::string,Shared<Shader>> ResourceLoader::s_ShaderCache;
std::unordered_map<std::string,Shared<VertexBuffer>> ResourceLoader::s_VertexCache;
std::unordered_map<std::string,Shared<IndexBuffer>> ResourceLoader::s_IndexCache;

void ResourceLoader::Init(){

}

void ResourceLoader::Shutdown(){

}

Shared<Texture> ResourceLoader::LoadTexture(std::string path){
    auto cachedTex = s_TexCache.find(path);
    if(cachedTex != s_TexCache.end()){
        return cachedTex->second;
    } else {
        Shared<Texture> tex = Texture::Create(path);
        s_TexCache.emplace(path,tex);
        return tex;
    }
}

Shared<Shader> ResourceLoader::LoadShader(std::string path){
    auto cachedShader = s_ShaderCache.find(path);
    if(cachedShader != s_ShaderCache.end()){
        return cachedShader->second;
    } else {
        Shared<Shader> shader = Shader::Create(path);
        s_ShaderCache.emplace(path,shader);
        return shader;
    }
}

Shared<Model> ResourceLoader::LoadModel(std::string path){
    return CreateShared<Model>(path);
}


}