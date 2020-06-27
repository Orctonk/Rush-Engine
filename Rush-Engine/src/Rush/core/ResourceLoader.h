#ifndef __RESOURCELOADER_H__
#define __RESOURCELOADER_H__
    
#include "Rushpch.h"
#include "Core.h"
#include "Rush/graphics/VertexArray.h"
#include "Rush/graphics/Shader.h"
#include "Rush/graphics/Texture.h"

namespace Rush {

class ResourceLoader{
private:
    static std::unordered_map<const char *,Shared<Texture>> s_TexCache;
    static std::unordered_map<const char *,Shared<Shader>> s_ShaderCache;
    static std::unordered_map<const char *,Shared<VertexBuffer>> s_VertexCache;
    static std::unordered_map<const char *,Shared<IndexBuffer>> s_IndexCache;

public:
    static void Init();
    static void Shutdown();
    
    static Shared<Texture> LoadTexture(const char *path);
    static Shared<Shader> LoadShader(const char *path);
    static Unique<VertexArray> LoadModel(const char *path);
};

} // namespace Rush

#endif // __RESOURCELOADER_H__
