#ifndef __SUBMESH_H__
#define __SUBMESH_H__

#include "Rush/core/Core.h"
#include "Rush/graphics/VertexArray.h"
#include "Material.h"

namespace Rush {

struct RUSH_API SubMesh {
    std::string meshName;
    Shared<VertexArray> vertices;
    Shared<Material> material;
};

}


#endif // __SUBMESH_H__
