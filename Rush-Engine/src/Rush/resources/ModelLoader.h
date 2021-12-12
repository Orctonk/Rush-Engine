#ifndef __MODEL_H__
#define __MODEL_H__

#include "Material.h"
#include "Path.h"
#include "RootMesh.h"
#include "Rush/core/Core.h"
#include "Rush/graphics/VertexArray.h"

#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <string>

namespace Rush {

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec2 texture;
};

class RUSH_API ModelLoader {
private:
    static Path s_CurDirectory;
    static Shared<Texture> s_DefaultColorTexture;
    static Shared<Texture> s_DefaultNormalTexture;

    static void ProcessNode(RootMesh &root, const aiNode *node, const aiScene *scene);
    static SubMesh ProcessMesh(const aiMesh *mesh, const aiScene *scene);
    static Shared<Material> ProcessMaterial(const aiMaterial *material, const aiScene *scene, const std::string &parentMesh);

public:
    ModelLoader() = delete;
    ~ModelLoader();

    static RootMesh LoadModel(const Path &filepath);
};

} // namespace Rush

#endif // __MODEL_H__
