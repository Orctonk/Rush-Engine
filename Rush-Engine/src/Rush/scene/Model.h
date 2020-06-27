#ifndef __MODEL_H__
#define __MODEL_H__

#include "Rushpch.h"
#include "Rush/core/Core.h"
#include "Rush/graphics/VertexArray.h"

#include <assimp/scene.h>

namespace Rush {
    
class Model{
private:
    std::vector<Shared<VertexBuffer>> m_VBs;
    Unique<VertexArray> m_VA;

    void ProcessNode(const aiNode *node, const aiScene *scene);
    void ProcessMesh(const aiMesh *node, const aiScene *scene);
public:
    Model(std::string path);
    ~Model();
};

} // namespace Rush

#endif // __MODEL_H__
