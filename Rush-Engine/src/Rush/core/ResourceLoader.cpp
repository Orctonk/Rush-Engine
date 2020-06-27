#include "ResourceLoader.h"

namespace Rush {

std::unordered_map<const char *,Shared<Texture>> ResourceLoader::s_TexCache;
std::unordered_map<const char *,Shared<Shader>> ResourceLoader::s_ShaderCache;
std::unordered_map<const char *,Shared<VertexBuffer>> ResourceLoader::s_VertexCache;
std::unordered_map<const char *,Shared<IndexBuffer>> ResourceLoader::s_IndexCache;

void ResourceLoader::Init(){

}

void ResourceLoader::Shutdown(){

}

Shared<Texture> ResourceLoader::LoadTexture(const char *path){
    auto cachedTex = s_TexCache.find(path);
    if(cachedTex != s_TexCache.end()){
        return cachedTex->second;
    } else {
        Shared<Texture> tex = Texture::Create(path);
        s_TexCache.emplace(path,tex);
        return tex;
    }
}

Shared<Shader> ResourceLoader::LoadShader(const char *path){
    auto cachedShader = s_ShaderCache.find(path);
    if(cachedShader != s_ShaderCache.end()){
        return cachedShader->second;
    } else {
        Shared<Shader> shader = Shader::Create(path);
        s_ShaderCache.emplace(path,shader);
        return shader;
    }
}

Unique<VertexArray> ResourceLoader::LoadModel(const char *path){
    
}


}