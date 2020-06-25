#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Rushpch.h"
#include "Rush/core/Core.h"

namespace Rush {

class RUSH_API Texture {
protected:
    Texture();

public:
    virtual ~Texture();

    Texture(Texture &) = delete;
    Texture(const Texture &) = delete;

    virtual void Bind(uint8_t textureSlot) = 0;

    static Shared<Texture> Create(std::string filepath);
};
    
} // namespace Rush


#endif // __TEXTURE_H__
