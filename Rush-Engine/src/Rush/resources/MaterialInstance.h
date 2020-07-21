#ifndef __MATERIALINSTANCE__
#define __MATERIALINSTANCE__

#include "Rush/core/Core.h"
#include "Material.h"

struct MaterialInstance{
    Rush::Shared<Rush::Material> parent;
};

#endif // __MATERIALINSTANCE__
