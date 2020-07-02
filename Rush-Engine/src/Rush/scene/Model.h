#ifndef __MODEL_H__
#define __MODEL_H__

#include "Rushpch.h"
#include "Mesh.h"
#include "Material.h"
#include "Rush/core/Core.h"
#include "Rush/graphics/VertexArray.h"

#include <glm/glm.hpp>
#include <assimp/scene.h>

namespace Rush {

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec2 texture;
};
    
class RUSH_API Model{
private:
    std::vector<Mesh> m_Meshes;
    std::string m_Directory;

    void ProcessNode(const aiNode *node, const aiScene *scene);
    Mesh ProcessMesh(const aiMesh *mesh, const aiScene *scene);
    Material ProcessMaterial(const aiMaterial *material, const aiScene *scene);
public:
    Model(std::string path);
    ~Model();

    std::vector<Mesh> &getMeshes(){ return m_Meshes; }
};

} // namespace Rush

#endif // __MODEL_H__
