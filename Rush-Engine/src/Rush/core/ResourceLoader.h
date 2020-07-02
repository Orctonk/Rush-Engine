#ifndef __RESOURCELOADER_H__
#define __RESOURCELOADER_H__
    
#include "Rushpch.h"
#include "Core.h"
#include "Rush/graphics/VertexArray.h"
#include "Rush/graphics/Shader.h"
#include "Rush/graphics/Texture.h"
#include "Rush/scene/Model.h"

namespace Rush {

class ResourceLoader{
private:
    static std::unordered_map<std::string,Shared<Texture>> s_TexCache;
    static std::unordered_map<std::string,Shared<Shader>> s_ShaderCache;
    static std::unordered_map<std::string,Shared<VertexBuffer>> s_VertexCache;
    static std::unordered_map<std::string,Shared<IndexBuffer>> s_IndexCache;

public:
    static void Init();
    static void Shutdown();
    
    static Shared<Texture> LoadTexture(std::string path);
    static Shared<Shader> LoadShader(std::string path);
    static Shared<Model> LoadModel(std::string path);
};

} // namespace Rush

#endif // __RESOURCELOADER_H__
