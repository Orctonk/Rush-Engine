#ifndef __SUBMESH_H__
#define __SUBMESH_H__

#include <Rushpch.h>
#include "Rush/core/Core.h"
#include "Rush/graphics/VertexArray.h"
#include "MaterialInstance.h"

struct RUSH_API SubMesh {
    std::string meshName;
    Rush::Shared<Rush::VertexArray> vertices;
    MaterialInstance material;
};

#endif // __SUBMESH_H__
