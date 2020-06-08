#include "ShaderDataUtil.h"
#include "Rush/core/Core.h"

namespace Rush {
    
GLenum getOpenGLType(ShaderData type){
    switch (type){
    case ShaderData::FLOAT:
    case ShaderData::FLOAT2:
    case ShaderData::FLOAT3:
    case ShaderData::FLOAT4:
        return GL_FLOAT;
    case ShaderData::INT:
    case ShaderData::INT2:
    case ShaderData::INT3:
    case ShaderData::INT4:
        return GL_INT;
    case ShaderData::BOOL:
        return GL_BOOL;
    case ShaderData::MAT3:
        return GL_FLOAT_MAT3;
    case ShaderData::MAT4:
        return GL_FLOAT_MAT4;
    default:
        RUSH_ASSERT(false);
        return 0;
    }
}

uint8_t getElemCount(ShaderData type){
    switch (type){
    case ShaderData::FLOAT:
    case ShaderData::INT:
    case ShaderData::BOOL:
        return 1;
    case ShaderData::FLOAT2:
    case ShaderData::INT2:
        return 2;
    case ShaderData::FLOAT3:
    case ShaderData::INT3:
        return 3;
    case ShaderData::FLOAT4:
    case ShaderData::INT4:
        return 4;
    case ShaderData::MAT3:
        return 9;
    case ShaderData::MAT4:
        return 16;
    default:
        RUSH_ASSERT(false);
        return 0;
    }
}

uint8_t getSize(ShaderData type){
    uint8_t size = 0;
    switch (type){
    case ShaderData::FLOAT:
    case ShaderData::FLOAT2:
    case ShaderData::FLOAT3:
    case ShaderData::FLOAT4:
        size = sizeof(float);
        break;
    case ShaderData::INT:
    case ShaderData::INT2:
    case ShaderData::INT3:
    case ShaderData::INT4:
        size = sizeof(int);
        break;
    case ShaderData::BOOL:
        size = sizeof(bool);
        break;
    case ShaderData::MAT3:
        size = sizeof(float);
        break;
    case ShaderData::MAT4:
        size = sizeof(float);
        break;
    default:
        RUSH_ASSERT(false);
        return 0;
    }
    return size * getElemCount(type);
}

} // namespace Rush