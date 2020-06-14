#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Rushpch.h"
#include "Rush/core/Core.h"

namespace Rush {

class RUSH_API Texture {
protected:
    Texture(std::string filepath);

public:
    virtual ~Texture();

    Texture(Texture &) = delete;
    Texture(const Texture &) = delete;

    static Shared<Texture> Create(std::string filepath);
};
    
} // namespace Rush


#endif // __TEXTURE_H__
