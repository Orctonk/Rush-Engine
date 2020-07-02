#ifndef __MESH_H__
#define __MESH_H__

#include "Rush/core/Core.h"
#include "Rush/graphics/VertexArray.h"
#include "Material.h"

struct RUSH_API Mesh {
    Rush::Unique<Rush::VertexArray> vertices;
    Rush::Material material;
};

#endif // __MESH_H__
