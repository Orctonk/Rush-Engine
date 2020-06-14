#ifndef __OGLTEXTURE_H__
#define __OGLTEXTURE_H__

#include <stdint.h>

#include "Rush/graphics/Texture.h"

namespace Rush {

class OGLTexture : public Texture {
private:
    uint32_t m_Texture;

public:
    OGLTexture(std::string filepath);
    ~OGLTexture();
    OGLTexture(OGLTexture &) = delete;
    OGLTexture(const OGLTexture &) = delete;

    void Bind();
};
    
} // namespace Rush

#endif // __OGLTEXTURE_H__
