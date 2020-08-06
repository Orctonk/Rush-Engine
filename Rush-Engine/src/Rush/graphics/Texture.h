#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Rushpch.h"
#include "Rush/core/Core.h"

namespace Rush {

struct TextureProperties {
    int width,height;
    int channels = 4;
    int bpp = 8;
};

class RUSH_API Texture {
private:
    std::string m_DebugPath;
protected:
    TextureProperties m_Props;

    Texture(std::string filepath);

public:
    virtual ~Texture();

    Texture(Texture &) = delete;
    Texture(const Texture &) = delete;

    virtual void Bind(uint8_t textureSlot) = 0;
    virtual uint32_t GetID() = 0;
    // WARNING: Use with caution, this function is sloooooow
    virtual void GetTextureData(unsigned char *buffer, unsigned int bufsize) = 0;


    static Shared<Texture> Create(std::string filepath);

    std::string GetDebugPath() { return m_DebugPath; }
    TextureProperties GetProperties(){ return m_Props; }
};
    
} // namespace Rush


#endif // __TEXTURE_H__
