#ifndef __SUBMESH_H__
#define __SUBMESH_H__

#include "Material.h"
#include "Rush/core/Core.h"
#include "Rush/graphics/VertexArray.h"

namespace Rush {

struct RUSH_API SubMesh {
    std::string meshName;
    Shared<VertexArray> vertices;
    Shared<Material> material;
};

} // namespace Rush

#endif // __SUBMESH_H__
