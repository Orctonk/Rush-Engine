#ifndef __ROOTMESH_H__
#define __ROOTMESH_H__

#include "Rush/core/Core.h"
#include "SubMesh.h"

#include <vector>
#include <string>

namespace Rush {

struct RUSH_API RootMesh {
    std::string name;
    std::vector<SubMesh> submeshes;
};

}

#endif // __ROOTMESH_H__
