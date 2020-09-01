#ifndef __ROOTMESH_H__
#define __ROOTMESH_H__

#include "Rushpch.h"
#include "SubMesh.h"

namespace Rush {

struct RootMesh {
    std::string name;
    std::vector<SubMesh> submeshes;
};

}

#endif // __ROOTMESH_H__
