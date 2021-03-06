#ifndef __MODEL_H__
#define __MODEL_H__

#include "RootMesh.h"
#include "Material.h"
#include "Rush/core/Core.h"
#include "Rush/graphics/VertexArray.h"

#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <string>

namespace Rush {

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec2 texture;
};
    
class RUSH_API ModelLoader{
private:
    static std::string s_CurDirectory;
    static Shared<Texture> s_DefaultColorTexture;
    static Shared<Texture> s_DefaultNormalTexture;

    static void ProcessNode(RootMesh &root, const aiNode *node, const aiScene *scene);
    static SubMesh ProcessMesh(const aiMesh *mesh, const aiScene *scene);
    static Shared<Material> ProcessMaterial(const aiMaterial *material, const aiScene *scene, const std::string &parentMesh);
public:
    ModelLoader() = delete;
    ~ModelLoader();

    static RootMesh LoadModel(const std::string &path);
};

} // namespace Rush

#endif // __MODEL_H__
