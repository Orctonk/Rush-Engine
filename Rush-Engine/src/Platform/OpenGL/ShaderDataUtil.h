#ifndef __SHADERDATAUTIL_H__
#define __SHADERDATAUTIL_H__

#include "Rush/graphics/Shader.h"

#include <glad/glad.h>

namespace Rush {
    
GLenum getOpenGLType(ShaderData type);

uint8_t getElemCount(ShaderData type);

uint8_t getSize(ShaderData type);

} // namespace Rush

#endif // __SHADERDATAUTIL_H__
