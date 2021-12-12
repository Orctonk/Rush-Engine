#ifndef __ROOTMESH_H__
#define __ROOTMESH_H__

#include "Rush/core/Core.h"
#include "SubMesh.h"

#include <string>
#include <vector>

namespace Rush {

struct RUSH_API RootMesh {
    std::string name;
    std::vector<SubMesh> submeshes;
};

} // namespace Rush

#endif // __ROOTMESH_H__
