#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Rush/core/Core.h"
#include "Rush/resources/Path.h"

#include <string>
#include <stdint.h>

namespace Rush {

enum class RUSH_API TextureFormat {
    RGBA8,
    RGBA16F,
    DEPTH24STENCIL8,
    DEPTH32
};

struct RUSH_API TextureProperties {
    int width,height;
    int channels = 4;
    int bpp = 8;
    TextureFormat format;
};

class RUSH_API Texture {
private:
    std::string m_DebugName;
protected:
    TextureProperties m_Props;

    Texture(std::string filepath);
    Texture(uint32_t width, uint32_t height, TextureFormat format, uint8_t *data, std::string name);

public:
    virtual ~Texture();

    Texture(Texture &) = delete;
    Texture(const Texture &) = delete;

    virtual void Bind(uint8_t textureSlot) = 0;
    virtual uint32_t GetID() = 0;

    // WARNING: Use with caution, this function is sloooooow
    virtual void GetTextureData(unsigned char *buffer, unsigned int bufsize) = 0;


    static Shared<Texture> Create(Path filepath); 
    static Shared<Texture> Create(uint32_t width, uint32_t height, TextureFormat format, uint8_t* data, std::string name = "Created texture");

    std::string GetDebugName() { return m_DebugName; }
    TextureProperties GetProperties(){ return m_Props; }
};
    
} // namespace Rush


#endif // __TEXTURE_H__
